#pragma once

#include "VariableManager.h"
#include "ShuntingYardAlgorithm.h"
#include "FunctionManager.h"
#include "ErrorLogs.h"

#include <sstream>

class Compiler {
public:
	Compiler(Map* map);
	~Compiler();

	void runCode(vector<string> lines);
	float runFunction(string s);

	string getFirstToken(string line);
	string getArgument(string line, bool* notFound = nullptr);

	Logs* getLogs();

	void restart();

	int currentLine = 0;
	bool checked = false;
private:
	void runLine(string line, int index);
	void addVariable(string variable, int index);

	//helpers
	string RemoveSpaces(string s);
	vector<string> getArgs(string line);
	vector<float> calculateArgs(vector<string> arguments);
	bool isCharIn(char c, const vector<char> arr);
	int getIndexAfterInstruction(vector<string> lines, int i);

	VariableManager* variableManager;
	Calculator* calculator;
	FunctionManager* functionManager;
	Logs* log;
	Map* map;
};

const vector<char> afterFirstToken = { ' ', '(', '=', '[' };
