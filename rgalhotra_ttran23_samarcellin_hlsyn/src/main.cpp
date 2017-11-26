#include <iostream>
#include <stdlib.h>
#include <vector>

#include "node.h"
#include "variable.h"
#include "fileIn.h"
#include "hlsm.h"

int main(int argc, char* argv[]) {
	int i;
	std::vector<variable> unscheduledIO;
	std::vector<node> unscheduledNodes;
	std::vector<state> states;
	std::vector<std::vector<node>> scheduledASAP, scheduledALAP;
	

	if (argc != 4) {  //check for correct function call
		std::cout << "\nUSAGE: hlsyn cFile Latency verilogFile\n\n";
		return EXIT_FAILURE;
	}

	if (fileRead(argv[1], &unscheduledIO, &unscheduledNodes) == EXIT_FAILURE) {  //read/parse input file
		std::cout << "\nError reading from cFile\n\n";
		return EXIT_FAILURE;
	}

	//make the graph
	if (!scheduleASAP(std::stoi(argv[2]), &unscheduledNodes, &scheduledASAP)) { //do ASAP
		std::cout << "ASAP schedule exceeds latency provided. Cannot schedule. Error.\n";
		return EXIT_FAILURE;
	}

	for(i = 0; i < unscheduledNodes.size(); i++){ //reset unscheduled nodes for ALAP
		unscheduledNodes.at(i).setScheduled(false);
	}

	if (!scheduleALAP(std::stoi(argv[2]), &unscheduledNodes, &scheduledALAP)) { //do ALAP
		std::cout << "ALAP failed to schedule all nodes.\n";
		return EXIT_FAILURE;
	}

	//do FDS   FINALLY
	if (!FDS(unscheduledNodes.size(), &scheduledASAP,  &scheduledALAP)){
		std::cout << "FDS Error.\n";
		return EXIT_FAILURE;
	}

	//test print of results
	//for (unsigned int i = 0; i < unscheduledNodes.size(); i++) {
	//	std::cout << unscheduledNodes.at(i).getNodeNum() << "\t" << unscheduledNodes.at(i).getOperation() << std::endl;
	//}
	//std::cout << "\n\n\n";



	//output verilog #need to know how we're handling FDS output
	if (!generateVerilogFile(unscheduledIO, states, argv[3])) {
		std::cout << "Output Error.\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

