#pragma once
#include <ctime>
#include <fstream>
#include<string>
#include<chrono>

#include "../paxos_constants.h"
using namespace  std;

/*
This file holds the class to manage the log file for each node. This each node will have its own instance of PaxosNodeLogger
and each such instance will point to its own log file.
Just call two functions to use this logger:
PaxosNodeLOgger p1 = new PaxosNodeLogger(guid) // instantiate by passing a guid to constructor
AddRowToLogFile()                              // pass required values to log next row of data in the log file.
                                               // Values and datatypes to pass is understood from function signature.
*/

class PaxosNodeLogger {
    // variables
private:
    ofstream myfile;
    string logfilename;
    int nodeuniqueid;

private:
    void SetLogFileName(int nodeid);
    string GetLogFileName();
    int OpenLogFile(string logfilename);          // private to be used by init function to get a handle to the log file
    int AddLogFileHeader();

public:
    // functions
    PaxosNodeLogger(int nodeguid); // Init function to get a handle to the log file. Do we need to extract previous paxos run count?
    ~PaxosNodeLogger(); // destructor to closet the file handle
    int AddRowToLogFile(int nodeAlive, int N, string value, int nodeRole, int maxPromisedN, string consensusValue, int currentAction, int64_t ts);
    void AddMsg (message *m, int role);
    void CrashSequence (message *m);
    void AddStartup (int id, int role, int nodes);
    void Crash ();
    void ResumeFromCrash ();
    void FlushToDisk();
    void CloseLogFile();
};