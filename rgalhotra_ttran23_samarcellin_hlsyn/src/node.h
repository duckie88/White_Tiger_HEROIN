#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

class node {
public:
	node(); //default constructor
	node(std::string type, std::string name, bool SIGN, int dataSize);
	void setName(std::string name);
	std::string getName();
	void setSIGN(bool SIGN);
	bool getSIGN();
	void setDataSize(int dataSize);
	int getDataSize();
	void setType(std::string type);
	std::string getType();
	void addEdge(node* nextNode);
	std::vector<node*> getEdges();
	void addWeight(int weight);
	std::vector<int> getWeights();
private:
	std::string type;
	std::string name;
	bool SIGN;
	int dataSize;
	std::vector<node*> edges;
	std::vector<int> weights;
};

#endif