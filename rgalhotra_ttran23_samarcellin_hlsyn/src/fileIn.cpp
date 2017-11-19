#include "node.h"
#include "fileIn.h"

int fileRead(char* fileName, std::vector<node>* unscheduledIO, 
					std::vector<operation>* unscheduledOperation, 
					std::vector<conditional>* unscheduledConditional) {

	std::istringstream inSS;
	std::ifstream inFS;
	std::string line;
	std::string word;
	std::vector<std::string> results;
	std::string temp;
	unsigned int i = 0;
	unsigned int j = 0;
	int SIZE = 0;
	bool SIGN = false;
	int lineNumber = 0;		// For tracking when conditionals start/end
	bool cond = false; //to skip conditional lines

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

			// Splits line into a string vector "results"
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

			// If a variable, adds line as a node
			if ((results[0] == "input") || (results[0] == "output") || (results[0] == "variable")) { //get data types and variable names of inputs and outputs. Parse data for node
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
						(*unscheduledIO).push_back(node(results[0], results[i], SIGN, SIZE));
					}
				}
			}
			// For if-else conditional
			else if ((results[0] == "if") || (results[0] == "else")) {
				(*unscheduledConditional).push_back(conditional(results[0], lineNumber, results[2]));
				cond = true;
			}
			// For the ending parenthesis }
			else if (results[0] == "}") {
				(*unscheduledConditional).push_back(conditional(results[0], lineNumber, ""));
				cond = false;
			}
			// For everything else
			else if (results[1] == "=") {
				if (0) {	// If mux
					printf("temp");
				}
				else {
					results[0].erase(std::remove(results[0].begin(), results[0].end(), '\t'), results[0].end());
					node tempOut, tempIn1, tempIn2;
					int tempDelay = findDelay(results[3]);
					// Check that the three nodes are in the list
					if (!findNode(results[0], &tempOut, unscheduledIO) || !findNode(results[2], &tempIn1, unscheduledIO) || !findNode(results[4], &tempIn2, unscheduledIO)) {
						std::cout << "Error, cannot find variable used on line " << lineNumber << " of the netlist.\n";
						return EXIT_FAILURE;
					}
					
					(*unscheduledOperation).push_back(operation(lineNumber, results[3], tempOut, tempIn1, tempIn2, node(), tempDelay, cond));
				}
			}
		}
		lineNumber++;	// Tracking how many lines we have, used for conditional tracking
	}
	return EXIT_SUCCESS;
}

/*
Get the delay from the operations as per pdf
"Multipliers have a 2 cycle delay, divider/modulo have a 3 cycle delay, all other resources have a 1 cycle delay"

*/
int findDelay(std::string oper) {
	if (oper == "/" || oper == "%") {
		return 3;
	}
	else if (oper == "*") {
		return 2;
	}
	else {	
		// Should probably check for all other operations, in case there's a random & as an operation or something but the 
		// error cases are only for missing vars so eh. Just food for thought.
		return 1;
	}
}


/*
Searches for the node inside the unscheduled list
*/
bool findNode(std::string name, node* currNode, std::vector<node>* list) {
	for (unsigned int i = 0; i < list->size(); i++) {
		if (list->at(i).getName() == name) {
			*currNode = list->at(i);
			return true;
		}
	}
	return false;
}