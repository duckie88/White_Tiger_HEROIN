#include <iostream>
#include <stdlib.h>
#include <vector>

#include "node.h"
#include "variable.h"
#include "fileIn.h"
#include "fileOut.h"
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

	for(i = 0; (unsigned int)i < unscheduledNodes.size(); i++){ //reset unscheduled nodes for ALAP
		unscheduledNodes.at(i).setScheduled(false);
		unscheduledNodes.at(i).setCyclesElapsed(-1);
		//std::cout << unscheduledNodes.at(i).getResult() << "\t" << unscheduledNodes.at(i).getCyclesElapsed() << std::endl;
	}

	if (!scheduleALAP(std::stoi(argv[2]), &unscheduledNodes, &scheduledALAP)) { //do ALAP
		std::cout << "ALAP failed to schedule all nodes.\n";
		return EXIT_FAILURE;
	}

	for(i = 0; (unsigned int)i < unscheduledNodes.size(); i++){ //reset unscheduled nodes for FDS
		unscheduledNodes.at(i).setScheduled(false);
	}

	//do FDS   FINALLY
	if (!FDS(unscheduledNodes.size(),std::stoi(argv[2]), &unscheduledNodes)){
		std::cout << "FDS Error.\n";
		return EXIT_FAILURE;
	}

	//output verilog
	if (!generateVerilogFile(unscheduledIO, createStates(scheduledASAP), argv[3])) { //wrong arg for createStates, but FDS output not created yet
		std::cout << "Output Error.\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

