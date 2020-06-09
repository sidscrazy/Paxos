#include "PaxosNodeLogger.h"

// Constructor will open/create a log file with the guid as ts name, add header to the csv file and keep it open
PaxosNodeLogger::PaxosNodeLogger(int nodeid)
{
    nodeuniqueid = nodeid;
    SetLogFileName(nodeid);
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
void PaxosNodeLogger::SetLogFileName(int nodeid)
{
    logfilename = "nodelog" + to_string(nodeid) + ".csv";
}

// adds header to the logfile of current logger instance.
int PaxosNodeLogger::AddLogFileHeader(void)
{
    if (myfile.is_open()) // should always be true
    {
        // Now add header to the file
        myfile << "timeStamp" << ",";
        myfile << "nodeid" << ",";
        myfile << "receiverid" << ",";
        myfile << "nodeAlive" << ",";
        myfile << "N" << ",";
        myfile << "value" << ",";
        myfile << "nodeRole" << ",";
        myfile << "maxPromisedN" << ",";
        myfile << "consensusValue" << ",";
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

void PaxosNodeLogger::FlushToDisk()
{
    if (myfile.is_open())
    {
        myfile.flush();
    }
}

void PaxosNodeLogger::CloseLogFile()
{
    if (myfile.is_open())
    {
        myfile.close();
    }
}



// Adds one row of data to log file, assume that log file is already open
int PaxosNodeLogger::AddRowToLogFile(int nodeAlive, int N, string value, int nodeRole, int maxPromisedN, string consensusValue, int currentAction, int64_t ts)
{
//    time_t timeStamp = time(0); // will this work on linux?
   
    myfile << "\n"; // get to the new line for next entry.
    myfile << ts        << ",";         // get to the new line for next entry.
    myfile << nodeuniqueid      << ",";         // string: unique guid for the node which owns this log file, same as file name.
    myfile << "NA" << ",";
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

void PaxosNodeLogger::AddMsg (message *m, int role) {
    myfile << "\n"; // get to the new line for next entry.
    myfile << m->timestamp        << ",";         // get to the new line for next entry.
    myfile << m->sender     << ",";         // string: unique guid for the node which owns this log file, same as file name.
    myfile << m->receiver     << ",";         
    myfile << MSG_ENTRY         << ",";         // int: denotes if the node is alive is crashed state.
    myfile << m->round                 << ",";         // int: The value of N proposed or accepted by a node. In say Broadcast action, its value is redundant.
    myfile << m->value             << ",";         // string: Pick value v of highest proposal number -> this is the v we are talking about
    myfile << role          << ",";         // int: Proposer, Acceptor, Learner.
    myfile << m->round     << ",";         // int: Maximum Promised value of N so far for the node. Even leader can have a promised N
    if (m->type == MSG_CONSENSUS){
        myfile << m->value    << ","; 
    } else {
        myfile << ""    << ","; 
    }
    myfile << m->type     << ",";         // int Current action on the node as the logging is happening                              
}

void PaxosNodeLogger::CrashSequence (message *m){
    myfile << "\n"; // get to the new line for next entry.
    myfile << m->timestamp        << ",";         // get to the new line for next entry.
    myfile << m->sender     << ",";         // string: unique guid for the node which owns this log file, same as file name.
    myfile << "-1" << ",";
    myfile << m->value         << ",";         // int: denotes if the node is alive is crashed state.
    myfile << "-1"                  << ",";         // int: The value of N proposed or accepted by a node. In say Broadcast action, its value is redundant.
    myfile << "-1"            << ",";         // string: Pick value v of highest proposal number -> this is the v we are talking about
    myfile << "-1"           << ",";         // int: Proposer, Acceptor, Learner.
    myfile << "-1"      << ",";         // int: Maximum Promised value of N so far for the node. Even leader can have a promised N
    myfile << "-1"     << ","; 
    myfile << "-1"      << ",";         // int Current action on the node as the logging is happening      
}



void PaxosNodeLogger::Crash (){
    auto now = std::chrono::system_clock::now();
    int64_t timeStamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count(); // gets us nanoseconds since epoch
    AddRowToLogFile (CRASH_ENTRY, -1, "", -1, -1, "", -1, timeStamp);
}

void PaxosNodeLogger::ResumeFromCrash (){
    auto now = std::chrono::system_clock::now();
    int64_t timeStamp = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count(); // gets us nanoseconds since epoch
    AddRowToLogFile (RESUME_ENTRY, -1, "", -1, -1, "", -1, timeStamp);
}
