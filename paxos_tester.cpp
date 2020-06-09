#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include "paxos_controller.cpp"
#include "paxos_constants.h"
#include <memory.h>
#include <ctime>


int main (){

	PaxosController pc (DEFAULT_NODES, DEFAULT_PROPOSERS);
	pc.end_simulation ();
	return 0;

}