#include <stdio.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <vector>
#include <sys/wait.h>
#include <iostream>
#include <thread>
#include <mutex>
#include "paxos_message.h"
#include <queue>

class PaxosNode {
private:
	int id;
	int num_nodes;
	int role;
	int vote;
	int round;
	int num_consensus;
	std::mutex network_lock;
	std::mutex queue_lock;
	std::queue<message *> messages;

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

public:
	PaxosNode (message *m){
		if (m->type != init) {
			exit (1);
		}
		id = m->sender;
		role = m->receiver;
		num_nodes = m->value;
	}

	void run (){
		std::thread ([this]{ this->network_listener(); });
		message *cur;
		while (true){
			cur = NULL;
			queue_lock.lock ();
			if (!messages.empty ()){
				cur = messages.front ();
				messages.pop ();
			}
			queue_lock.unlock ();
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