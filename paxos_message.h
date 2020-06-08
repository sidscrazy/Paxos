#ifndef PAXOS_MESSAGE_H
#define PAXOS_MESSAGE_H
#include <memory.h>
#define PROPOSER 1
#define LEARNER  1 << 1
#define ACCEPTOR 1 << 2
#define ACK_ACC 1
#define ACK_REJ 0
#define NO_VALUE -1
#define PREPARING -4
#define CONSENSUS -2
#define KILL_PROCESS -3
#define CONTROLLER_LOG -1
#define ALIVE 1
#define CRASHED 0

enum MSG_TYPE {MSG_INIT, MSG_PREPARE, MSG_PREPARE_ACK, 
			   MSG_PROPOSE, MSG_PROPOSE_ACK, MSG_UPDATE,
			   MSG_CONSENSUS, MSG_PING, MSG_TEARDOWN};

struct message {
	int sender;
	int receiver;
	int value;
	int round;
	MSG_TYPE type;

	message (int s, int recv, int v, int rnd, MSG_TYPE ty){
		sender = s;
		receiver = recv;
		value = v;
		round = rnd;
		type = ty;
	}


};


struct vote {
	int node;
	int n;
	int v;

	vote (int sender, int num, int val){
		node = sender;
		num = n;
		val = v;
	}
};


message *receive_packet (int fd){
	message *m = (message *) malloc (sizeof (message));
	if (m == NULL){
		return NULL;
	}

	char buf[1024];
	size_t len = sizeof (message);
	size_t r = recv (fd, buf, len, MSG_DONTWAIT);
	if (r != len){
		return NULL;
	}

	memcpy (m, buf, sizeof (message));
	return m;


}

void send_packet (int fd, message *m){
	write (fd, (char *)m, sizeof (message));
}



/* For debugging - insert this function to
   test connectivity. */
void ping (int fd){
	message *m = (message *) malloc (sizeof (message));
	if (m == NULL){
		return;
	}
	m->sender = -1;
	m->receiver = -1;
	m->value = NO_VALUE;
	m->round = -1;
	m->type = MSG_PING;
	write (fd, (char *)m, sizeof (message));
	free (m);

}

/* For Debugging - used to print messages
   from the Paxos Controller switch component. */
void dump_message (message *m) {
	std::cout << "Paxos Message Start" << std::endl;
	std::cout << "Sender: " << m->sender << std::endl;
	std::cout << "Receier: " << m->receiver << std::endl;
	std::cout << "Value: " << m->value << std::endl;
	std::cout << "Round: " << m->round << std::endl;
	std::cout << "Type: " << m->type << std::endl;
	std::cout << "Paxos Message End" << std::endl;
}

#endif /* PAXOS_MESSAGE_H */