/* Role Constants. */
#define PROPOSER 1
#define LEARNER  1 << 1
#define ACCEPTOR 1 << 2

/* Message Response Constants. */
#define ACK_ACC 1
#define ACK_REJ 0

/* State Constants. */
#define NO_VALUE -1
#define PREPARING -4
#define CONSENSUS -2
#define KILL_PROCESS -3

/* Uniquely Idenfity the Controller
   in log files. */
#define CONTROLLER_LOG -1

/* Crash Constants. */
#define ALIVE 1
#define CRASHED -1
#define RESUME 0
#define TIMEOUT 2
#define P_CRASH 3
#define C_DIST 15