#include "hlsm.h"

/*
Calculates the dependency and gets cycle times for each operation
*/
bool scheduleASAP(int latency, std::vector<operation>* unscheduledOperation, std::vector<conditional>* unscheduledConditional, std::vector<std::vector<operation>>* ASAP) {
	std::vector<operation> unschedASAP = *unscheduledOperation;
	unsigned int i = 0, j = 0, k = 0, t = 0;
	bool found = false;
	(*ASAP).push_back(std::vector<operation>());
	// Setting initial predecessor nodes sched'd
	for (i = 0; i < unscheduledOperation->size(); i++) {
		found = false;
		for (j = 0; j < unschedASAP.size(); j++) {
			if ((*unscheduledOperation).at(i).getInput1().getName() == unschedASAP.at(j).getOutput().getName()
				|| (*unscheduledOperation).at(i).getInput2().getName() == unschedASAP.at(j).getOutput().getName()) {
				found = true;
				break;
			}
		}
		unscheduledOperation->at(i).setSchedState(false);
		if (!found) {
			unscheduledOperation->at(i).setSchedState(true);
			(*ASAP).at(0).push_back((*unscheduledOperation).at(i));
		}
	}

	// Starting now from time 2 (in code, it's t=1 since it starts at 0) and onwards.
	t=1;
	while (t < unscheduledOperation->size() - ASAP->at(0).size()) {	// Iterate through the rest of operations
		(*ASAP).push_back(std::vector<operation>());
		for (i = 0; i < unscheduledOperation->size(); i++) {

		}
		t++;
	}
	// Checking if latency is enough
	if ((*ASAP).size() > latency) {
		return false;
	}
	return true;
}

/*
Calculates the dependency and gets cycle times for each operation
*/
bool scheduleALAP(int latency, std::vector<operation>* unscheduledOperation, std::vector<conditional>* unscheduledConditional, std::vector<std::vector<operation>>* ALAP) {
	return false;
}