#include "state.h"

state::state() {
	//constructor
}

void state::addAssignedNode(node* addNode) {
	_assignedNodes.push_back(addNode);
}

std::vector<node*> state::getAssignedNodes() {
	return _assignedNodes;
}