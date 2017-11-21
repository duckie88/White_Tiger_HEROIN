#ifndef HLSM_H
#define HLSM_H

#include "node.h"

bool scheduleASAP(int latency, std::vector<node>* unscheduledOperation, std::vector<std::vector<node>>* ASAP);
bool scheduleALAP(int latency, std::vector<node>* unscheduledOperation, std::vector<std::vector<node>>* ALAP);
bool FDS(int numNodes, std::vector<std::vector<node>>* ASAP,  std::vector<std::vector<node>>* ALAP);

#endif //HLSM_H