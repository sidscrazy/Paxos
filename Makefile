make:
	g++ -std=c++11   -o node paxos_node.cpp  ./plan/PaxosNodeLogger.cpp -pthread
	g++ -std=c++11   -o tester paxos_tester.cpp  ./plan/PaxosNodeLogger.cpp -pthread