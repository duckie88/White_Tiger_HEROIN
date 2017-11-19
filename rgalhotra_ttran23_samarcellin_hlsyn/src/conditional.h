#ifndef CONDITIONAL_H
#define CONDITIONAL_H

#include "node.h"	// Has <string> and <vector>

class conditional {
private:
	std::string nameType;	// Name of line, if/else/for
	int lineNum;	// When the if starts
	std::string condition;
public:
	// Constructors
	conditional();
	conditional(std::string nameType, int num, std::string condition);

	// Methods

	// Getters
	std::string getName();
	int getLineNum();
	std::string getCondition();

	// Setters. Probably won't see use but declared for consistency sake.
	void setName(std::string str);
	void setLineNum(int start);
	void setCondition(std::string str);
};

#endif //CONDITIONAL_H