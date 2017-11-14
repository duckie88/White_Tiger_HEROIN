#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>

#include "fileIn.h"
#include "node.h"

int fileRead(char* fileName, std::vector<node>* unscheduled) {
	std::istringstream inSS;
	std::ifstream inFS;
	std::string line;
	std::string word;
	std::vector<std::string> results;
	std::string temp;
	int i = 0;
	int j = 0;
	int SIZE = 0;
	bool SIGN = false;

	inFS.open(fileName);//open input file
	if (!inFS.is_open()) { //check opened correctly
		std::cout << "Could not open input file." << std::endl;
		return EXIT_FAILURE;
	}

	while (!inFS.eof()) { //process file
		line.clear();
		getline(inFS, line);
		inSS.clear();
		inSS.str(line);
		if (!line.empty()) { //if line isn't empty (we ignore empty lines)
			results.clear();
			word.clear();
			for (i = 0; i < line.size(); i++) {
				if (line[i] == ' ') {
					results.push_back(word);
					word.clear();
				}
				else {
					word += line[i];
				}
			}
			results.push_back(word);
			word.clear();

			if ((results[0] == "input") || (results[0] == "output") || (results[0] == "variable")) { //get data types and variable names of inputs and outputs
																															   //parse data for node
				temp = results[1];
				if (temp[0] == 'U') {
					SIGN = false; //unsigned 
				}
				else {
					SIGN = true; //signed
				}
				j = 0;
				for (i = 0; i < temp.size(); ++i) { //get dataSize by removing letters from Int32, etc.
					if (isdigit(temp[i])) {
						temp[j] = temp[i];
						j++;
					}
				}
				while (j < temp.size()) {
					temp[j] = '\0';
					j++;
				}
				std::istringstream(temp) >> SIZE;
				for (i = 2; i < results.size(); i++) { //create a node for each variable with these stats
					temp = results[i]; //get rid of commas
					temp.erase(std::remove(temp.begin(), temp.end(), ','), temp.end());
					results[i] = temp;
					if (!(results[i] == " ") && !(results[i] == "")) {
						(*unscheduled).push_back(node(results[0], results[i], SIGN, SIZE));
					}
				}
			}

		}
	}
	return EXIT_SUCCESS;
}