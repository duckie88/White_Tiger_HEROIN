#ifndef FILEOUT_H
#define FILEOUT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "node.h"

// Call it as: generateVerilogFile(argv[2]);
void generateVerilogFile(std::vector<node> ioList, std::vector<std::string> moduleList, char* inFileStr, char* outFileStr);
void generateIO(std::vector<node> list, char* outFileStr);
std::string generateModule(std::string result, std::string oper1, std::string oper2, std::string type, int num, std::vector<node>ioList);
std::string generateMux(std::string result, std::string oper1, std::string oper2, std::string oper3, int num, std::vector<node>ioList);
std::string toString(int n);
std::string toString(double n);
#endif // VERILOGOUTPUT_H