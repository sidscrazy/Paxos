#ifndef PAXOS_MESSAGE_H
#define PAXOS_MESSAGE_H

#define PROPOSER 0x1
#define LEARNER  0x1 << 1

enum MSG_TYPE {init, proposal, proposal_ack};

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

#endif /* PAXOS_MESSAGE_H */