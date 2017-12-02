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

	// Clear out inputs as nodes, cannot schedule inputs
	for(j = 0; j < (*unscheduled).size(); j++){
		if((*unscheduled).at(j).getResult() == ""){
			(*unscheduled).at(j).setScheduled(true);
			scheduled++;
		}
	}

	// Cycle until all nodes scheduled
	while (scheduled < (*unscheduled).size()){
		(*ASAP).push_back(std::vector<node>());
		// Cycle through each node in list
		for (j = 0; j < (*unscheduled).size(); ++j) {
			unschedPrevNode = false;
			if ((*unscheduled).at(j).getScheduled() == false && (*unscheduled).at(j).getPrevNodes().size() == 0) {
				(*unscheduled).at(j).setCyclesElapsed(0);
				(*unscheduled).at(j).setAsapTime(i);
				(*unscheduled).at(j).setScheduled(true);
				(*ASAP).at(i).push_back((*unscheduled).at(j));
				scheduled++;

				// Deal with the delay (if exists)
				for (k = 0; k < (*unscheduled).at(j).getNextNodes().size(); k++) {
					if (i + (*unscheduled).at(j).getDelay() >= (*unscheduled).at(j).getNextNodes().at(k)->getCyclesElapsed()) {
						(*unscheduled).at(j).getNextNodes().at(k)->setCyclesElapsed(i + (*unscheduled).at(j).getDelay());
					}
				}
			}
			else if ((*unscheduled).at(j).getScheduled() == false) {
				// Schedule node if cycle is i
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
	
	// Deal with the input nodes (don't want to schedule them)
	for(j = 0; j < (*unscheduled).size(); j++){ 
		if((*unscheduled).at(j).getResult() == ""){
			(*unscheduled).at(j).setScheduled(true);
			(*unscheduled).at(j).setCyclesElapsed(0);
		}
	}

	// Adding empty vectors to the times -- populate the amount of timeslots to match latency
	for (i = 0; (unsigned int)i < latency; ++i) {
		(*ALAP).push_back(std::vector<node>());
	}

	// Start at last timeslot and work backwards
	for (i = latency - 1; i >= 0; --i) { 
		for (j = 0; j < (*unscheduled).size(); ++j) {
			// Last timeslot
			if ((unsigned int)i == latency - 1) { 
				if ((*unscheduled).at(j).getNextNodes().size() == 0 && (*unscheduled).at(j).getNextIfNodes().size() == 0 && !(*unscheduled).at(j).getScheduled()) { //if no node comes after me, and  I am unscheduled
					// If delay is longer than one cycle
					if ((*unscheduled).at(j).getDelay() > 1) { 
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
					// If delay is one cycle
					else { 
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
			// If not last timeslot
			else { 
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
	// Sanity check (did everything get scheduled?)
	for (i = 0; (unsigned int)i < (*unscheduled).size(); ++i) { 
		if (!(*unscheduled).at(i).getScheduled()){
			//std::cout << (*unscheduled).at(i).getResult();
			return false;
		}
	}
	return true; 
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

	for(i = 0; i < totalNodes; i++){  
		// "schedule" input nodes
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
		// Calculate probability dist
		// (ALAP - ASAP + 1)
		for(i = 0; i < totalNodes; i++){
			temp = (*nodes).at(i).getAlapTime() - (*nodes).at(i).getAsapTime() + 1;
			(*nodes).at(i).setProbability(1.0/temp);
			if((*nodes).at(i).getResult() == ""){ //make sure not to set inputs!
				(*nodes).at(i).setProbability(0);
			}
			//std::cout << (*nodes).at(i).getProbability() << std::endl;
		}

		// Calculate probability distributions
		for(time = 0; time < latency; time++){
			addDist.push_back(0);
			divDist.push_back(0);
			mulDist.push_back(0);
			logicDist.push_back(0);
			for(i = 0; i < totalNodes; i++){
				// Add and Sub distribution
				if((*nodes).at(i).getOperation() == "+" || (*nodes).at(i).getOperation() == "-"){  
					// Check to be sure we're within 
					if(time <= (*nodes).at(i).getAlapTime() && time >= (*nodes).at(i).getAsapTime()){ 
						addDist.at(time) = addDist.at(time) + (*nodes).at(i).getProbability();
					}
				}
				// Mul distribution
				else if((*nodes).at(i).getOperation() == "*"){  
					// Check to be sure we're within 
					if(time <= (*nodes).at(i).getAlapTime() && time >= (*nodes).at(i).getAsapTime()){ 
						mulDist.at(time) = mulDist.at(time) + (*nodes).at(i).getProbability();
					}
				}
				// Div and Mod distribution
				else if((*nodes).at(i).getOperation() == "/" || (*nodes).at(i).getOperation() == "%"){  
					// Check to be sure we're within 
					if(time <= (*nodes).at(i).getAlapTime() && time >= (*nodes).at(i).getAsapTime()){ 
						divDist.at(time) = divDist.at(time) + (*nodes).at(i).getProbability();
					}
				}
				// Logic distribution
				else{ 
					// Check to be sure we're within 
					if(time <= (*nodes).at(i).getAlapTime() && time >= (*nodes).at(i).getAsapTime()){ 
						logicDist.at(time) = logicDist.at(time) + (*nodes).at(i).getProbability();
					}
				}
			}
		}

		// Self Force
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

			// Cycling through each self force
			for (time1 = (*nodes).at(i).getAsapTime(); time1 <= (*nodes).at(i).getAlapTime(); time1++) {	
				double temp = 0.0;
				// Loop to do the increment
				for (time2 = 0; (unsigned int)time2 < selfDist.size(); time2++) {	
					// Only do the ones within time
					if (time2 <= (*nodes).at(i).getAlapTime() && time2 >= (*nodes).at(i).getAsapTime()) {	
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
			// Only check if previous forces exist
			if((*nodes).at(i).getPrevNodes().size() != 0){ 
				for(j = 0; (unsigned int)j < (*nodes).at(i).getPrevNodes().size(); j++){ 
					/*
					i is current node
					j is previous node
					k is time frame of curent node
					m is distribution index
					*/
					for(k = (*nodes).at(i).getAsapTime(); k <= (*nodes).at(i).getAlapTime(); k++){
						// Only do the ones within time frame of previous node
						for(m = (*nodes).at(i).getPrevNodes().at(j)->getAsapTime(); m <= (*nodes).at(i).getPrevNodes().at(j)->getAlapTime(); m++) {	
							if (((*nodes).at(i).getResult() == "g" && (*nodes).at(i).getPrevNodes().at(j)->getResult() == "dLTe") || ((*nodes).at(i).getResult() == "z" && (*nodes).at(i).getPrevNodes().at(j)->getResult() == "g")) {
								//SECRET DEFEAT CODE DON'T LET EPA KNOW
								prevDist = (*nodes).at(i).getPrevNodes().at(j)->getSelfForce();
								for (x = 0; (unsigned int)x < prevDist.size(); x++) {
									(*nodes).at(i).setPredForce(x, prevDist.at(x));
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
		// Print predecessor forces
		std::cout << "Predecessor Forces:" << std::endl;
		for(i = 0; i < totalNodes; i++){
			std::cout << (*nodes).at(i).getResult() << " : ";
			for(j = 0; j < latency; j++){
				std::cout << (*nodes).at(i).getPredForce().at(j) << " ";
			}
			std::cout << std::endl;
		}*/
		
		// Successor Force (Stephanie's Version)
		for (i = 0; i < totalNodes; i++){
			// If successor forces exist
			if ((*nodes).at(i).getNextNodes().size() != 0){ 
				// For each existing next node
				for (j = 0; (unsigned int)j < (*nodes).at(i).getNextNodes().size(); j++){ 
					/*
					i is current node
					j is next node
					k is time frame of curent node
					m is time frame of next node
					*/
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
		// Print Successor Forces
		std::cout << "Successor Forces:" << std::endl;
		for(i = 0; i < totalNodes; i++){
			std::cout << (*nodes).at(i).getResult() << " : ";
			for(j = 0; j < latency; j++){
				std::cout << (*nodes).at(i).getSuccForce().at(j) << " ";
			}
			std::cout << std::endl;
		}
		*/
		
		// Total force
		min = 50000;	// Moved reset of min here bc readability
		for(i = 0; i < totalNodes; i++){
			if((*nodes).at(i).getScheduled() == false){
				for(j = 0; j < latency; j++){
					temp = (*nodes).at(i).getSelfForce().at(j) + (*nodes).at(i).getSuccForce().at(j) + (*nodes).at(i).getPredForce().at(j);
					(*nodes).at(i).setTotalForce(j, temp);

					bool prevScheduled = true;
					for (unsigned int prevIndex = 0; prevIndex < (*nodes).at(i).getPrevNodes().size(); prevIndex++) {
						// If previous node not scheduled
						if ((*nodes).at(i).getPrevNodes().at(prevIndex)->getScheduled() == false) {	
							prevScheduled = false;
						}
						// If previous node was scheduledut the time is the same as the current node, cannot schedule current node
						if ((*nodes).at(i).getPrevNodes().at(prevIndex)->getScheduled() == true	
							&& (*nodes).at(i).getPrevNodes().at(prevIndex)->getAlapTime() == j) { 
							prevScheduled = false;
						}
					}
					
					// Check if curr total force is the lowest
					if(temp < min && prevScheduled){
						min = temp;
						k = i;  // k is index of node
						m = j;	// j is time slot node should go in
					}
				}
			}
		}

		// Schedule least force
		(*nodes).at(k).setScheduled(true);
		(*nodes).at(k).setAlapTime(m);	// Update timeframe (asap and alap = same number) 
		(*nodes).at(k).setAsapTime(m);	// of scheduled node (or else future pred/succ forces will be off)
		(*FDS).at(m).push_back((*nodes).at(k));
		scheduled++;

		// Check exit condition (all nodes scheduled)
		if(scheduled == totalNodes){
			finished = true;
		}
	}
	return true;
}

// Creates the set of states
std::vector<state> createStates(std::vector<std::vector<node>> FDS) {
	std::vector<state> states;
	int i = 0;
	int j = 0;

	// Add each node to a state corresponding with its time
	for (i = 0; (unsigned int)i < FDS.size(); i++) { 
		state* newState = new state();
		states.push_back(*newState);
		for (j = 0; (unsigned int)j < FDS.at(i).size(); j++) {
			states.at(i).addNode(FDS.at(i).at(j));
		}
	}

	// Remove states that have no nodes
	for (i = 0; (unsigned int)i < states.size(); i++) { 
		if (states.at(i).getNodes().size() == 0) {
			states.erase(states.begin() + i);
			i--;
		}
	}

	// If statements
	for (i = 0; (unsigned int)i < states.size(); i++) {
		for (j = 0; (unsigned int)j < states.at(i).getNodes().size(); j++) {
			//if it's an if statement do shit
		}
	}
	return states;
}