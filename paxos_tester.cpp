#include <stdio.h> 
#include <string>
#include <sys/types.h>
#include <unistd.h> 
#include "paxos_controller.cpp"
#include "paxos_constants.h"
#include <memory.h>
#include <ctime>
#include <chrono>

#define ATTEMPTS 25

std::chrono::duration<double> run_paxos (int n, int p){
	std::chrono::duration<double> total;
	for (int i = 0; i < ATTEMPTS; i++) {
		auto start = std::chrono::system_clock::now();
		PaxosController *pc  = new PaxosController(n, p);
		pc->end_simulation ();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end-start;
		pc->clear_memory ();
		delete pc;
		total += diff;
		std::cout << "cur avg "  << (total / (i + 1)).count () << " seconds." << std::endl;
	}

	return (total / ATTEMPTS );
}

std::chrono::duration<double> _default (){
	return run_paxos (DEFAULT_NODES, DEFAULT_PROPOSERS);
}

std::chrono::duration<double> _default_2p (){
	return run_paxos (DEFAULT_NODES, 2);
}

std::chrono::duration<double> _default_3p (){
	return run_paxos (DEFAULT_NODES, 3);
}

void dump_results (std::chrono::duration<double>  avg, string test){
	std::cout << "On test: " << test << ", with " << ATTEMPTS 
	<< " simulations average consensus time was " << avg.count () << " seconds." << std::endl;
}


void node_count_performance (int num){
	dump_results (run_paxos (num, DEFAULT_PROPOSERS), to_string (num) + "n-" + to_string (DEFAULT_PROPOSERS) + "p" );
}

void proposer_count_performance (int num){
	dump_results (run_paxos (15, num), to_string (15) + "n-" + to_string (num) + "p" );
}


int main (int argc, char *argv[]){
	std::cout << atoi (argv[1]) << std::endl;
	node_count_performance (atoi (argv[1]));
	return 0;

}