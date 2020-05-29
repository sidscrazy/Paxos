make:
	g++ -std=c++11 -o controller  paxos_controller.cpp -pthread
	g++ -std=c++11   -o node paxos_node.cpp -pthread