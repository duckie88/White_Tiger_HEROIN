#ifndef FILEIN_H
#define FILEIN_H

#include "variable.h"	// Has <string> and <vector>
#include "node.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

int fileRead(char* fileName, std::vector<variable>* unscheduledIO, std::vector<node>* unscheduledNodes);

bool checkMux(std::vector<std::string> results, std::vector<variable> variables, int* output, int* input1, int* input2, int* input3);

bool checkOperation(std::vector<std::string> results, std::vector<variable> variables, int* output, int* input1, int* input2);

int findDelay(std::string oper);

void newVariableList(int input1, int input2);

void newVariableListMux(int input1, int input2, int input3);

#endif