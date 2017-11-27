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
	std::string result;
	std::string var1;
	std::string var2;
	std::string var3;
	std::vector<node*> prevNodes;
	std::vector<node*> nextNodes;
	std::vector<node*> nextIfNodes;
	std::vector<node*> nextElseNodes;
	std::vector<double> selfForce;
	int nodeNum;
	int delay;
	int asapTime;
	int alapTime;
	int cyclesElapsed;
	float probability;
	char visited;
	bool scheduled;
	bool conditional;
	int ifElseBlock;	

public:
	node();
	node(int nodeNum);

	std::string getOperation();
	void setOperation(std::string operation);
	std::string getResult();
	void setResult(std::string result);
	std::string getVar1();
	void setVar1(std::string var1);
	std::string getVar2();
	void setVar2(std::string var2);
	std::string getVar3();
	void setVar3(std::string var3);
	
	std::vector<node*> getPrevNodes();
	void setPrevNodes(std::vector<node*> previous);
	std::vector<node*> getNextNodes();
	void setNextNodes(std::vector<node*> next);
	std::vector<node*> getNextIfNodes();				// ?? What we gonna do
	void setNextIfNodes(std::vector<node*> ifNext);		// ?? with these?
	std::vector<node*> getNextElseNodes();				// ??
	void setNextElseNodes(std::vector<node*> elseNext);	// ??
	
	char getVisited();
	void setVisited(char visited);
	
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
	
	float getProbability();
	void setProbability(float probability);
	std::vector<double> getSelfForce();
	void addSelfForce(double val);
	void setSelfForce(int index, double val);

	bool getScheduled();
	void setScheduled(bool s);
	bool getConditional();
	void setConditional(bool c);
	int getIfElse();
	void setIfElse(int c);

	void addPrevNode(node* temp);
	void addNextNode(node* temp);
};

#endif