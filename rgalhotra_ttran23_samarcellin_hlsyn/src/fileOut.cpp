#include "fileOut.h"

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
	outFS << "module HLSM" << "(Clk, Rst, Start, Done, ";

	// Putting all input/output variables into a secondary vector
	for (int i = 0; (unsigned int)i < ioList.size(); i++) {
		if (ioList.at(i).getType().compare("input") == 0 || ioList.at(i).getType().compare("output") == 0) {
			ioHeaderList.push_back(ioList.at(i));
		}
	}

	// Traverses secondary vector and just puts the names
	for (int i = 0; (unsigned int)i < ioHeaderList.size() - 1; i++) {
		outFS << ioHeaderList.at(i).getName() << ", ";
	}

	// Does the last variable because all other variables end with ',' while last one ends with ');'
	outFS << ioHeaderList.at(ioHeaderList.size() - 1).getName() << " );\n";
	
	// Generates the list of Input and Outputs
	generateIO(ioList, &outFS);

	// Generates the list of operations
	outFS << "\treg[";
	outFS << ceil(log(states.size() + 2) / log(2));
	outFS << ":0] state;" << std::endl;

	// Generates the list of parameters
	outFS << "\tparameter sWait = 0,";
	for (i = 0; (unsigned int)i < states.size(); i++) {
		outFS << " sTimeSlot" << i + 1 << " = " << i + 1 << ",";
	}
	outFS << " sFinal = " << i + 1 << ";" << std::endl << std::endl;

	// Create the case statements
	outFS << "\talways@(posedge Clk) begin" << std::endl;

	// Reset state
	outFS << "\t\tif(Rst == 1) begin" << std::endl;
	outFS << "\t\t\tstate <= sWait;" << std::endl;
	for (i = 0; (unsigned int)i < ioList.size(); i++) {
		if (ioList.at(i).getType() != "input")
			outFS << "\t\t\t" << ioList.at(i).getName() << " <= 0;" << std::endl;
	}
	outFS << "\t\t\tDone <= 0;" << std::endl;
	outFS << "\t\tend" << std::endl;

	// HLSM
	outFS << "\t\telse begin" << std::endl;
	outFS << "\t\t\tcase(state)" << std::endl;

	// sWait State.
	outFS << "\t\t\tsWait: begin" << std::endl;
	outFS << "\t\t\t\tDone <= 0;" << std::endl;
	outFS << "\t\t\t\tif (Start == 1)" << std::endl;
	outFS << "\t\t\t\tstate <= sTimeSlot1;" << std::endl;
	outFS << "\t\t\t\telse" << std::endl;
	outFS << "\t\t\t\tstate <= sWait;" << std::endl;
	outFS << "\t\t\tend" << std::endl;

	// Generate list of time states
	generateStates(states, &outFS, ioList);

	// sFinal state
	outFS << "\t\t\tsFinal: begin" << std::endl;
	outFS << "\t\t\t\tDone <= 1;" << std::endl;
	outFS << "\t\t\t\tstate <= sWait;" << std::endl;
	outFS << "\t\t\tend" << std::endl;
	outFS << "\t\t\tendcase" << std::endl;
	outFS << "\t\tend" << std::endl;
	outFS << "\tend" << std::endl;

	// Footer and close file
	outFS << "endmodule" << std::endl;
	outFS.close();
	return true;
}

void generateIO(std::vector<variable> list, std::ofstream *outFS) {
	// Variables for string concatenation
	std::string ioTemp;
	
	// Default HLSM variables
	(*outFS) << "\tinput Clk, Rst, Start;" << std::endl;
	(*outFS) << "\toutput reg Done;" << std::endl;

	// Looping through input/output/variables
	for (int i = 0; (unsigned int)i < list.size(); i++) {
		(*outFS) << "\t";
		if (list.at(i).getType() == "output") {
			// If signed value
			if (list.at(i).getSIGN() == true) {
				if (list.at(i).getDataSize() == 1) {
					(*outFS) << list.at(i).getType() << " reg signed " << list.at(i).getName() << ';' << std::endl;
				}
				else {
					(*outFS) << list.at(i).getType() << " reg signed [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
				}
			}
			// If unsigned value
			else {
				if (list.at(i).getDataSize() == 1) {
					(*outFS) << list.at(i).getType() << " reg " << list.at(i).getName() << ';' << std::endl;
				}
				else {
					(*outFS) << list.at(i).getType() << " reg [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
				}
			}
		}
		// Not needed, from a(*outFS)ignment two, but leaving in just in case.
		else if (list.at(i).getType() == "variable") {
			if (list.at(i).getSIGN() == true) {
				if (list.at(i).getDataSize() == 1) {
					(*outFS) << "reg signed " << list.at(i).getName() << ';' << std::endl;
				}
				else {
					(*outFS) << "reg signed [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
				}
			}
			else {
				if (list.at(i).getDataSize() == 1) {
					(*outFS) << "reg " << list.at(i).getName() << ';' << std::endl;
				}
				else {
					(*outFS) << "reg [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
				}
				//std::cout << ioTemp;
			}
		}
		// Everything else
		else {
			if (list.at(i).getSIGN() == true) {
				if (list.at(i).getDataSize() == 1) {
					(*outFS) << list.at(i).getType() << " signed " << list.at(i).getName() << ';' << std::endl;
				}
				else {
					(*outFS) << list.at(i).getType() << " signed [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
				}
				//std::cout << ioTemp;
			}
			else {
				if (list.at(i).getDataSize() == 1) {
					(*outFS) << list.at(i).getType() << " " << list.at(i).getName() << ';' << std::endl;
				}
				else {
					(*outFS) << list.at(i).getType() << " [" << list.at(i).getDataSize() - 1 << ":0] " << list.at(i).getName() << ';' << std::endl;
				}
				//std::cout << ioTemp;
			}
		}
	}
	// End line
	(*outFS) << std::endl;
	//std::cout << std::endl;
}

void generateStates(std::vector<state> states, std::ofstream *outFS, std::vector<variable> ioList) {
	int i = 0;
	int j = 0;
	if (!(*outFS).is_open()) { //check opened correctly
		std::cout << "Could not open output file." << std::endl;
		exit(EXIT_FAILURE);
	}
	for (i = 0; (unsigned int)i < states.size(); i++) {
		// Handle State/Time slots
		(*outFS) << "\t\t\tsTimeSlot";
		(*outFS) << (i + 1);
		(*outFS) << ": begin" << std::endl;
		for (j = 0; (unsigned int)j < states.at(i).getNodes().size(); j++) {
			// Handle Muxes
			if (states.at(i).getNodes().at(j).getOperation() == "?") {
				(*outFS) << "\t\t\t\tif (" << states.at(i).getNodes().at(j).getVar3() << " == 0)" << std::endl << "\t\t\t\t\t" << states.at(i).getNodes().at(j).getResult() << " <= " << states.at(i).getNodes().at(j).getVar1() << ";" << std::endl << "\t\t\t\telse" << std::endl << "\t\t\t\t\t" << states.at(i).getNodes().at(j).getResult() << " <= " << states.at(i).getNodes().at(j).getVar2() << ";" << std::endl;
			}
			// Handle operations
			else {
				(*outFS) << "\t\t\t\t" << states.at(i).getNodes().at(j).getResult() << " <= " << states.at(i).getNodes().at(j).getVar1() << " " << states.at(i).getNodes().at(j).getOperation() << " " << states.at(i).getNodes().at(j).getVar2() << ";" << std::endl;
			}
		}
		// State transition
		(*outFS) << "\t\t\t\tstate <= ";
		if ((unsigned int)i < states.size() - 1) {
			(*outFS) << "sTimeSlot" << (i + 2) << ";" << std::endl;
		}
		else {
			(*outFS) << "sFinal;" << std::endl;
		}
		(*outFS) << "\t\t\tend" << std::endl;
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

