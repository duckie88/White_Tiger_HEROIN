#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cstring>
#include<algorithm>

class node {
private:
	std::string operation;
	std::vector<node*> prevNodes;
	std::vector<node*> nextNodes;
	std::vector<node*> nextIfNodes;
	std::vector<node*> nextElseNodes;
	int nodeNum;
	int delay;
	int asapTime;
	int alapTime;
	int cyclesElapsed;
	char visited;
	bool scheduled;
	bool conditional;


public:
	node();
	node(int nodeNum);

	std::string getOperation();
	void setOperation(std::string operation);
	
	
	std::vector<node*> getPrevNodes();
	void setPrevNodes(std::vector<node*> previous);
	std::vector<node*> getNextNodes();
	void setNextNodes(std::vector<node*> next);
	std::vector<node*> getNextIfNodes();
	void setNextIfNodes(std::vector<node*> ifNext);
	std::vector<node*> getNextElseNodes();
	void setNextElseNodes(std::vector<node*> elseNext);
	
	
	char getVisted();
	void setVisted(char visited);
	
	
	int getNodeNum();
	void setNodeNum(int num);
	int getDelay();
	void setDelay(int delay);
	int getAsapTime();
	void setAsapTime(int time);
	int getAlapTime();
	void setAlapTime(int time);
	int getCyclesElapsed();
	void setCyclesElapsed(int cycles);
	
	
	bool getScheduled();
	void setScheduled(bool s);
	bool getConditional();
	void setConditional(bool c);

	void addPrevNode(node* temp);
	void addNextNode(node* temp);
};

#endif