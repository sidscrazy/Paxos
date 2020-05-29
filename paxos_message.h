#ifndef PAXOS_MESSAGE_H
#define PAXOS_MESSAGE_H

struct message {
		int node;
		int value;
		int round;
};

#endif /* PAXOS_MESSAGE_H */