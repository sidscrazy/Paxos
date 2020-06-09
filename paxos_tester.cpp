#include <stdio.h> 
#include <string>
#include <sys/types.h>
#include <unistd.h> 
#include "paxos_controller.cpp"
#include "paxos_constants.h"
#include <memory.h>
#include <ctime>
#include <chrono>

#define ATTEMPTS 15

std::chrono::duration<double> run_paxos (int n, int p){
	std::chrono::duration<double> total;
	for (int i = 0; i < ATTEMPTS; i++) {
		auto start = std::chrono::system_clock::now();
		PaxosController pc (n, p);
		pc.end_simulation ();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end-start;
		total += diff;
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


int main (){
	dump_results (_default (), "5n-1p");
	//dump_results (_default_2p (), "5n-2p");
	//dump_results (_default_3p (), "5n-2p");
	return 0;

}