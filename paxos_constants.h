#ifndef PAXOS_CONSTANTS_H
#define PAXOS_CONSTANTS_H



/* Role Constants. */
#define DEFAULT_NODES 5
#define DEFAULT_PROPOSERS 1
#define PROPOSER 1
#define LEARNER  1 << 1
#define ACCEPTOR 1 << 2

/* Message Response Constants. */
#define ACK_ACC 1
#define ACK_REJ 0

/* State Constants. */
#define NO_VALUE -1
#define CONSENSUS -2
#define KILL_PROCESS -3
#define PREPARING -4
#define PROPOSING -5
#define PREPARED -6


/* Uniquely Idenfity the Controller
   in log files. */
#define CONTROLLER_LOG -1


/* Identifiers for Log Entry Types. */
#define INIT_ENTRY 0
#define MSG_ENTRY 1
#define CRASH_ENTRY 2
#define RESUME_ENTRY 3

/* Crash Constants. */
#define TIMEOUT 8
#define CRASH_DUR 2
#define P_CRASH 5
#define C_DIST 2

enum MSG_TYPE {MSG_INIT, MSG_PREPARE, MSG_PREPARE_ACK, 
			   MSG_PROPOSE, MSG_PROPOSE_ACK, MSG_UPDATE,
			   MSG_CONSENSUS, MSG_PING, MSG_TEARDOWN, 
			   MSG_CRASH};

struct message {
	int sender;
	int receiver;
	int value;
	int round;
	MSG_TYPE type;
	int64_t timestamp;

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
#endif /* PAXOS_CONSTANTS_H */