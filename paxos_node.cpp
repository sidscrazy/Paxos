#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <vector>
#include <sys/wait.h>
#include <iostream>
#include <thread>
#include <mutex>
#include "paxos_message.h"
#include "./plan/PaxosNodeLogger.h"
#include <queue>
#include <memory.h>
#include <ctime>
#include <chrono>

class PaxosNode {
private:
	int id;
	int num_nodes;
	int role;
	int v;
	int n;
	int state;
	bool proposed_v = false;
	PaxosNodeLogger *log;
	
	std::mutex network_lock;
	std::mutex queue_lock;
	std::queue<message *> messages;

	/* Proposer and Learner Relevant Data. 
	   Proposers need to track how many accepting
	   acks they have received for prepare messages.
	   Learners need to track how many votes for 
	   value v there are in round n. */
	std::vector<vote *> votes;
	std::vector<int> prepare_acks;

	/* Crash Management Members. By default, 
	   there is a 5% to crash upon any event 
	   that can trigger a crash. Crashing is 
	   split evenly among network and machine
	   failures, but handled distinctly. */
	int p_crash = P_CRASH;
	int crash_dist = C_DIST;


	/* Handle timeout events. */
	time_t timeout;



	void safe_exit (message *m) {
		log->CloseLogFile ();
		m->type = MSG_TEARDOWN;
		network_lock.lock ();
		send_message (0, m);
		while (true){
			m = receive_packet (1);
			if (m != NULL){
				if (m->type == MSG_TEARDOWN){
					network_lock.unlock ();
					exit (0);
				}
			}
		}
	}

	void network_listener (){
		message *m;
		while (true){
			network_lock.lock ();
			m = receive_packet (1);
			network_lock.unlock ();
			if (m != NULL){
				queue_lock.lock ();
				messages.push (m);
				queue_lock.unlock ();
			}
		}
	}

	void clear_network () {
		message *m;
		while (true){
			m = receive_packet (1);
			if (m == NULL){
				break;
			} else {
				free (m);
			}
		}

	}

	void notify_crash (int code) {
		message m (id, -1, code, n, MSG_CRASH);
		auto now = std::chrono::system_clock::now();
   		m.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
		send_packet (0, &m);
	}

	/* Randomly simulate a crash. */
	void simulate_crash (){
		int c = rand () % 100;
		int crash_type = rand () % 100;
		if (c < p_crash){
			/* Don't sleep while holding locks. It's fine
			   to just modify system state accordingly when
			   waking up. */
			network_lock.lock ();
			log->Crash ();
			notify_crash (CRASH_ENTRY);
			network_lock.unlock ();

			int crash_duration = (rand () % (CRASH_DUR * 3) ) + 1;
			sleep (crash_duration);

			network_lock.lock ();
			queue_lock.lock ();
			while (!messages.empty ()) {
				message *m = messages.front ();
				messages.pop ();
				free (m);
			}
			queue_lock.unlock ();
			/* Machine Failure. Clear all current
			   state and then recover from log. */
			if (crash_type < crash_dist) {
				clear_responses ();
			}


			clear_network ();
			log->ResumeFromCrash ();
			notify_crash (RESUME_ENTRY);
			network_lock.unlock ();
		}
	}

	void check_timeout (){
		if (role & PROPOSER){
			time_t timeStamp = time(0);
			if (timeStamp - timeout  > TIMEOUT) {
				clear_responses ();
				proposer_state_update ();
			}
		}
	}


	/* Thin wrapper around send packet, 
	   makes logging convenient. Log whenever
	   a packet is sent. */
	void send_message (int fd, message *m){
		auto now = std::chrono::system_clock::now();
   		m->timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
		if (m->type == MSG_CONSENSUS){
			log->AddRowToLogFile (MSG_ENTRY, n, to_string (v),
								  role, n, to_string (v), m->type, m->timestamp);
		} else {
			log->AddRowToLogFile (MSG_ENTRY, n, to_string (v),
								  role, n, "", m->type, m->timestamp);
		}

		send_packet (fd, m);
	}

	void check_consensus () {
		if ((role & LEARNER)&& (role & PROPOSER) &&
			state == PROPOSING) {
			int valid_votes = 0;
			for (int i = 0; i < votes.size (); i++){
				if (votes [i]->v == v && votes [i]->n == n){
					valid_votes++;
				}
			}
			if (valid_votes > num_nodes / 2) {
				message cons (id, -1, v, n, MSG_CONSENSUS);
				network_lock.lock ();
				send_message (0, &cons);
				network_lock.unlock ();
				safe_exit (&cons);
			}
		}
	}

	/* Called when previous votes & acks need to be 
	   cleared. Can happen when preparing or receiving
	   a message with m_n > cur_n */
	void clear_responses (){
		if (role & LEARNER){
			for (int i = 0; i < votes.size (); i++){
				free (votes [i]);
			}
			votes.clear ();
		}
		prepare_acks.clear ();
		state = NO_VALUE;
	}


	/* Increment n and erase previous votes */
	void prepare (){
		if (role & PROPOSER){
			n += 1;
			message m (id, -1, -1, n, MSG_PREPARE);

			clear_responses ();
			network_lock.lock ();
			send_message (0, &m);
			network_lock.unlock ();
			prepare_acks.push_back (id);
			state = PREPARING;
			timeout = time(0);
			simulate_crash ();

		}
	}

	void propose (){

		if ((num_nodes / 2) > prepare_acks.size ()){
			return;
		}

		proposed_v = true;

		state = PROPOSING;

		if (v == NO_VALUE){
			v = rand () % 250;
		}
		vote *vo = (vote *) malloc (sizeof (vote));
		vo->node = id;
		vo->n = n;
		vo->v = v;
		votes.push_back (vo);

		message m (id, -1, v, n, MSG_PROPOSE);
		network_lock.lock ();
		send_message (0, &m);
		network_lock.unlock ();
		timeout  = time(0);
		simulate_crash ();
	}

	void proposer_state_update () {
		if ((role & PROPOSER)){
			if (prepare_acks.size () == 0 && state == NO_VALUE){
				prepare ();
			} else if (votes.size () == 0 && state == PREPARING) {
				propose ();
			}

		}
	}

	void process_message (message *m){
		if (m == NULL){
			return;
		}

		int reponse_code; 
		switch (m->type){
			case MSG_PREPARE: {
				if (m->round > n) {
					n = m->round;
				} 
				state = PREPARING;
				message response (id, m->sender, v, n, MSG_PREPARE_ACK);
				network_lock.lock ();
				send_message (0, &response);
				network_lock.unlock ();
				break; 
			}

			case MSG_PREPARE_ACK: {

				if (m->round > n){
					n = m->round;
					state = NO_VALUE;
					clear_responses ();
				} else if (m->round == n){
					bool already_voted = false;
					for (int i = 0; i < prepare_acks.size (); i++){
						if (prepare_acks [i] == m->sender){
							already_voted = true;
							break;
						}
					}
					if (!already_voted){
						prepare_acks.push_back (m->sender);
						if (v < m->value && !proposed_v){
							v = m->value;
						}
					}

				} else {
					message response (id, m->sender, NO_VALUE, n, MSG_UPDATE);
					network_lock.lock ();
					send_message (0, &response);
					network_lock.unlock ();
					 
				}
				break;
			}

			case MSG_PROPOSE: {
				if (m->round == n && state == PREPARING &&
					((m->value >= v && !proposed_v) || (m->value == v && proposed_v)) ){
					v = m->value;
					state = PROPOSING;
					proposed_v = true;
				} 
				message response (id, m->sender, v, n, MSG_PROPOSE_ACK);
				network_lock.lock ();
				send_message (0, &response);
				network_lock.unlock ();
				break;
			}

			case MSG_PROPOSE_ACK: {
				if (m->round > n){
					n = m->round;
					state = NO_VALUE;
					clear_responses ();
				} else if (m->round < n){
					message response (id, m->sender, NO_VALUE, n, MSG_UPDATE);
					network_lock.lock ();
					send_message (0, &response);
					network_lock.unlock ();

				} else {
					vote *vo = (vote *) malloc (sizeof (vote));
					vo->node = m->sender;
					vo->n = m->round;
					vo->v = m->value;
					votes.push_back (vo);
				} 
				break;
			}

			case MSG_UPDATE: {
				if (m->round > n){
					n = m->round;
					state = NO_VALUE;
					clear_responses ();
				}
				break; 
			}

			case MSG_CONSENSUS: {
				safe_exit (m);
				break;
			}

			case MSG_TEARDOWN: {
				safe_exit (m);
				break;
			}

			default: 
				break;
		}

		free (m);
		if (!(role & PROPOSER)){
			simulate_crash ();
		}
		
	}

public:
	PaxosNode (message *m){
		if (m->type != MSG_INIT) {
			exit (1);
		}
		id = m->sender;
		role = m->receiver;
		num_nodes = m->value;
		n = 0;
		v = NO_VALUE;
		state = NO_VALUE;
		log = new PaxosNodeLogger (id);
	}

	void run (){
		std::thread t ([this]{ this->network_listener(); });
		message *m;
		while (true){
			m = NULL;
			queue_lock.lock ();
			if (!messages.empty ()){
				m = messages.front ();
				messages.pop ();
			}
			queue_lock.unlock ();
			
			process_message (m);
			proposer_state_update ();
			check_consensus ();
			check_timeout ();	
		}

	}
};

int main (){
	/* Listen for initialization message */
	message *m;
	while (true){
		m = (receive_packet (1));
		if (m != NULL){
			break;
		}
	}

	PaxosNode node (m);
	free (m);
	node.run ();
	return 0;
}