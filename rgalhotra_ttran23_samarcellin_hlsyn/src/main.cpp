#include <iostream>
#include <stdlib.h>
#include <vector>

#include "node.h"
#include "fileIn.h"


int main(int argc, char* argv[]) {
	std::vector<node> unscheduled;
	
	if (argc != 4) {  //check for correct function call
		std::cout << "\nUSAGE: hlsyn cFile Latency verilogFile\n\n";
		return EXIT_FAILURE;
	}

	if (fileRead(argv[1], &unscheduled) == EXIT_FAILURE) {  //read/parse input file
		std::cout << "\nError reading from cFile\n\n";
		return EXIT_FAILURE;
	}

	//test print of results
	for (int i = 0; i < unscheduled.size(); i++) {
		std::cout << i << "\t" << unscheduled.at(i).getName() << "\t" << unscheduled.at(i).getType() << "\t" << unscheduled.at(i).getDataSize() << "\t" << unscheduled.at(i).getSIGN() << std::endl;
	}
	std::cout << "\n\n\n";


	//make the graph

	//output verilog


	return EXIT_SUCCESS;
}
