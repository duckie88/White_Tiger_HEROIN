#include <vector>
#include <string>

#include "conditional.h"

conditional::conditional() {
	this->nameType = "";	
	this->lineNum = -1;	
	this->condition = "";
}

conditional::conditional(std::string nameType, int num, std::string condition, bool nested) {
	this->nameType = nameType;
	this->lineNum = num;
	this->condition = condition;
	this->nested = nested;
}

// Getters
std::string conditional::getName() { return this->nameType; }
int conditional::getLineNum() { return this->lineNum; }
std::string conditional::getCondition() { return this->condition; }
bool conditional::getNested() { return this->nested; }

// Setters. Probably won't see use but declared for consistency sake.
void conditional::setName(std::string str) { this->nameType = str; }
void conditional::setLineNum(int start) { this->lineNum = start; }
void conditional::setCondition(std::string str) { this->condition = str; }
void conditional::setNested(bool nested) { this->nested = nested; }