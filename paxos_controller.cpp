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

#define NODES 5

class PaxosController {

private:
	std::vector<int> children;
	std::vector<int*> sockets;
	std::vector<std::thread> threads;
	std::vector<std::mutex*> mutexes;
	int nodes;
	void network_simulator (int id){
		return;
	}
public:
	PaxosController (int n){
		nodes = n;
		children.resize (n);
		sockets.resize (n);
		threads.resize (n);
		mutexes.resize (n);

		for (int i = 0; i < nodes; i++){
			
			int *s = (int *) malloc (2 * sizeof (int));
			
			if (s == NULL){
				exit (1);
			}

			if (socketpair(AF_UNIX, SOCK_DGRAM, 0, s) < 0){
				exit (1);
			}

			sockets[i] = s;

			int pid = fork ();
			if (pid == 0 ){
				close (sockets[i][0]);
				dup2 (sockets[i][1], 0);
				dup2 (sockets[i][1], 1);
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
				close (sockets[i][1]);
			}
			threads[i] = std::thread ( [this, i] { this->network_simulator(i); } );
		}

	}


	void end_simulation (){
		int status;
		char buf[1024];
		std::cout << "Controller Exit Begin" << std::endl;
		for (int i = 0; i < nodes ; i++){
			waitpid (children[i], &status, 0);
			read (sockets[i][0], buf, 8);
			buf[8] = '\0';
			std::cout << buf;
			threads[i].join ();
		}
		std::cout << "Controller Exit End" << std::endl;
	}
};




int main (){
	PaxosController pc (NODES);
	pc.end_simulation ();
	return 0;

}