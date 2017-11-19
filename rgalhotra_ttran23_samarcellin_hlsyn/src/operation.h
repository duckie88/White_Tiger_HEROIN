#ifndef OPERATION_H
#define OPERATION_H

#include "node.h"	// Has <string> and <vector>

class operation {
private:
	int lineNum;
	std::string operType;	// For whatever the operation is
	node output;	// For output
	node input1;	// For first input
	node input2;	// For second input
	node variable;	// For mux operations
	int delay;	// The cycle time required: mul = 2 cycles, div/mod = 3 cycles, add/sub/logic/else = 1 cycle
	bool cond; //if it's within a conditional
public:
	// Constructors
	operation();
	operation(int start, std::string oper, node out, node in1, node in2, node var, int delay, bool cond);

	// Methods
	
	// Getters
	int getLineNum();
	std::string getOperType();
	node getInput1();
	node getInput2();
	node getOutput();
	node getMuxSel();
	int getDelay();
	bool getConditional();

	// Setters. Probably won't see use but declared for consistency sake.
	void setLineNum(int start);
	void setOperType(std::string type);
	void setInput1(node in1);
	void setInput2(node in2);
	void setOutput(node out);
	void setMuxSel(node sel);
	void setDelay(int num);
	void setConditional(bool cond);
};

#endif //OPERATION_H