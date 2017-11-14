#include <vector>
#include <string>

#include "node.h"


node::node() {
	return;
}

node::node(std::string type, std::string name, bool SIGN, int dataSize) {
	this->type = type;
	this->name = name;
	this->SIGN = SIGN;
	this->dataSize = dataSize;
	return;
}

void node::setType(std::string type) {
	this->type = type;
	return;
}

std::string node::getType() {
	return this->type;
}

void node::setName(std::string name) {
	this->name = name;
	return;
}

std::string node::getName() {
	return this->name;
}

void node::setSIGN(bool SIGN) {
	this->SIGN = SIGN;
	return;
}

bool node::getSIGN() {
	return this->SIGN;
}

void node::setDataSize(int dataSize) {
	this->dataSize = dataSize;
	return;
}

int node::getDataSize() {
	return this->dataSize;
}

void node::addEdge(node* nextNode) {
	this->edges.push_back(nextNode);
	return;
}

std::vector<node*> node::getEdges() {
	return this->edges;
}

void node::addWeight(int weight) {
	this->weights.push_back(weight);
	return;
}

std::vector<int> node::getWeights() {
	return this->weights;
}