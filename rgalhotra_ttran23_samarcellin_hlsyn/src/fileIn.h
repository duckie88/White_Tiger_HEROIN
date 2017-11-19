#ifndef FILEIN_H
#define FILEIN_H

#include "node.h"	// Has <string> and <vector>
#include "operation.h"
#include "conditional.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

int fileRead(char* fileName, std::vector<node>* unscheduledIO, 
	std::vector<operation>* unscheduledOperation, 
	std::vector<conditional>* unscheduledConditional);

int findDelay(std::string oper);
bool findNode(std::string name, node* currNode, std::vector<node>* list);
#endif