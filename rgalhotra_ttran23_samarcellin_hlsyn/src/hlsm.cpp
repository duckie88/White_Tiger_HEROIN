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
					(*unscheduled).at(j).setCyclesElapsed(0);
					(*unscheduled).at(j).setAsapTime(i);
					(*unscheduled).at(j).setScheduled(true);
					(*ASAP).at(i).push_back((*unscheduled).at(j));
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
					(*unscheduled).at(j).setAsapTime(i);
					(*unscheduled).at(j).setScheduled(true);
					(*ASAP).at(i).push_back((*unscheduled).at(j));
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
	
	int i, m;
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
				if ((*unscheduled).at(j).getNextNodes().size() == 0 && (*unscheduled).at(j).getNextIfNodes().size() == 0 && !(*unscheduled).at(j).getScheduled()) { //if no node comes after me, and  I am unscheduled
					if ((*unscheduled).at(j).getDelay() > 1) { //if delay is longer than one cycle
						(*unscheduled).at(j).setAlapTime(i - (*unscheduled).at(j).getDelay() + 1);
						(*unscheduled).at(j).setScheduled(true);
						(*ALAP).at(i - (*unscheduled).at(j).getDelay() + 1).push_back((*unscheduled).at(j)); //set it so that the delay ends in last timeslot
						for (k = 0; k < (*unscheduled).at(j).getPrevNodes().size(); ++k) { //work backwards from current node
							//fix cycles elapsed
							if((*unscheduled).at(j).getPrevNodes().at(k)->getCyclesElapsed()  == -1){
								(*unscheduled).at(j).getPrevNodes().at(k)->setCyclesElapsed(i-1);
							}
							for(m = 0; (unsigned int)m < (*unscheduled).at(j).getPrevNodes().at(k)->getPrevNodes().size(); m++){
								(*unscheduled).at(j).getPrevNodes().at(k)->getPrevNodes().at(m)->setCyclesElapsed(i-2);
							}
						}
					}
					else { //if delay is one cycle
						(*unscheduled).at(j).setAlapTime(i);
						(*unscheduled).at(j).setScheduled(true);
						(*ALAP).at(i).push_back((*unscheduled).at(j)); //add to last timeslot
						for (k = 0; k < (*unscheduled).at(j).getPrevNodes().size(); ++k) { //work backwards from current node
							//fix cycles elapsed
							if((*unscheduled).at(j).getPrevNodes().at(k)->getCyclesElapsed()  == -1){
								(*unscheduled).at(j).getPrevNodes().at(k)->setCyclesElapsed(i-1);
							}
							for(m = 0; (unsigned int)m < (*unscheduled).at(j).getPrevNodes().at(k)->getPrevNodes().size(); m++){
								(*unscheduled).at(j).getPrevNodes().at(k)->getPrevNodes().at(m)->setCyclesElapsed(i-2);
							}
						}
					}
				}
			}
			else { //if not last timeslot
				if ((*unscheduled).at(j).getCyclesElapsed() == i && !(*unscheduled).at(j).getScheduled()) { //if not scheduled
					if ((*unscheduled).at(j).getDelay() > 1) { //if delay is longer than 1
						(*unscheduled).at(j).setAlapTime(i - (*unscheduled).at(j).getDelay() + 1);
						(*unscheduled).at(j).setScheduled(true);
						(*ALAP).at(i - (*unscheduled).at(j).getDelay() + 1).push_back((*unscheduled).at(j)); 
						for (k = 0; k < (*unscheduled).at(j).getPrevNodes().size(); ++k) { //work backwards from current node
							//fix cycles elapsed
							if((*unscheduled).at(j).getPrevNodes().at(k)->getCyclesElapsed()  == -1){
								(*unscheduled).at(j).getPrevNodes().at(k)->setCyclesElapsed(i-1);
							}
							for(m = 0; (unsigned int)m < (*unscheduled).at(j).getPrevNodes().at(k)->getPrevNodes().size(); m++){
								(*unscheduled).at(j).getPrevNodes().at(k)->getPrevNodes().at(m)->setCyclesElapsed(i-2);
							}
						}
					}
					else { //if delay is 1
						(*unscheduled).at(j).setAlapTime(i);
						(*unscheduled).at(j).setScheduled(true);
						(*ALAP).at(i).push_back((*unscheduled).at(j)); //add to current timeslot
						for (k = 0; k < (*unscheduled).at(j).getPrevNodes().size(); ++k) { //work backwards from current node
							//fix cycles elapsed
							if((*unscheduled).at(j).getPrevNodes().at(k)->getCyclesElapsed()  == -1){
								(*unscheduled).at(j).getPrevNodes().at(k)->setCyclesElapsed(i-1);
							}
							for(m = 0; (unsigned int)m < (*unscheduled).at(j).getPrevNodes().at(k)->getPrevNodes().size(); m++){
								(*unscheduled).at(j).getPrevNodes().at(k)->getPrevNodes().at(m)->setCyclesElapsed(i-2);
							}
						}
					}

				}
			}
		}
	}
	for (i = 0; (unsigned int)i < (*unscheduled).size(); ++i) { //fuck up check (did everything get scheduled?)
		if (!(*unscheduled).at(i).getScheduled()){
			//std::cout << (*unscheduled).at(i).getResult();
			return false;
		}
	}
	return true; //shouldn't we check for if all unscheduled nodes have true scheduled? // ANSWER: that is what the for loop with the return false does
}

bool FDS(int totalNodes, int latency, std::vector<node>* nodes){
	int scheduled = 0;
	int i, temp, time;
	std::vector<double> mulDist;
	std::vector<double> divDist;
	std::vector<double> addDist;
	std::vector<double> logicDist;

	bool finished = false;

	for(i = 0; i < totalNodes; i++){  //"schedule" input nodes
		if((*nodes).at(i).getResult() == ""){
			(*nodes).at(i).setScheduled(true);
			scheduled++;
		}
	}

	while(!finished){
	//calculate probability dist
		//1/(ALAP - ASAP + 1)
		for(i = 0; i < totalNodes; i++){
			temp = (*nodes).at(i).getAlapTime() - (*nodes).at(i).getAsapTime() + 1;
			(*nodes).at(i).setProbability(1.0/temp);
			if((*nodes).at(i).getResult() == ""){ //make sure not to set inputs!
				(*nodes).at(i).setProbability(0);
			}
			//std::cout << (*nodes).at(i).getProbability() << std::endl;
		}

	//calculate probability distributions
		for(time = 0; time < latency; time++){
			addDist.push_back(0);
			divDist.push_back(0);
			mulDist.push_back(0);
			logicDist.push_back(0);
			for(i = 0; i < totalNodes; i++){
				if((*nodes).at(i).getOperation() == "+" || (*nodes).at(i).getOperation() == "-"){  //add and sub distribution
					if(time <= (*nodes).at(i).getAlapTime() && time >= (*nodes).at(i).getAsapTime()){ //check to be sure we're within 
						addDist.at(time) = addDist.at(time) + (*nodes).at(i).getProbability();
					}
				}
				else if((*nodes).at(i).getOperation() == "*"){  //multiply distribution
					if(time <= (*nodes).at(i).getAlapTime() && time >= (*nodes).at(i).getAsapTime()){ //check to be sure we're within 
						mulDist.at(time) = mulDist.at(time) + (*nodes).at(i).getProbability();
					}
				}
				else if((*nodes).at(i).getOperation() == "/" || (*nodes).at(i).getOperation() == "%"){  //div and mod distribution
					if(time <= (*nodes).at(i).getAlapTime() && time >= (*nodes).at(i).getAsapTime()){ //check to be sure we're within 
						divDist.at(time) = divDist.at(time) + (*nodes).at(i).getProbability();
					}
				}
				else{  //logic distribution
					if(time <= (*nodes).at(i).getAlapTime() && time >= (*nodes).at(i).getAsapTime()){ //check to be sure we're within 
						logicDist.at(time) = logicDist.at(time) + (*nodes).at(i).getProbability();
					}
				}
			}
		}

	//SELF FORCE
	// sf = distribution@thisT(1-probability) + distribution@otherT1(0-probability) + distribution@otherT2(0 -probability) ...

	//predecesor force
	//sucessor force
	//schedule least force
		
	scheduled++;
	//update timeframe (asap and alap = same number) of scheduled node (or else future pred/succ foces will be off)
		
	//check exit condition (all nodes scheduled)
		if(scheduled == totalNodes){
			finished = true;
		}
	}


	return true;
}

std::vector<state> createStates(std::vector<std::vector<node>> FDS) {
	std::vector<state> states;
	int i = 0;
	int j = 0;

	for (i = 0; (unsigned int)i < FDS.size(); i++) { //add each node to a state corresponding with its time
		state* newState = new state();
		states.push_back(*newState);
		for (j = 0; (unsigned int)j < FDS.at(i).size(); j++) {
			states.at(i).addNode(FDS.at(i).at(j));
		}
	}
	for (i = 0; (unsigned int)i < states.size(); i++) { //remove states that have no nodes
		if (states.at(i).getNodes().size() == 0) {
			states.erase(states.begin() + i);
			i--;
		}
	}
	//if statements
	for (i = 0; (unsigned int)i < states.size(); i++) {
		for (j = 0; (unsigned int)j < states.at(i).getNodes().size(); j++) {
			//if it's an if statement do shit
		}
	}
	return states;
}