	
#include "node.h"

node::node(){
	return; //default constructor
}

node::node(int nodeNum){
	this->nodeNum = nodeNum;
	visited = 'w';
	delay = -1;
	asapTime = -1;
	alapTime = -1;
	cyclesElapsed = -1;
	scheduled = false;
	conditional = false;
	ifElseBlock = 0;
}

std::string node::getOperation(){
	return this->operation;
}

void node::setOperation(std::string operation){
	this->operation = operation;
	return;
}

std::string node::getResult(){
	return this->result;
}

void node::setResult(std::string result){
	this->result = result;
	return;
}

std::string  node::getVar1(){
	return this->var1;
}

void node::setVar1(std::string var1){
	this->var1 = var1;
	return;
}

std::string  node::getVar2(){
	return this->var2;
}

void node::setVar2(std::string var2){
	this->var2 = var2;
	return;
}

std::string  node::getVar3(){
	return this->var3;
}

void node::setVar3(std::string var3){
	this->var3 = var3;
	return;
}


float node::getProbability(){
	return this->probability;
}

void node::setProbability(float probability){
	this->probability = probability;
	return;
}

std::vector<double> node::getSelfForce() {
	return this->selfForce;
}

void node::addSelfForce(double val) {
	this->selfForce.push_back(val);
}

void node::setSelfForce(int index, double val) {
	this->selfForce.at(index) = val;
}

std::vector<double> node::getPredForce() {
	return this->predForce;
}

void node::addPredForce(double val) {
	this->predForce.push_back(val);
}

void node::setPredForce(int index, double val) {
	this->predForce.at(index) = val;
}

std::vector<double> node::getSuccForce() {
	return this->succForce;
}

void node::addSuccForce(double val) {
	this->succForce.push_back(val);
}

void node::setSuccForce(int index, double val) {
	this->succForce.at(index) = val;
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

int node::getIfElse(){
	return this->ifElseBlock;
}

void node::setIfElse(int c){
	this->ifElseBlock = c;
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