#include <vector>
#include <string>

#include "conditional.h"

conditional::conditional() {
	this->nameType = "";	
	this->lineNum = -1;	
	this->condition = "";
}

conditional::conditional(std::string nameType, int num, std::string condition) {
	this->nameType = nameType;
	this->lineNum = num;
	this->condition = condition;
}

// Getters
std::string conditional::getName() { return this->nameType; }
int conditional::getLineNum() { return this->lineNum; }
std::string conditional::getCondition() { return this->condition; }

// Setters. Probably won't see use but declared for consistency sake.
void conditional::setName(std::string str) { this->nameType = str; }
void conditional::setLineNum(int start) { this->lineNum = start; }
void conditional::setCondition(std::string str) { this->condition = str; }