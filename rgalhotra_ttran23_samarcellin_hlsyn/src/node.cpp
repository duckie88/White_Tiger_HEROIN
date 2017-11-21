	
#include "node.h"

node::node(){
	return; //default constructor
}

node::node(int nodeNum){
	this->nodeNum = nodeNum;
	visited = 'w';
	delay = 0;
	asapTime = -1;
	alapTime = -1;
	cyclesElapsed = 0;
	scheduled = false;
	conditional = false;
}

std::string node::getOperation(){
	return this->operation;
}

void node::setOperation(std::string operation){
	this->operation = operation;
	return;
}

int node::getNodeNum(){
	return nodeNum;
}

void node::setNodeNum(int nodeNum){
	this->nodeNum = nodeNum;
	return;
}

std::vector<node*> node::getPrevNodes(){
	return this->prevNodes;
}

void node::setPrevNodes(std::vector<node*> prevNodes){
	this->prevNodes = prevNodes;
	return;
}

std::vector<node*> node::getNextNodes(){
	return this->nextNodes;
}

void node::setNextNodes(std::vector<node*> nextNodes){
	this->nextNodes = nextNodes;
	return;
}

std::vector<node*> node::getNextIfNodes(){
	return this->nextIfNodes;
}

void node::setNextIfNodes(std::vector<node*> nextIfNodes){
	this->nextIfNodes = nextIfNodes;
	return;
}

std::vector<node*> node::getNextElseNodes(){
	return this->nextElseNodes;
}

void node::setNextElseNodes(std::vector<node*> nextElseNodes){
	this->nextElseNodes = nextElseNodes;
	return;
}

char node::getVisited(){
	return this->visited;
}

void node::setVisited(char visited){
	this->visited = visited;
	return;
}

int node::getDelay(){
	return this->delay;
}

void node::setDelay(int delay){
	this->delay = delay;
	return;
}

int node::getAsapTime(){
	return this->asapTime;
}

void node::setAsapTime(int asapTime){
	this->asapTime = asapTime;
	return;
}

int node::getAlapTime(){
	return this->alapTime;
}

void node::setAlapTime(int alapTime){
	this->alapTime = alapTime;
	return;
}

bool node::getScheduled(){
	return this->scheduled;
}

void node::setScheduled(bool scheduled){
	this->scheduled = scheduled;
	return;
}

bool node::getConditional(){
	return this->conditional;
}

void node::setConditional(bool conditional){
	this->conditional = conditional;
	return;
}

void node::addPrevNode(node* temp){
	prevNodes.push_back(temp);
	return;
}

void node::addNextNode(node* temp){
	nextNodes.push_back(temp);
	return;
}

int node::getCyclesElapsed(){
	return this->cyclesElapsed;
}

void node::setCyclesElapsed(int cycles){
	this->cyclesElapsed = cycles;
	return;
}