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
	std::vector<std::vector<node>> scheduledASAP, scheduledALAP, scheduledFDS;
	
	// Check for correct function call
	if (argc != 4) {  
		std::cout << "\nUSAGE: hlsyn cFile Latency verilogFile\n\n";
		return EXIT_FAILURE;
	}

	// Read and parse input file
	if (fileRead(argv[1], &unscheduledIO, &unscheduledNodes) == EXIT_FAILURE) {  
		std::cout << "\nError reading from cFile\n\n";
		return EXIT_FAILURE;
	}

	// Assign ASAP values
	if (!scheduleASAP(std::stoi(argv[2]), &unscheduledNodes, &scheduledASAP)) { 
		std::cout << "ASAP schedule exceeds latency provided. Cannot schedule. Error.\n";
		return EXIT_FAILURE;
	}

	// Reset unscheduled nodes for ALAP
	for(i = 0; (unsigned int)i < unscheduledNodes.size(); i++){ 
		unscheduledNodes.at(i).setScheduled(false);
		unscheduledNodes.at(i).setCyclesElapsed(-1);
	}

	// Assign ALAP values
	if (!scheduleALAP(std::stoi(argv[2]), &unscheduledNodes, &scheduledALAP)) { 
		std::cout << "ALAP failed to schedule all nodes.\n";
		return EXIT_FAILURE;
	}

	// Reset unscheduled nodes for ALAP
	for(i = 0; (unsigned int)i < unscheduledNodes.size(); i++){
		unscheduledNodes.at(i).setScheduled(false);
	}

	// Set up FDS
	for(i = 0; i < std::stoi(argv[2]); i++){
		scheduledFDS.push_back(std::vector<node>());
	}

	// Perform FDS
	if (!FDS(unscheduledNodes.size(),std::stoi(argv[2]), &unscheduledNodes, &scheduledFDS)){
		std::cout << "FDS Error.\n";
		return EXIT_FAILURE;
	}

	/*
	// print FDS
	for(i = 0; (unsigned int)i < scheduledFDS.size(); i++){
		std::cout << "time = " << i << ":\t";
		for(int j = 0; (unsigned int)j < scheduledFDS.at(i).size(); j++){
			std::cout << scheduledFDS.at(i).at(j).getResult() << "\t";
		}
		std::cout << std::endl;
	}
	*/

	// Output verilog file
	if (!generateVerilogFile(unscheduledIO, createStates(scheduledFDS), argv[3])) { 
		std::cout << "Output Error.\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

