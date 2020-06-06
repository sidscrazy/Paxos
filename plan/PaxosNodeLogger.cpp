#include <ctime>
#include <fstream>
#include<string>
#include <sstream>
#include <random>
#include <string>
using namespace  std;

/*
This file holds the class to manage the log file for each node. This each node will have its own instance of PaxosNodeLogger
and each such instance will point to its own log file.
Just call two functions to use this logger:
PaxosNodeLOgger p1 = new PaxosNodeLogger(guid) // instantiate by passing a guid to constructor
AddRowToLogFile()                              // pass required values to log next row of data in the log file.
                                               // Values and datatypes to pass is understood from function signature.
*/

class PaxosNodeLogger{
    // variables
    private:
        ofstream myfile;
        string logfilename;
        string nodeuniqueid;

    private:
        void SetLogFileName(string nodeguid);
        string GetLogFileName();
        int OpenLogFile(string logfilename);          // private to be used by init function to get a handle to the log file
        int AddLogFileHeader();

    public:
        // functions
        PaxosNodeLogger(string nodeguid); // Init function to get a handle to the log file. Do we need to extract previous paxos run count?
        ~PaxosNodeLogger(); // destructor to closet the file handle
        int AddRowToLogFile(int nodeAlive, int N, string value, int nodeRole, int maxPromisedN, string consensusValue, int currentAction);
        void CloseLogFile();
};

// Constructor will open/create a log file with the guid as ts name, add header to the csv file and keep it open
PaxosNodeLogger::PaxosNodeLogger(string nodeguid)
{
    nodeuniqueid = nodeguid;
    SetLogFileName(nodeguid);
    OpenLogFile(logfilename);
    AddLogFileHeader();
}

// Destructor will just close the file handle to the log file.
PaxosNodeLogger::~PaxosNodeLogger(void)
{
    if(myfile.is_open())
        myfile.close();
}

// Sets the name of the file to be "guid.csv"
void PaxosNodeLogger::SetLogFileName(string nodeguid)
{
    logfilename = nodeguid + ".csv";
}

// adds header to the logfile of current logger instance.
int PaxosNodeLogger::AddLogFileHeader(void)
{
    if (!myfile.is_open()) // should always be true
    {
        // Now add header to the file
        myfile << "uniqueId" << ",";
        //myfile << "currentState" << ","; // ???
        myfile << "N" << ",";
        myfile << "value" << ",";
        myfile << "nodeRole" << ",";
        myfile << "maxPromisedN" << ",";
        myfile << "maxAcceptedN" << ",";
        myfile << "consensusValue" << ",";
        myfile << "nodeAlive" << ",";
        myfile << "currentAction" << ",";
        return 1;
    }
    return 0; // file not open, return that failed to add header to the log file.
}

int PaxosNodeLogger::OpenLogFile(string logfilename)
{
    if (!myfile.is_open())
    {
        myfile.open(logfilename);
    }
    return 1;
}
void PaxosNodeLogger::CloseLogFile()
{
    if (myfile.is_open())
    {
        myfile.close();
    }
}

// Adds one row of data to log file, assume that log file is already open
int PaxosNodeLogger::AddRowToLogFile(int nodeAlive, int N, string value, int nodeRole, int maxPromisedN, string consensusValue, int currentAction)
{
    time_t timeStamp = time(0); // will this work on linux
    myfile << "\n"; // get to the new line for next entry.
    myfile << nodeuniqueid      << ",";         // string: unique guid for the node which owns this log file, same as file name.
    myfile << nodeAlive         << ",";         // int: denotes if the node is alive is crashed state.
    myfile << N                 << ",";         // int: The value of N proposed or accepted by a node. In say Broadcast action, its value is redundant.
    myfile << value             << ",";         // string: Pick value v of highest proposal number -> this is the v we are talking about
    myfile << nodeRole          << ",";         // int: Proposer, Acceptor, Learner.
    myfile << maxPromisedN      << ",";         // int: Maximum Promised value of N so far for the node. Even leader can have a promised N
    myfile << consensusValue    << ",";         // string: Final agreed upon consensus value
    myfile << currentAction     << ",";         // int Current action on the node as the logging is happening 
                                                // proposeStart, proposeEnd,
                                                // promiseStart, promiseEnd,
                                                // acceptReqStart, acceptReqEnd,
                                                // broadcastStart, broadcastEnd,
                                                // nodecrashStart, nodecrashEnd

    return 1;
}

unsigned int  random_char()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    return dis(gen);
}

string generate_hex(const unsigned int len)
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

// create an instance of node logger and add two rows of logs to it
// Not a member of PaxosNodeLogger class, so we have to instantiate the object
int TestPaxosNodeLogger()
{
    string guid = generate_hex(16);
    PaxosNodeLogger *plogger = new PaxosNodeLogger(guid);
    //proposestart
    plogger->AddRowToLogFile(1, 1, NULL, 1, -1, NULL, 1);
    // propose end
    plogger->AddRowToLogFile(1, 1, NULL, 1, -1, NULL, 2);
    plogger->CloseLogFile();
    return 1;
}