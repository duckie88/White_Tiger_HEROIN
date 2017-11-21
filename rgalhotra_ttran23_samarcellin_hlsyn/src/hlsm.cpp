#include "hlsm.h"

/*
Calculates the dependency and gets cycle times for each operation
*/
std::vector<std::vector<node>> scheduleASAP(int latency, std::vector<node> unscheduled) {
	int i = 0; //gaaaaah
	int j = 0;
	int k = 0;
	int m = 0;
	int n = 0;
	int z = 0;
	std::vector<std::vector<node>> ASAP; //output thing.
	ASAP.push_back(std::vector<node>()); //START THIS SHIT
	for (j = 0; j < unscheduled.size(); j++) { //go down the list of events to add
		ASAP.push_back(std::vector<node>());
		ASAP.at(i).push_back(unscheduled.at(j)); //start off by adding the first event
		unscheduled.at(j).setAsapTime(i); //leftover, will probably remove
		for (k = 1; k < unscheduled.size() - j; k++) { //check events after current one
			for (m = j; m < k; m++) { //check to make sure no conflicts
				if (unscheduled.at(m).getVarOne() == unscheduled.at(j + k).getVarOne() || unscheduled.at(m).getVarTwo() == unscheduled.at(j + k).getVarTwo()) {
					goto exitcond; //FIGHT ME STEPH
				}
				else {
					for (n = 0; n < unscheduled.at(j + k).getDelay(); n++) { //if delay, add it to a bunch of ASAPs
						for (z = 0; z < ASAP.at(i).size(); z++) { //check conflicts again
							if (ASAP.at(i).at(z).getVarOne() == unscheduled.at(j + k).getVarOne() || ASAP.at(i).at(z).getVarTwo() == unscheduled.at(j + k).getVarTwo()) {
								goto exitcond; //FIGHT ME STEPH
							}
						}
						ASAP.at(i + n).push_back(unscheduled.at(j + k)); //add it to every time slot it'll be happening in
					}
				}
			}
		}
	exitcond:
		j = j + k - 1; //skip all the events we already added
		i++; //next time slot
	}
	return ASAP;
}

/*
Calculates the dependency and gets cycle times for each operation
*/
bool scheduleALAP(int latency, std::vector<node>* unscheduled, std::vector<std::vector<node>>* ALAP) {
	return false;
}