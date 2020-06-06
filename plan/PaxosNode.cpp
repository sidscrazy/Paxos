// This class is used to create an instance of a node. Only for testing as Jackson has already implemented node class.
// Open: Differentiate N and "red" values. Eg Propose(N) and AcceptReq(N, "red") which one is value?
// Open: A leader node must know which all other nodes are live, how to store/query this?

#include <sstream>
#include <random>
#include <string>

using namespace  std;

class PaxosNode
{
    // constructors and destructors
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

    // guid generator
    unsigned int random_char();
    std::string generate_hex(const unsigned int len);
};

unsigned int  PaxosNode::random_char() 
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    return dis(gen);
}

string PaxosNode::generate_hex(const unsigned int len) 
{
    stringstream ss;
    for (auto i = 0; i < len; i++) {
        const auto rc = random_char();
        stringstream hexstream;
        hexstream << hex << rc;
        auto hex = hexstream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }
    return ss.str();
}