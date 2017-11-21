#include "hlsm.h"

/*
Calculates the dependency and gets cycle times for each operation
*/
bool scheduleASAP(int latency, std::vector<node>* unscheduled, std::vector<std::vector<node>>* ASAP) {
	int scheduled = 0;
	int i = 0;
	int j,k,m;
	bool unschedPrevNode = false;
	std::vector<node*> temp;

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

					//deal with the delay (if exists)
					for (k = 0; k < (*unscheduled).at(j).getNextNodes().size(); k++) {
						if (i + (*unscheduled).at(j).getDelay() >= (*unscheduled).at(j).getNextNodes().at(k)->getCyclesElapsed()) {
							(*unscheduled).at(j).getNextNodes().at(k)->setCyclesElapsed(i + (*unscheduled).at(j).getDelay() + 1);
						}
					}
				}
			}
			else {
				//schedule node if cycle is i
				temp = (*unscheduled).at(j).getPrevNodes();
				for (m = 0; m < temp.size(); m++) {
					if (temp.at(m)->getAsapTime() == -1) {
						unschedPrevNode = true;
					}
				}
				if ((*unscheduled).at(j).getCyclesElapsed() == i && !unschedPrevNode) {
					(*ASAP).at(i).push_back((*unscheduled).at(j));
					(*unscheduled).at(j).setAsapTime(i);
					scheduled++;
					//update node cycle time
					for (k = 1; k < (*unscheduled).at(j).getNextNodes().size(); ++k) {
						if (i + (*unscheduled).at(j).getDelay() >(*unscheduled).at(j).getNextNodes().at(k)->getCyclesElapsed()) {
							(*unscheduled).at(j).getNextNodes().at(k)->setCyclesElapsed(i + (*unscheduled).at(j).getDelay());
						}
					}
					if ((*unscheduled).at(j).getConditional()) {
						temp = (*unscheduled).at(j).getNextIfNodes();
						for (k = 0; k < (*unscheduled).at(j).getNextIfNodes().size(); ++k) {
							if (i + (*unscheduled).at(j).getDelay() > temp.at(k)->getCyclesElapsed()) {
								temp.at(k)->setCyclesElapsed(i + (*unscheduled).at(j).getDelay());
							}
						}

						temp = (*unscheduled).at(j).getNextElseNodes();
						for (k = 0; k < temp.size(); ++k) {
							if (i + (*unscheduled).at(j).getDelay() > temp.at(k)->getCyclesElapsed()) {
								temp.at(k)->setCyclesElapsed(i + (*unscheduled).at(j).getDelay());
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