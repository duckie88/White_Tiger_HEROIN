#include "hlsm.h"

/*
Calculates the dependency and gets cycle times for each operation
*/
std::vector<std::vector<node>> scheduleASAP(int latency, std::vector<node> unscheduled) {
	int i = 0;
	int j = 0;
	int k = 0;
	int m = 0;
	std::vector<std::vector<node>> ASAP;
	ASAP.push_back(std::vector<node>());
	for (j = 0; j < unscheduled.size(); j++) {
		ASAP.at(i).push_back(unscheduled.at(j));
		unscheduled.at(j).setAsapTime(i);
		for (k = 1; k < unscheduled.size() - j; k++) {
			for (m = j; m < k; m++) {
				if (unscheduled.at(m).getVarOne() == unscheduled.at(j + k).getVarOne() || unscheduled.at(m).getVarTwo() == unscheduled.at(j + k).getVarTwo()) {
					goto exitcond; //FIGHT ME STEPH
				}
				else {
					ASAP.at(i).push_back(unscheduled.at(j + k));
				}
			}
		}
	exitcond:
		j = j + k - 1;
		i++;
	}
	return ASAP;
}

/*
Calculates the dependency and gets cycle times for each operation
*/
bool scheduleALAP(int latency, std::vector<node>* unscheduled, std::vector<std::vector<node>>* ALAP) {
	return false;
}