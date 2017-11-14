#ifndef FILEIN_H
#define FILEIN_H

#include <vector>
#include "node.h"

int fileRead(char* fileName, std::vector<node>* unscheduledList);

#endif