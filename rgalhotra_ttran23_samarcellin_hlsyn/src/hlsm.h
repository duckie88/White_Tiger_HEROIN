#ifndef HLSM_H
#define HLSM_H

#include "node.h"
#include "fileOut.h"
#include "state.h"

bool scheduleASAP(unsigned int latency, std::vector<node>* unscheduledOperation, std::vector<std::vector<node>>* ASAP);
bool scheduleALAP(unsigned int latency, std::vector<node>* unscheduledOperation, std::vector<std::vector<node>>* ALAP);
bool FDS(int numNodes, int latency, std::vector<node>* nodes,  std::vector<std::vector<node>>* FDS);
std::vector<state> createStates(std::vector<std::vector<node>> FDS);

#endif //HLSM_H