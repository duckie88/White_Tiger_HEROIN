#ifndef STATE_H
#define STATE_H

#include "node.h"

class state {
private:
	std::vector<node> nodes;

public:
	state();
	void addNode(node addNode);
	std::vector<node> getNodes();
};
#endif // STATE_H