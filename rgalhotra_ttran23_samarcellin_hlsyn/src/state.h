#ifndef STATE_H
#define STATE_H

#include<string>
#include<vector>
#include<cstring>
#include<algorithm>

#include "node.h"

class state;

class state {
private:
	std::vector<node> _assignedNodes;

public:
	state();
	void addAssignedNode(node addNode);
	std::vector<node> getAssignedNodes();
};
#endif // STATE_H