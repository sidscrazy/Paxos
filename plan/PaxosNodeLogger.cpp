// This file contains various classes and their functionality
// This class is used by each node to log its various states. In the end, we should be able to merge all the logs generated from various nodes to understand the replay of the consensus algorithm.
// Q. Should logging happen when the node is in crashed state. Eg only the fact that it received a propose request from a leader.
// Logging is done by this class in form of rows where each row has these values
// PaxosRunInstanceCount: Incremental int. How do we set run instance count? One option is to send one broadcast from leader setting a common instance count for all nodes. This will also return back to the leader about how many nodes are present in first place. But majority should be static and pre-configured value and not decided on the run. TODO
class PaxosNodeLogger{
    // variables
        int NodeRole;           //Proposer, acceptor, learner.Enu
        int MaxPromisedValue;    // Int value representing the maximum value Promised by the current node.In case of proposer, or at init, it is set to - 1.
        int MaxAcceptedValue;    // Maximum value accepted by the node.
        int ConsensusValue;     // Consensus value finalized by proposer or received by Acceptors and Learners
        int NodeState;          // Live, Crashed.When in crashed state, the logged values must conform to situation where node is not communicating.So we log receipt of message but don't send anything.

    // functions
        PaxosNodeLogger(void) {}; // Init function to get a handle to the log file. Do we need to extract previous paxos run count?
        ~PaxosNodeLogger(void) {}; // destructor to closet the file handle

        int OpenFile();          // private to be used by init function to get a handle to the log file
        int CloseLogFile();      // also called by destructor

        int GetRole();           // proposer, accepter, learner, crashed. Crashed state is only for simulation, original algorithm supports the role.
        int SetRole(); //

        // State changes for the node itself logged in next few functions

        int StartPaxosRun(); // Q. Does this happen with promise() message from the leader or before that? Paxos algorithm itself doesn’t allow anything called init, so avoid.
        int EndPaxosRun();

        int PromiseStart(); // Logged when promise message is received from the leader
        int  PromiseEnd(); // after the Promise() message is sent by the client

        int AcceptReqStart();
        int AcceptReqEnd();

        int BroadCastStart(); // By leader only to send message to all that a final consensus has been reached.
        int BroadCastEnd();

        int NodeCrashStart(); // to simulate that the node is crashing. Might not need separate start and end, but still useful.
        int NodeCrashEnd();
}