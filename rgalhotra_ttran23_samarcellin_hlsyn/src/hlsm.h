#ifndef HLSM_H
#define HLSM_H

#include "node.h"
#include "operation.h"
#include "conditional.h"

bool scheduleASAP(int latency, std::vector<operation>* unscheduledOperation, std::vector<conditional>* unscheduledConditional, std::vector<std::vector<operation>>* ASAP);
bool scheduleALAP(int latency, std::vector<operation>* unscheduledOperation, std::vector<conditional>* unscheduledConditional, std::vector<std::vector<operation>>* ALAP);

#endif //HLSM_H