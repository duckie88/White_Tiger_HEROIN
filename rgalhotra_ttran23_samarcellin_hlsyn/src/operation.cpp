#include "operation.h"

operation::operation() {
	this->lineNum = -1;
	this->operType = "";	
	this->output = node();	
	this->input1 = node();	
	this->input2 = node();	
	this->variable = node();	
	this->delay = -1;
	this->schedState = false;
}

operation::operation(int start, std::string oper, node out, node in1, node in2, node var, int delay, bool cond) {
	this->lineNum = start;
	this->operType = oper;
	this->output = out;	
	this->input1 = in1;
	this->input2 = in2;	
	this->variable = var;	
	this->delay = delay;
	this->cond = cond;
	this->schedState = false;
}

// Getters
int operation::getLineNum() { return this->lineNum; }
std::string operation::getOperType() { return this->operType; }
node operation::getInput1() { return this->input1; }
node operation::getInput2() { return this->input2; }
node operation::getOutput() { return this->output; }
node operation::getMuxSel() { return this->variable; }
int operation::getDelay() { return this->delay; }
bool operation::getConditional() { return this->cond; }
bool operation::getSchedState() { return this->schedState; }

// Setters. Probably won't see use but declared for consistency sake.
void operation::setLineNum(int start) { this->lineNum = start; }
void operation::setOperType(std::string type) { this->operType = type; }
void operation::setInput1(node in1) { this->input1 = in1; }
void operation::setInput2(node in2) { this->input2 = in2; }
void operation::setOutput(node out) { this->output = out; }
void operation::setMuxSel(node sel) { this->variable = sel; }
void operation::setDelay(int num) { this->delay = num; }
void operation::setConditional(bool cond) { this->cond = cond; }
void operation::setSchedState(bool s) { this->schedState = s; }