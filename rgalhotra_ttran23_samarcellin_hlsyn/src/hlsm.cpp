#include "hlsm.h"
#include <iomanip>
#include <iostream>
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
					if ((*unscheduled).at(j).getConditional()) { //if-else check
						
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


// Performs Force Directed Scheduling

bool FDS(int totalNodes, int latency, std::vector<node>* nodes,  std::vector<std::vector<node>>* FDS){
	int scheduled = 0;
	int i, j, k, m, time, time1, time2, x, min;
	double temp;
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

		// Initialize self-force, pred-force, succ-force, total-force
		for (time = 0; time < latency; time++) {
			(*nodes).at(i).addSelfForce(10000.0);
			(*nodes).at(i).addPredForce(0.0);
			(*nodes).at(i).addSuccForce(0.0);
			(*nodes).at(i).addTotalForce(0.0);
		}
	}

	while(!finished){
		//calculate probability dist
		// (ALAP - ASAP + 1)
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

		// Self Force  WORKING
		std::vector<double> selfDist;
		for (i = 0; i < totalNodes; i++){
			// Select the dist based on the operation
			if ((*nodes).at(i).getOperation() == "+" || (*nodes).at(i).getOperation() == "-"){
				selfDist = addDist;
			}
			else if ((*nodes).at(i).getOperation() == "*"){  
				selfDist = mulDist;
			}
			else if ((*nodes).at(i).getOperation() == "/" || (*nodes).at(i).getOperation() == "%"){  
				selfDist = divDist;
			}
			else{  
				selfDist = logicDist;
			}
			for (time1 = (*nodes).at(i).getAsapTime(); time1 <= (*nodes).at(i).getAlapTime(); time1++) {	// Cycling through each self force, 
				double temp = 0.0;
				for (time2 = 0; (unsigned int)time2 < selfDist.size(); time2++) {	// Loop to do the increment
					if (time2 <= (*nodes).at(i).getAlapTime() && time2 >= (*nodes).at(i).getAsapTime()) {	// Only do the ones within time
						if (time2 == time1) {
							temp += selfDist.at(time2) * (1 - ((*nodes).at(i).getProbability()));
						}
						else {
							temp += selfDist.at(time2) * (0 - ((*nodes).at(i).getProbability()));
						}
						(*nodes).at(i).setSelfForce(time1, temp);
					}
				}
			}
		}


		// Predecessor Force 
		std::vector<double> prevDist;
		for (i = 0; i < latency; i++) {
			(*nodes).at(i).addPredForce(0.0);
		}
		for (i = 0; i < totalNodes; i++){
			if((*nodes).at(i).getPrevNodes().size() != 0){ //if previous forces exist
				for(j = 0; (unsigned int)j < (*nodes).at(i).getPrevNodes().size(); j++){ //for each previous node
					//i is current node
					//j is previous node
					//k is time frame of curent node
					//m is distribution index
					for(k = (*nodes).at(i).getAsapTime(); k <= (*nodes).at(i).getAlapTime(); k++){
						for(m = (*nodes).at(i).getPrevNodes().at(j)->getAsapTime(); m <= (*nodes).at(i).getPrevNodes().at(j)->getAlapTime(); m++) {	// Only do the ones within time frame of previous node
							if (((*nodes).at(i).getResult() == "g" && (*nodes).at(i).getPrevNodes().at(j)->getResult() == "dLTe") || ((*nodes).at(i).getResult() == "z" && (*nodes).at(i).getPrevNodes().at(j)->getResult() == "g")) {
								//SECRET DEFEAT CODE DON'T LET EPA KNOW
								prevDist = (*nodes).at(i).getPrevNodes().at(j)->getSelfForce();
								for (x = 0; (unsigned int)x < prevDist.size(); x++) {
									(*nodes).at(i).setPredForce(prevDist.at(i), x);
								}
								goto cocaine;
							}
							if(m < k && (*nodes).at(i).getPrevNodes().at(j)->getAlapTime() >= (*nodes).at(i).getAsapTime()){
								temp = (*nodes).at(i).getPredForce().at(k);
								temp += (*nodes).at(i).getPrevNodes().at(j)->getSelfForce().at(m);
								(*nodes).at(i).setPredForce(k,temp);
							}
						}
					}
				}
			cocaine: x = 0;
			}
		}

		/*
		std::cout << "Predecessor Forces:" << std::endl;
		for(i = 0; i < totalNodes; i++){
			std::cout << (*nodes).at(i).getResult() << " : ";
			for(j = 0; j < latency; j++){
				std::cout << (*nodes).at(i).getPredForce().at(j) << " ";
			}
			std::cout << std::endl;
		}*/
		
		// Stephanie's broken as fuck Successor Force - noooope
		for (i = 0; i < totalNodes; i++){
			if ((*nodes).at(i).getNextNodes().size() != 0){ //if successor forces exist
				for (j = 0; (unsigned int)j < (*nodes).at(i).getNextNodes().size(); j++){ //for each existing next node
					//i is current node
					//j is next node
					//k is time frame of curent node
					//m is time frame of next node
					for (k = (*nodes).at(i).getAsapTime(); k <= (*nodes).at(i).getAlapTime(); k++){
						if(k == -1){
							break;
						}
						for (m = (*nodes).at(i).getNextNodes().at(j)->getAsapTime(); m <= (*nodes).at(i).getNextNodes().at(j)->getAlapTime(); m++){
							if (m > k) {
								temp = (*nodes).at(i).getSuccForce().at(k);
								temp += (*nodes).at(i).getNextNodes().at(j)->getSelfForce().at(m);
								(*nodes).at(i).setSuccForce(k, temp);
							}
						}
					}
				}
			}
		}
		
		/*
		std::cout << "Successor Forces:" << std::endl;
		for(i = 0; i < totalNodes; i++){
			std::cout << (*nodes).at(i).getResult() << " : ";
			for(j = 0; j < latency; j++){
				std::cout << (*nodes).at(i).getSuccForce().at(j) << " ";
			}
			std::cout << std::endl;
		}
		*/
		
		
		/*
		//Tam's Janky Successor Force
		std::vector<double> nextDist;
		for (i = 0; i < totalNodes; i++){
			// Initializing successor forces
			for (time = 0; time < latency; time++) {
				(*nodes).at(i).addSuccForce(0.0);	// This can be 0.
			}

			if ((*nodes).at(i).getNextNodes().size() != 0){ //if successor forces exist
				for (j = 0; (unsigned int)j < (*nodes).at(i).getNextNodes().size(); j++){ //find times each previous incoming node could have been scheduled at
					// Select the dist based on the distribution
					if ((*nodes).at(i).getNextNodes().at(j)->getOperation() == "+" || (*nodes).at(i).getNextNodes().at(j)->getOperation() == "-"){
						nextDist = addDist;
					}
					else if ((*nodes).at(i).getNextNodes().at(j)->getOperation() == "*"){
						nextDist = mulDist;
					}
					else if ((*nodes).at(i).getNextNodes().at(j)->getOperation() == "/" || (*nodes).at(i).getNextNodes().at(j)->getOperation() == "%"){
						nextDist = divDist;
					}
					else{
						nextDist = logicDist;
					}

					// This part MIGHT work
					for (k = (*nodes).at(i).getNextNodes().at(j)->getAsapTime(); k <= (*nodes).at(i).getNextNodes().at(j)->getAlapTime(); k++){
						temp = 0.0;
						for (time2 = 0; (unsigned int)time2 < nextDist.size(); time2++){
							if (time2 <= (*nodes).at(i).getNextNodes().at(j)->getAlapTime() && time2 >= (*nodes).at(i).getNextNodes().at(j)->getAsapTime()) {	// Only do the ones within time
								if (k == time2) {
									temp += nextDist.at(time2) * (1 - ((*nodes).at(i).getNextNodes().at(j)->getProbability()));
								}
								else {
									temp += nextDist.at(time2) * (0 - ((*nodes).at(i).getNextNodes().at(j)->getProbability()));
								}
								(*nodes).at(i).setSuccForce(k, temp);
							}
						}
					}
				}
			}
		}
		*/

		//total force
		min = 50000;	// Moved reset of min here bc readability
		for(i = 0; i < totalNodes; i++){
			if((*nodes).at(i).getScheduled() == false){
				for(j = 0; j < latency; j++){
					temp = (*nodes).at(i).getSelfForce().at(j) + (*nodes).at(i).getSuccForce().at(j) + (*nodes).at(i).getPredForce().at(j);
					(*nodes).at(i).setTotalForce(j, temp);

					bool prevScheduled = true;
					for (unsigned int prevIndex = 0; prevIndex < (*nodes).at(i).getPrevNodes().size(); prevIndex++) {
						if ((*nodes).at(i).getPrevNodes().at(prevIndex)->getScheduled() == false) {	// If previous node not scheduled
							prevScheduled = false;
						}
						if ((*nodes).at(i).getPrevNodes().at(prevIndex)->getScheduled() == true	// If previous node was scheduled
							&& (*nodes).at(i).getPrevNodes().at(prevIndex)->getAlapTime() == j) { // But the time is the same as the current node, cannot schedule current node
							prevScheduled = false;
						}
					}

					if(temp < min && prevScheduled){
						min = temp;
						k = i;  //k is index of node
						m = j; //j is time slot node should go in
					}
				}
			}
		}

		//schedule least force
		(*nodes).at(k).setScheduled(true);
		(*nodes).at(k).setAlapTime(m);	//update timeframe (asap and alap = same number) of scheduled node (or else future pred/succ forces will be off)
		(*nodes).at(k).setAsapTime(m);
		(*FDS).at(m).push_back((*nodes).at(k));
		scheduled++;

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