#include <iostream>
#include <stdlib.h>
#include <vector>

#include "node.h"
#include "fileIn.h"
#include "operation.h"
#include "conditional.h"


int main(int argc, char* argv[]) {
	std::vector<node> unscheduledIO;
	std::vector<operation> unscheduledOperation;
	std::vector<conditional> unscheduledConditional;
	
	if (argc != 4) {  //check for correct function call
		std::cout << "\nUSAGE: hlsyn cFile Latency verilogFile\n\n";
		return EXIT_FAILURE;
	}

	if (fileRead(argv[1], &unscheduledIO, &unscheduledOperation, &unscheduledConditional) == EXIT_FAILURE) {  //read/parse input file
		std::cout << "\nError reading from cFile\n\n";
		return EXIT_FAILURE;
	}

	//test print of results
	for (unsigned int i = 0; i < unscheduledIO.size(); i++) {
		std::cout << i << "\t" << unscheduledIO.at(i).getName() << "\t" << unscheduledIO.at(i).getType() << "\t" << unscheduledIO.at(i).getDataSize() << "\t" << unscheduledIO.at(i).getSIGN() << std::endl;
	}
	std::cout << "\n\n\n";
	
	std::cout << "Testing to see if all operations were read, ignoring if-elses";
	for (unsigned int i = 0; i < unscheduledOperation.size(); i++) {
		std::cout << unscheduledOperation.at(i).getOutput().getName() << "=" << unscheduledOperation.at(i).getInput1().getName() << unscheduledOperation.at(i).getOperType() << unscheduledOperation.at(i).getInput2().getName() << std::endl;
	}
	//make the graph

	//output verilog


	return EXIT_SUCCESS;
}
