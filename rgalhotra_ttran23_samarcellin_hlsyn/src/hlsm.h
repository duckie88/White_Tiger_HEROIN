#ifndef HLSM_H
#define HLSM_H

#include "node.h"

std::vector<std::vector<node>> scheduleASAP(int latency, std::vector<node> unscheduled);
bool scheduleALAP(int latency, std::vector<node>* unscheduledOperation, std::vector<std::vector<node>>* ALAP);

#endif //HLSM_H