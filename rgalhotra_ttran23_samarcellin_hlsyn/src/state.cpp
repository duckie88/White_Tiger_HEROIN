#include "state.h"

state::state() {
	//constructor
}

void state::addNode(node addNode) {
	this->nodes.push_back(addNode);
}

std::vector<node> state::getNodes() {
	return this->nodes;
}