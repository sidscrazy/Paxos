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
#include "./plan/PaxosNodeLogger.h"
#include <queue>
#include <memory.h>

#define NODES 5

/* The PaxosController class handles initialization
   of PaxosNodes and also simulates packet switching
   between the nodes over a network. The PaxosController
   is not a PaxosNode; it cannot vote on any proposal.

   It is a convenient abstraction for the network and 
   makes it easier to perform interprocess communication
   having a single parent process with sockets to the children.
   We believe that this model is able to simulate paxos without
   loss of generality, since our primary goal of the project
   was to create an effective simulator and not to dig into
   the weeds of interprocess communication between two processes
   that have to search for each other on the same machine.
   Plus, a real world Paxos implementation would likely
   know the other machines that it is communicating with
   and would not have the additional trouble that an
   implementation without this controller needs to deal
   with in the process rather than server model of Paxos.
*/
class PaxosController {

private:
	std::vector<int> children;
	std::vector<int> sockets;
	std::vector<std::thread> threads;
	std::vector<std::mutex> mutexes;
	std::mutex cout_mutex;
	int nodes;
	bool dump_messages = true;
	PaxosNodeLogger *log;

	/* Performs Packet Switching. */
	void network_simulator (int id){

		message *m;
		while (true){
			mutexes[id].lock ();
			m = receive_packet (sockets[id]);
			mutexes[id].unlock ();
			if (m != NULL){
				int receiver = m->receiver;
				
				if (m->type == MSG_TEARDOWN) {
					free (m);
					return;
				}

				/* Broadcast Packet. */
				if (receiver == -1) {
					for (int i = 0; i < nodes; i++){
						if (i != id){
							mutexes[i].lock ();
							m->receiver = i;
							send_packet (sockets[i], m);
							mutexes[i].unlock ();
						}
					}
				/* Send to single recipient. */
				} else {
					mutexes[receiver].lock ();
					send_packet (sockets[receiver], m);
					mutexes[receiver].unlock ();
				}

				m->receiver = receiver;
				cout_mutex.lock ();
				if (dump_messages){
					dump_message (m);
				}
				cout_mutex.unlock ();
				free (m);

			}
		}
	}


public:
	PaxosController (int n){
		nodes = n;
		children = std::vector<int> (n);
		sockets = std::vector<int> (n);
		threads = std::vector<std::thread> (n);
		mutexes = std::vector<std::mutex> (n);
		log = new PaxosNodeLogger (CONTROLLER_LOG);



		for (int i = 0; i < nodes; i++){
			
			int *s = (int *) malloc (2 * sizeof (int));
			
			if (s == NULL){
				exit (1);
			}

			if (socketpair(AF_UNIX, SOCK_DGRAM, 0, s) < 0){
				exit (1);
			}

			sockets[i] = s[0];

			int pid = fork ();
			if (pid == 0 ){
				close (s[0]);
				dup2 (s[1], 0);
				dup2 (s[1], 1);
				char *args[2];
				args[0] = (char *)malloc (7);
				if (args[0] == NULL){
					exit (1);
				}
				memcpy (args[0], "./node", 6);
				args[0][7] = '\0';
				args[1] = NULL;
				execvp (args[0], args);
			} else {
				children[i] = pid;
				close (s[1]);
			}

			int role = ACCEPTOR;
			if (i == 0){
				role |= PROPOSER;
				role |= LEARNER;
			}
			message init_message (i, role, nodes, -1, MSG_INIT);
			mutexes[i].lock ();
			send_packet (sockets[i], &init_message);
			mutexes[i].unlock ();
			free (s);
			threads[i] = std::thread ( [this, i] { this->network_simulator(i); } );
		}

	}


	void end_simulation (){
		int status;
		char buf[1024];
		std::cout << "Controller Exit Begin" << std::endl;
		for (int i = 0; i < nodes ; i++){
			threads[i].join ();
			waitpid (children[i], &status, 0);
		}
		std::cout << "Controller Exit End" << std::endl;
	}
};


int main (){
	PaxosController pc (NODES);
	pc.end_simulation ();
	return 0;

}