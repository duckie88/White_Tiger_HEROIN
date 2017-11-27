#ifndef FILEOUT_H
#define FILEOUT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "variable.h"
#include "state.h"

// Call it as: generateVerilogFile(argv[2]);
bool generateVerilogFile(std::vector<variable> ioList, std::vector<state> states, char* outFileStr);
void generateIO(std::vector<variable> list, char* outFileStr);
std::string generateModule(std::string result, std::string oper1, std::string oper2, std::string type, int num, std::vector<variable>ioList);
std::string generateMux(std::string result, std::string oper1, std::string oper2, std::string oper3, int num, std::vector<variable>ioList);
std::string toString(int n);
std::string toString(double n);
void generateStates(std::vector<state> states, std::ofstream *outFS, std::vector<variable> ioList);
#endif // VERILOGOUTPUT_H