#include "variable.h"


variable::variable() {
	return;
}

variable::variable(std::string type, std::string name, bool SIGN, int dataSize) {
	this->type = type;
	this->name = name;
	this->SIGN = SIGN;
	this->dataSize = dataSize;
	return;
}

void variable::setType(std::string type) {
	this->type = type;
	return;
}

std::string variable::getType() {
	return this->type;
}

void variable::setName(std::string name) {
	this->name = name;
	return;
}

std::string variable::getName() {
	return this->name;
}

void variable::setSIGN(bool SIGN) {
	this->SIGN = SIGN;
	return;
}

bool variable::getSIGN() {
	return this->SIGN;
}

void variable::setDataSize(int dataSize) {
	this->dataSize = dataSize;
	return;
}

int variable::getDataSize() {
	return this->dataSize;
}