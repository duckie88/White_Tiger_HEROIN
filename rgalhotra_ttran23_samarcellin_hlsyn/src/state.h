#ifndef STATE_H
#define STATE_H

#include "node.h"

class state {
private:
	std::vector<node> nodes;

public:
	state();
	void addNode(node addNode);
	std::vector<node> getNodes(); //stop changing this, it won't compile if you put the function body here
};
#endif // STATE_H