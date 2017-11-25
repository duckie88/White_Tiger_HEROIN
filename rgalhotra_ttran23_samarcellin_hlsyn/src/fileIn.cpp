#include "variable.h"
#include "fileIn.h"
#include "node.h"

int fileRead(char* fileName, std::vector<variable>* variables, std::vector<node>* unscheduledNodes) {

	std::istringstream inSS;
	std::ifstream inFS;
	std::string line;
	std::string word;
	std::vector<std::string> results;
	std::string temp;
	node edge;
	unsigned int i = 0;
	unsigned int j = 0;
	int num = 1;
	int input1, input2, input3, output;
	int SIZE = 0;
	int delay = 0;
	bool SIGN = false;
	bool ifFlag = false;
	bool elseFlag = false;

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
						(*variables).push_back(variable(results[0], results[i], SIGN, SIZE));
						(*unscheduledNodes).push_back(node(num));
						num++;
					}
				}
			}
			//end input, output, variables

			// For if-else conditional
			else if (results[0] == "if"){
				ifFlag = true;
			}
			else if(results[0] == "}"){
				ifFlag = false;
			}
			//end if-else
			
			
			
			// For everything else
			else if (results[1] == "=") {
				if(results[3] == "?") {
					if(checkMux(results, *variables, &output, &input1, &input2, &input3)){ //make sure no errors in input
						(*unscheduledNodes).at(output).setOperation(results[3]);
						(*unscheduledNodes).at(output).setResult(results[0]);
						(*unscheduledNodes).at(output).setVar1(results[2]);
						(*unscheduledNodes).at(output).setVar2(results[4]);
						(*unscheduledNodes).at(output).setVar3(results[6]);
						(*unscheduledNodes).at(output).setDelay(findDelay((*unscheduledNodes).at(output).getOperation())); //set delay
						(*unscheduledNodes).at(output).addNextNode(&(*unscheduledNodes).at(output)); //set edges
						(*unscheduledNodes).at(input1).addNextNode(&(*unscheduledNodes).at(output));
						(*unscheduledNodes).at(input2).addNextNode(&(*unscheduledNodes).at(output));
						(*unscheduledNodes).at(input3).addNextNode(&(*unscheduledNodes).at(output));
						(*unscheduledNodes).at(output).addPrevNode(&(*unscheduledNodes).at(input1));
						(*unscheduledNodes).at(output).addPrevNode(&(*unscheduledNodes).at(input2));
						(*unscheduledNodes).at(output).addPrevNode(&(*unscheduledNodes).at(input3));
					}
					else{
						return EXIT_FAILURE;
					}
				}
				else{
					if(checkOperation(results, *variables, &output, &input1, &input2)){
						(*unscheduledNodes).at(output).setOperation(results[3]);
						(*unscheduledNodes).at(output).setResult(results[0]);
						(*unscheduledNodes).at(output).setVar1(results[2]);
						(*unscheduledNodes).at(output).setVar2(results[4]);
						(*unscheduledNodes).at(output).setDelay(findDelay((*unscheduledNodes).at(output).getOperation()));
						(*unscheduledNodes).at(output).addNextNode(&(*unscheduledNodes).at(output));
						(*unscheduledNodes).at(input1).addNextNode(&(*unscheduledNodes).at(output));
						(*unscheduledNodes).at(input2).addNextNode(&(*unscheduledNodes).at(output));
						(*unscheduledNodes).at(output).addPrevNode(&(*unscheduledNodes).at(input1));
						(*unscheduledNodes).at(output).addPrevNode(&(*unscheduledNodes).at(input2));
						//std::cout << (*unscheduledNodes).at(output).getOperation() << (*unscheduledNodes).at(output).getNodeNum()  << "\t" << std::endl; //test indexes returned right
					}
					else{
						return EXIT_FAILURE;
					}
				}
			}
		}
	}
	return EXIT_SUCCESS;
}


bool checkMux(std::vector<std::string> results, std::vector<variable> variables, int* output, int* input1, int* input2, int* input3){
	int i;
	bool check1 = false;
	bool check2 = false;
	bool check3 = false;
	bool check4 = false;

	for(i = 0; i < variables.size(); i++){
		if( results[0] == variables.at(i).getName()){
			check1 = true;
			*output = i;
			break;
		}
	}
	for(i = 0; i < variables.size(); i++){
		if( results[2] == variables.at(i).getName()){
			check2 = true;
			*input1 = i;
			break;
		}
	}
	for(i = 0; i < variables.size(); i++){
		if( results[4] == variables.at(i).getName()){
			check3 = true;
			*input2 = i;
			break;
		}
	}
	for(i = 0; i < variables.size(); i++){
		if( results[6] == variables.at(i).getName()){
			check4 = true;
			*input3 = i;
			break;
		}
	}

	if( check1 == true && check2 == true && check3 == true && check4 == true ){
		return true;
	}

	return false;
}

bool checkOperation(std::vector<std::string> results, std::vector<variable> variables, int* output, int* input1, int* input2){
	int i;
	bool check1 = false;
	bool check2 = false;
	bool check3 = false;

	for(i = 0; i < variables.size(); i++){
		if( results[0] == variables.at(i).getName()){
			*output = i;
			check1 = true;
			break;
		}
	}
	for(i = 0; i < variables.size(); i++){
		if( results[2] == variables.at(i).getName()){
			check2 = true;
			*input1 = i;
			break;
		}
	}
	for(i = 0; i < variables.size(); i++){
		if( results[4] == variables.at(i).getName()){
			check3 = true;
			*input2 = i;
			break;
		}
	}

	if( check1 == true && check2 == true && check3 == true ){
		return true;
	}

	return false;
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