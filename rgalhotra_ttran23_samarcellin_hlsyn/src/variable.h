#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <vector>

class variable {
public:
	variable(); //default constructor
	variable(std::string type, std::string name, bool SIGN, int dataSize);
	void setName(std::string name);
	std::string getName();
	void setSIGN(bool SIGN);
	bool getSIGN();
	void setDataSize(int dataSize);
	int getDataSize();
	void setType(std::string type);
	std::string getType();
private:
	std::string type;
	std::string name;
	bool SIGN;
	int dataSize;
};

#endif