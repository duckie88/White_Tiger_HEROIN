#include "hlsm.h"


//Calculates the dependency and gets cycle times for each operation

bool scheduleASAP(unsigned int latency, std::vector<node>* unscheduled, std::vector<std::vector<node>>* ASAP) {
	unsigned int scheduled = 0;
	int i = 0;
	unsigned int j, k, m;
	bool unschedPrevNode = false;
	std::vector<node*> temp;

	for(j = 0; j < (*unscheduled).size(); j++){
		if((*unscheduled).at(j).getResult() == ""){
			(*unscheduled).at(j).setScheduled(true);
			scheduled++;
		}
	}

	while (scheduled < (*unscheduled).size()){
		(*ASAP).push_back(std::vector<node>());

		for (j = 0; j < (*unscheduled).size(); ++j) {
			unschedPrevNode = false;
			if ((*unscheduled).at(j).getScheduled() == false && (*unscheduled).at(j).getPrevNodes().size() == 0) {
					(*ASAP).at(i).push_back((*unscheduled).at(j));
					(*unscheduled).at(j).setAsapTime(i);
					(*unscheduled).at(j).setScheduled(true);
					scheduled++;

					//deal with the delay (if exists)
					for (k = 0; k < (*unscheduled).at(j).getNextNodes().size(); k++) {
						if (i + (*unscheduled).at(j).getDelay() >= (*unscheduled).at(j).getNextNodes().at(k)->getCyclesElapsed()) {
							(*unscheduled).at(j).getNextNodes().at(k)->setCyclesElapsed(i + (*unscheduled).at(j).getDelay());
						}
					}
				}
			else if ((*unscheduled).at(j).getScheduled() == false) {
				//schedule node if cycle is i
				temp = (*unscheduled).at(j).getPrevNodes();
				for (m = 0; m < temp.size(); m++) {
					if (temp.at(m)->getScheduled() == false) {
						unschedPrevNode = true;
					}
				}
				if ((*unscheduled).at(j).getCyclesElapsed() == i && !unschedPrevNode) {
					(*ASAP).at(i).push_back((*unscheduled).at(j));
					(*unscheduled).at(j).setAsapTime(i);
					(*unscheduled).at(j).setScheduled(true);
					scheduled++;
					//update node cycle time
					for (k = 0; k < (*unscheduled).at(j).getNextNodes().size(); ++k) {
						if (i + (*unscheduled).at(j).getDelay() >(*unscheduled).at(j).getNextNodes().at(k)->getCyclesElapsed()) {
							(*unscheduled).at(j).getNextNodes().at(k)->setCyclesElapsed(i + (*unscheduled).at(j).getDelay());
						}
					}
					if ((*unscheduled).at(j).getConditional()) { //if stuff
						temp = (*unscheduled).at(j).getNextIfNodes();
						for (k = 0; k < (*unscheduled).at(j).getNextIfNodes().size(); ++k) {
							if (i + (*unscheduled).at(j).getDelay() > temp.at(k)->getCyclesElapsed()) {
								temp.at(k)->setCyclesElapsed(i + (*unscheduled).at(j).getDelay());
							}
						}

						temp = (*unscheduled).at(j).getNextElseNodes(); //else stuff
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


//Calculates the dependency and gets cycle times for each operation

bool scheduleALAP(unsigned int latency, std::vector<node>* unscheduled, std::vector<std::vector<node>>* ALAP) {
	
	int i;
	unsigned int j, k;
	
	for(j = 0; j < (*unscheduled).size(); j++){ //deal with the input nodes (don't want to schedule them)
		if((*unscheduled).at(j).getResult() == ""){
			(*unscheduled).at(j).setScheduled(true);
			(*unscheduled).at(j).setCyclesElapsed(0);
		}
	}

	//adding empty vectors to the times -- populate the amount of timeslots to match latency
	for (i = 0; (unsigned int)i < latency; ++i) {
		(*ALAP).push_back(std::vector<node>());
	}

	for (i = latency - 1; i >= 0; --i) { //start at last timeslot and work backwards
		for (j = 0; j < (*unscheduled).size(); ++j) {
			if ((unsigned int)i == latency - 1) { //last timeslot
				if ((*unscheduled).at(j).getNextNodes().size() == 1 && (*unscheduled).at(j).getNextIfNodes().size() == 0 && !(*unscheduled).at(j).getScheduled()) { //???
					if ((*unscheduled).at(j).getDelay() > 1) { //if delay is longer than one cycle
						(*ALAP).at(i - (*unscheduled).at(j).getDelay() + 1).push_back((*unscheduled).at(j)); //set it so that the delay ends in last timeslot?
						(*unscheduled).at(j).setAlapTime(i - (*unscheduled).at(j).getDelay() + 1);
						(*unscheduled).at(j).setScheduled(true);
						for (k = 0; k < (*unscheduled).at(j).getPrevNodes().size(); ++k) { //work backwards from last node
							if (i - (*unscheduled).at(j).getDelay() + 1 < (*unscheduled).at(j).getPrevNodes().at(k)->getCyclesElapsed()) { //???
								(*unscheduled).at(j).getPrevNodes().at(k)->setCyclesElapsed(i - (*unscheduled).at(j).getDelay() + 1); //update Cycles Elapsed?
							}
						}
					}
					else { //if delay is one cycle
						(*ALAP).at(i).push_back((*unscheduled).at(j)); //add to last timeslot
						(*unscheduled).at(j).setAlapTime(i);
						(*unscheduled).at(j).setScheduled(true);
						for (k = 0; k < (*unscheduled).at(j).getPrevNodes().size(); ++k) { //work backwards from current node
							if (i - (*unscheduled).at(j).getDelay() < (*unscheduled).at(j).getPrevNodes().at(k)->getCyclesElapsed()) { //???
								(*unscheduled).at(j).getPrevNodes().at(k)->setCyclesElapsed(i - (*unscheduled).at(j).getDelay());
							}
						}
					}
				}
			}
			else { //if not last timeslot
				if ((*unscheduled).at(j).getCyclesElapsed() == i && !(*unscheduled).at(j).getScheduled()) { //if not scheduled
					if ((*unscheduled).at(j).getDelay() > 1) { //if delay is longer than 1
						(*ALAP)[i - (*unscheduled).at(j).getDelay()].push_back((*unscheduled).at(j)); //why is this so messy? assuming same behavior as before
						(*unscheduled).at(j).setAlapTime(i - (*unscheduled).at(j).getDelay());
						(*unscheduled).at(j).setScheduled(true);
						for (k = 0; k < (*unscheduled).at(j).getPrevNodes().size(); ++k) { //work backwards from current node
							if (i - (*unscheduled).at(j).getDelay() - 1 < (*unscheduled).at(j).getPrevNodes().at(k)->getCyclesElapsed()) {
								(*unscheduled).at(j).getPrevNodes().at(k)->setCyclesElapsed(i - (*unscheduled).at(j).getDelay());
							}
						}
					}
					else { //if delay is 1
						(*ALAP).at(i).push_back((*unscheduled).at(j)); //add to current timeslot
						(*unscheduled).at(j).setAlapTime(i);
						(*unscheduled).at(j).setScheduled(true);
						for (k = 0; k < (*unscheduled).at(j).getPrevNodes().size(); ++k) { //work backwards from current node
							if (i - (*unscheduled).at(j).getDelay() < (*unscheduled).at(j).getPrevNodes().at(k)->getCyclesElapsed()) {
								(*unscheduled).at(j).getPrevNodes().at(k)->setCyclesElapsed(i - (*unscheduled).at(j).getDelay());
							}
						}
					}

				}
			}
		}
	}
	for (i = 0; (unsigned int)i < (*unscheduled).size(); ++i) {
		if (!(*unscheduled).at(i).getScheduled()) std::cout << (*unscheduled).at(i).getResult() << std::endl;
		if ((*unscheduled).at(i).getCyclesElapsed() < 0) {
			//return false;
		}
	}
	return true; //shouldn't we check for if all unscheduled nodes have true scheduled?
}

bool FDS(int totalNodes, std::vector<std::vector<node>>* ASAP, std::vector<std::vector<node>>* ALAP){
	int scheduled = 0;
	int i, j;
	std::vector<float> mulDist;
	std::vector<float> divDist;
	std::vector<float> addDist;
	std::vector<float> logicDist;

	bool finished = false;

	while(!finished){
	//calculate time frames - we did this in actual scheduling by setting asap and alap time variables
	
	//calculate probability dist
		//1/(ASAP - ALAP + 1)
		for(i = 0; (unsigned int)i < (*ALAP).size(); i++){
			for(j = 0; (unsigned int)j < (*ALAP).at(i).size(); j++){
				std::cout << (*ALAP).at(i).at(j).getNodeNum();
			}
		}


	//self force
		//for each possible time
		// sf = distribution@thisT(1-probability) + distribution@otherT1(0-probability) + distribution@otherT2(0 -probability) ...

	//predecesor force
	//sucessor force
	///schedule least force
		scheduled++;
	//update timeframe of scheduled node (or else future pred/succ foces will be off)
		
	//check exit condition (all nodes scheduled)
		if(scheduled == totalNodes){
			finished = true;
		}
	}


	return true;
}