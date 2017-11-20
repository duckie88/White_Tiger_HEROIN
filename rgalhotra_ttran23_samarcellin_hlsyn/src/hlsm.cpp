#include "hlsm.h"

/*
Calculates the dependency and gets cycle times for each operation
*/
bool scheduleASAP(int latency, std::vector<node>* unscheduled, std::vector<std::vector<node>>* ASAP) {
	int scheduled = 0;
	int i = 0;
	int j,k,m;
	bool unschedPrevNode = false;

	while(scheduled < (*unscheduled).size()){
		(*ASAP).push_back(std::vector<node>());

		for (j = 0; j < (*unscheduled).size(); ++j) {
			unschedPrevNode = false;
			if (i == 0) {
				//if there are no previous nodes
				if ((*unscheduled).at(j).getPrevNodes().size() == 0) {
					(*ASAP).at(i).push_back((*unscheduled).at(j));
					(*unscheduled).at(j).setAsapTime(i);
					scheduled++;
					
					//Need to account for the delay if there is one
					//FIXME~!
					for (k = 0; k < (*unscheduled).at(j).getNextNodes().size(); k++) {
						if (i + (*unscheduled).at(j).getDelay() > (*unscheduled).at(j)->getNextNodes().at(k)->getCyclesElapsed()) {
							(*unscheduled).at(j)->getNextNodes().at(k)->setCyclesElapsed(i + (*unscheduled).at(j).getDelay());
						}
					}
				}
			}
		else {
				//schedule node if allowed cycle is equal to i
				for (m = 0; m < (*unscheduled).at(j)->getPreviousNodes().size(); m++) {
					if ((*unscheduled).at(j)->getPreviousNodes().at(m)->getAsapTime() == -1) {
						unschedPrevNode = true;
					}
				}
				if ((*unscheduled).at(j)->getCycleAllowed() == i && !unschedPrevNode) {
					(*ASAP).at(i).push_back((*unscheduled).at(j));
					(*unscheduled).at(j)->setAsapTime(i);
					scheduled++;
					//update nodes allowed cycle time
					for (k = 0; k < (*unscheduled).at(j)->getNextNodes().size(); ++k) {
						if (i + (*unscheduled).at(j)->getDelay() > (*unscheduled).at(j)->getNextNodes().at(k)->getCycleAllowed()) {
							(*unscheduled).at(j)->getNextNodes().at(k)->setCycleAllowed(i + (*unscheduled).at(j)->getDelay());
						}
					}
					if ((*unscheduled).at(j)->getConditional()) {
						for (k = 0; k < (int)(*unscheduled).at(j)->getNextIfNodes().size(); ++k) {
							if (i + (*unscheduled).at(j)->getDelay() > (*unscheduled).at(j)->getNextIfNodes().at(k)->getCycleAllowed()) {
								(*unscheduled).at(j)->getNextIfNodes().at(k)->setCycleAllowed(i + (*unscheduled).at(j)->getDelay());
							}
						}
						for (k = 0; k < (int)(*unscheduled).at(j)->getNextElseNodes().size(); ++k) {
							if (i + (*unscheduled).at(j)->getDelay() > (*unscheduled).at(j)->getNextElseNodes().at(k)->getCycleAllowed()) {
								(*unscheduled).at(j)->getNextElseNodes().at(k)->setCycleAllowed(i + (*unscheduled).at(j)->getDelay());
							}
						}
					}
				}
			}
		}
		i++;
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
bool scheduleALAP(int latency, std::vector<node>* unscheduled, std::vector<std::vector<node>>* ALAP) {
	return false;
}