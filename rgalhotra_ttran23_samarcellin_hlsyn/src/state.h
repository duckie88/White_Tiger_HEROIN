#ifndef STATE_H
#define STATE_H

#include<string>
#include<vector>
#include<algorithm>

#include "node.h"

class state;

class state {
private:
	std::vector<node> nodes;

public:
	state();
	void addNode(node addNode);
	std::vector<node> getNodes();
};
#endif // STATE_H