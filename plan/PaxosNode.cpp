// This class is used to create an instance of a node
// Open: Differentiate N and "red" values. Eg Propose(N) and AcceptReq(N, "red") which one is value?
// Open: A leader node must know which all other nodes are live, how to store/query this?

class PaxosNode
{
    // constructors and destructors
    PaxosNode();
    PaxosNode();

    // variables
    int NodeId;     // unique id for this node.
    int role;       // proposer, acceptor, learner
    int MaxPromisedValue;
    int MaxAcceptedValue;
    int ConsensusProposedValue;
    int ConsensusAcceptedValue;
    int NodeState; // live, crashed
    int ProposedValue;



    // functions
    int GetNodeState();
    int SetNodeState();
    int Propose();
    int Promise();
    int AcceptReq();
    int Accept();
    int Broadcast();

};