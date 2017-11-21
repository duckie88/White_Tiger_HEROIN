#include <iostream>
#include <stdlib.h>
#include <vector>

#include "node.h"
#include "variable.h"
#include "fileIn.h"
#include "hlsm.h"

int main(int argc, char* argv[]) {
	std::vector<variable> unscheduledIO;
	std::vector<node> unscheduledNodes;
	std::vector<std::vector<node>> scheduledASAP;

	if (argc != 4) {  //check for correct function call
		std::cout << "\nUSAGE: hlsyn cFile Latency verilogFile\n\n";
		return EXIT_FAILURE;
	}

	if (fileRead(argv[1], &unscheduledIO, &unscheduledNodes) == EXIT_FAILURE) {  //read/parse input file
		std::cout << "\nError reading from cFile\n\n";
		return EXIT_FAILURE;
	}

	//test print of results
	for (unsigned int i = 0; i < unscheduledNodes.size(); i++) {
		std::cout << unscheduledNodes.at(i).getNodeNum() << "\t" << unscheduledNodes.at(i).getOperation() << std::endl;
	}
	std::cout << "\n\n\n";

	//make the graph
	if (scheduleASAP(std::stoi(argv[2]), unscheduledNodes).size() > 0) {
		std::cout << scheduleASAP(std::stoi(argv[2]), unscheduledNodes).size();
		return EXIT_FAILURE;
	}


	//output verilog


	return EXIT_SUCCESS;
}