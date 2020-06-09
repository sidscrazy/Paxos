#ifndef PAXOS_MESSAGE_H
#define PAXOS_MESSAGE_H
#include <memory.h>
#include "paxos_constants.h"

message *receive_packet (int fd){
	message *m = (message *) malloc (sizeof (message));
	if (m == NULL){
		return NULL;
	}

	char buf[1024];
	size_t len = sizeof (message);
	size_t r = recv (fd, buf, len, MSG_DONTWAIT);
	if (r != len){
		free (m);
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