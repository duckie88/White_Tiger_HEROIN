#include "fileOut.h"

void generateIO(std::vector<variable> list, char* outFileStr) {
	// Variables for string concatenation
	std::stringstream ss;
	std::string ioTemp;
	// Variables for file, appending
	std::ofstream myFile;
	myFile.open(outFileStr, std::ios::app);
	myFile << std::endl;
	//std::cout << std::endl;
	// Looping through
	for (int i = 0; (unsigned int)i < list.size(); i++) {
		//list[i].getName() << list[i].getType() << list[i].getSIGN() << list[i].getDataSize() 
		ss = std::stringstream();
		if (list.at(i).getSIGN() == true) {
			if (list.at(i).getDataSize() == 1) {
				ss << list.at(i).getType() << " signed " << list.at(i).getName() << ';' << std::endl;
			}
			else {
				ss << list.at(i).getType() << " signed [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
			}
			ioTemp = ss.str();
			//std::cout << ioTemp;
			myFile << ioTemp;
		}
		else {
			if (list.at(i).getDataSize() == 1) {
				ss << list.at(i).getType() << " " << list.at(i).getName() << ';' << std::endl;
			}
			else {
				ss << list.at(i).getType() << " [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
			}
			ioTemp = ss.str();
			//std::cout << ioTemp;
			myFile << ioTemp;
		}
	}

	// End and close
	myFile << std::endl;
	//std::cout << std::endl;
	myFile.close();
}

std::string generateModule(std::string result, std::string oper1, std::string oper2, std::string type, int num, std::vector<variable>ioList) {
	//switch statement based off of type
	//if - or + check if oper2 is 1, then use inc or dec
	//default case to check for error where it's not a real operator
	std::string out;
	int i = 0;
	bool real1 = false;
	bool sign1 = false;
	int dataWidth1 = 0;
	bool real2 = false;
	bool sign2 = false;
	int dataWidth2 = 0;
	bool real3 = false;
	int dataWidth3 = 0;
	int datawidth = 0;
	int indata = 0;

	//checking all variables were correctly declared
	for (i = 0; (unsigned int)i < ioList.size(); i++) {
		if (ioList.at(i).getName() == oper1) {
			real1 = true;
			sign1 = ioList.at(i).getSIGN();
			dataWidth1 = ioList.at(i).getDataSize();
			if (ioList.at(i).getDataSize() > indata) indata = ioList.at(i).getDataSize();
		}
		if (ioList.at(i).getName() == oper2) {
			real2 = true;
			sign2 = ioList.at(i).getSIGN();
			dataWidth2 = ioList.at(i).getDataSize();
			if (ioList.at(i).getDataSize() > indata) indata = ioList.at(i).getDataSize();
		}
		if (ioList.at(i).getName() == result) {
			real3 = true;
			dataWidth3 = ioList.at(i).getDataSize();
			if (ioList.at(i).getDataSize() > datawidth) datawidth = ioList.at(i).getDataSize();
		}
	}

	// Appending/Unappending(?) bits
	if (type != "<<" && type != "==" && type != ">>" && type != ">" && type != "=" && type != "<") {
		if (dataWidth3 < dataWidth1) {	// result is lower bits than input 1
			oper1.append("[" + toString(dataWidth3 - 1) + ":0]");
		}
		else if (dataWidth3 > dataWidth1) {	// Apparently, {4{1'b0},4444} is the same as {00004444}, so the idea is to do {dataWidth{1st_bit} , remaining}
			oper1.insert(0, "{" + toString(dataWidth3 - dataWidth1) + "{" + oper1 + "[" + toString(dataWidth1 - 1) + "]},");
			oper1.append("}");
		}

		if (dataWidth3 < dataWidth2 && oper2 != "1") {	// result is lower bits than input 1
			oper2 = oper2.append('[' + toString(dataWidth3 - 1) + ":0]");
		}
		else if (dataWidth3 > dataWidth2 && oper2 != "1") {	// Apparently, {4{1'b0},4444} is the same as {00004444}, so the idea is to do {dataWidth{1st_bit} , remaining}
			oper2.insert(0, "{" + toString(dataWidth3 - dataWidth2) + "{" + oper2 + "[" + toString(dataWidth2 - 1) + "]},");
			oper2.append("}");
		}
	}
	else {
		if (dataWidth2 > dataWidth1) {	// Apparently, {4{1'b0},4444} is the same as {00004444}, so the idea is to do {dataWidth{1st_bit} , remaining}
			oper1.insert(0, "{" + toString(dataWidth2 - dataWidth1) + "{" + oper1 + "[" + toString(dataWidth1 - 1) + "]},");
			oper1.append("}");
		}
		else if (dataWidth1 > dataWidth2 && oper2 != "1") {	// Apparently, {4{1'b0},4444} is the same as {00004444}, so the idea is to do {dataWidth{1st_bit} , remaining}
			oper2.insert(0, "{" + toString(dataWidth1 - dataWidth2) + "{" + oper2 + "[" + toString(dataWidth2 - 1) + "]},");
			oper2.append("}");
		}
	}

	// Outputting to verilog file
	if (!(real1 || oper1 == "1") || !(real2 || type == "reg" || oper2 == "1") || !real3) return "error"; //error case 1-3

	if (type == "+") {
		if (oper2 == "1") out = "INC #(.DATAWIDTH(" + toString(datawidth) + ")) incrementor" + toString(num) + "(" + oper1 + "," + result + ");";
		else out = "ADD #(.DATAWIDTH(" + toString(datawidth) + ")) adder" + toString(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "-") {
		if (oper2 == "1") out = "DEC #(.DATAWIDTH(" + toString(datawidth) + ")) decrementor" + toString(num) + "(" + oper1 + "," + result + ");";
		else out = "SUB #(.DATAWIDTH(" + toString(datawidth) + ")) subber" + toString(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "reg") {
		out = "REG #(.DATAWIDTH(" + toString(datawidth) + ")) register" + toString(num) + "(Clk,Rst" + "," + oper1 + "," + result + ");";
	}
	else if (type == "*") {
		out = "MUL #(.DATAWIDTH(" + toString(datawidth) + ")) multiplier" + toString(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "==") {
		out = "COMP #(.DATAWIDTH(" + toString(indata) + ")) comparator" + toString(num) + "(" + oper1 + "," + oper2 + ", , ," + result + ");";
	}
	else if (type == ">") {
		out = "COMP #(.DATAWIDTH(" + toString(indata) + ")) comparator" + toString(num) + "(" + oper1 + "," + oper2 + "," + result + ", , );";
	}
	else if (type == "<") {
		out = "COMP #(.DATAWIDTH(" + toString(indata) + ")) comparator" + toString(num) + "(" + oper1 + "," + oper2 + ", ," + result + ", );";
	}
	else if (type == "<<") {
		out = "SHL #(.DATAWIDTH(" + toString(datawidth) + ")) shiftleft" + toString(num) + "(" + oper2 + "," + oper1 + "," + result + ");";
	}
	else if (type == ">>") {
		out = "SHR #(.DATAWIDTH(" + toString(datawidth) + ")) shiftright" + toString(num) + "(" + oper2 + "," + oper1 + "," + result + ");";
	}
	else if (type == "/") {
		out = "DIV #(.DATAWIDTH(" + toString(datawidth) + ")) divider" + toString(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else if (type == "%") {
		out = "MOD #(.DATAWIDTH(" + toString(datawidth) + ")) modulus" + toString(num) + "(" + oper1 + "," + oper2 + "," + result + ");";
	}
	else {
		return "error"; //error case 4
	}
	if ((sign1 || sign2) && !(type == "reg" || type == "<<" || type == ">>")) return "S" + out;
	else return out;
}

std::string generateMux(std::string result, std::string oper1, std::string oper2, std::string oper3, int num, std::vector<variable>ioList) {
	int i = 0;
	bool real1 = false;
	bool real2 = false;
	bool real3 = false;
	int datawidth = 0;

	//checking all variables were declared
	for (i = 0; (unsigned int)i < ioList.size(); i++) {
		if (ioList.at(i).getName() == oper1) {
			real1 = true;
		}
		if (ioList.at(i).getName() == oper2) {
			real2 = true;
		}
		if (ioList.at(i).getName() == result) {
			real3 = true;
			if (ioList.at(i).getDataSize() > datawidth) datawidth = ioList.at(i).getDataSize();
		}
	}
	if (!real1 || !real2 || !real3) return "error"; //error case 1-3

	return "MUX2x1 #(.DATAWIDTH(" + toString(datawidth) + ")) mux" + toString(num) + "(" + oper1 + "," + oper2 + "," + oper3 + "," + result + ");";
}

bool generateVerilogFile(std::vector<variable> ioList, std::vector<state> states, char* outFileStr) {

	std::ofstream outFS; // Open file to append/write to it.
	std::vector<variable> ioHeaderList;	// For having a smaller list of input/output for just the header module(___); part. Needed because main ioList will also include wire/regs
	int i = 0;

	outFS.open(outFileStr);//open output file
	if (!outFS.is_open()) { //check opened correctly
		std::cout << "Could not open output file." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Header
	outFS << "`timescale 1ns / 1ps" << std::endl;
	outFS << "module HLSM"  << "(Clk, Rst, Start, Done, ";

	// Putting all input/output variables into a secondary vector
	for (int i = 0; (unsigned int)i < ioList.size(); i++) {
		if (ioList.at(i).getType().compare("input") == 0 || ioList.at(i).getType().compare("output") == 0) {
			ioHeaderList.push_back(ioList.at(i));
		}
	}

	// Traverses secondary vector and just puts the names
	for (int i = 0; (unsigned int)i < ioHeaderList.size() - 1; i++) {
		outFS << ioHeaderList.at(i).getName() << ", ";
		//std::cout << ioHeaderList.at(i).getName() << ", ";
	}

	// Does the last variable because all other variables end with ',' while last one ends with ');'
	outFS << ioHeaderList.at(ioHeaderList.size() - 1).getName() << " );\n";
	//std::cout << ioHeaderList.at(ioHeaderList.size() - 1).getName() << " );\n";

	// Generates the list of Input and Outputs
	generateIO(ioList, outFileStr);

	// Generates the list of operations
	outFS << "reg[";
	outFS << ceil(log(states.size() + 2)/log(2));
	outFS << ":0] state;" << std::endl;

	// Generates the list of parameters
	outFS << "parameter sWait = 0,";
	for (i = 0; (unsigned int)i < states.size(); i++) {
		outFS << " s" << i + 2 << " = " << i + 1 << ",";
	}
	outFS << " sFinal = " << i + 1 << ";" << std::endl << std::endl;

	// Create the case statements.
	outFS << "always@(posedge Clk) begin" << std::endl;

	// Reset state.
	outFS << "if(Rst == 1) begin" << std::endl;
	outFS << "state <= sWait;" << std::endl;
	for (i = 0; (unsigned int)i < ioList.size(); i++) {
		outFS << ioList.at(i).getName() << " <= 0;" << std::endl;
	}
	//for (i = 0; (unsigned int)i < ioList.size(); i++) {
	//	if(ioList.at(i).getType() == "output")
	//		outFS << ioList.at(i).getName() << " <= 0;" << std::endl;
	//}
	outFS << "Done <= 0;" << std::endl;
	outFS << "end" << std::endl;

	//HLSM
	outFS << "else begin" << std::endl;
	outFS << "case(state)" << std::endl;

	// sWait State.
	outFS << "sWait: begin" << std::endl;
	outFS << "Done <= 0;" << std::endl;
	outFS << "if (Start == 1)" << std::endl;
	outFS << "state <= s2;" << std::endl;
	outFS << "else" << std::endl;
	outFS << "state <= sWait;" << std::endl;
	outFS << "end" << std::endl;

	// Generate list of states
	generateStates(states, &outFS, ioList);

	//last state
	outFS << "sFinal: begin" << std::endl;
	outFS << "Done <= 1;" << std::endl;
	outFS << "state <= sWait;" << std::endl;
	outFS << "end" << std::endl;
	outFS << "endcase" << std::endl;
	outFS << "end" << std::endl;
	outFS << "end" << std::endl;


	// Footer
	outFS << std::endl << "endmodule" << std::endl;
	//std::cout << std::endl << "endmodule" << std::endl;
	outFS.close();
	return true;
}

void generateStates(std::vector<state> states, std::ofstream *outFS, std::vector<variable> ioList) {
	int i = 0;
	int j = 0;
	if (!(*outFS).is_open()) { //check opened correctly
		std::cout << "Could not open output file." << std::endl;
		exit(EXIT_FAILURE);
	}
	for (i = 0; (unsigned int)i < states.size(); i++) {
		(*outFS) << "s";
		(*outFS) << (i + 2);
		(*outFS) << ": begin" << std::endl;
		for (j = 0; (unsigned int)j < states.at(i).getNodes().size(); j++) {
			
			if (states.at(i).getNodes().at(j).getOperation() == "?")
				(*outFS) << "if (" << states.at(i).getNodes().at(j).getVar3() << " ==  0)" << std::endl  << "\t" << states.at(i).getNodes().at(j).getResult() << " <= " << states.at(i).getNodes().at(j).getVar1() << ";" << std::endl << "else" << std::endl << "\t" << states.at(i).getNodes().at(j).getResult() << " <= " << states.at(i).getNodes().at(j).getVar2() << ";" << std::endl;
				//(*outFS) << generateMux(states.at(i).getNodes().at(j).getResult(), states.at(i).getNodes().at(j).getVar1(), states.at(i).getNodes().at(j).getVar2(), states.at(i).getNodes().at(j).getVar3(), (i + j), ioList) << std::endl;
			else
				(*outFS) << states.at(i).getNodes().at(j).getResult() << " <= " << states.at(i).getNodes().at(j).getVar1() << " " << states.at(i).getNodes().at(j).getOperation() << " " << states.at(i).getNodes().at(j).getVar2() << ";" << std::endl;
				//(*outFS) << generateModule(states.at(i).getNodes().at(j).getResult(), states.at(i).getNodes().at(j).getVar1(), states.at(i).getNodes().at(j).getVar2(), states.at(i).getNodes().at(j).getOperation(), (i + j), ioList) << std::endl;
			(*outFS) << "state <= ";
			
			if ((unsigned int)i < states.size() - 1) {
				(*outFS) << "s" << (i + 3) << ";" << std::endl;
			}
			else {
				(*outFS) << "sFinal;" << std::endl;
			}
			(*outFS) << "end" << std::endl;
		}
	}
}

std::string toString(int n) {
	std::ostringstream stm;
	stm << n;
	return stm.str();
}

std::string toString(double n) {
	std::ostringstream stm;
	stm << n;
	return stm.str();
}