#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "functions.h"

typedef float(Functions::* func)(vector<float> args, vector<string> names);

class Compiler;

class FunctionManager {
public:
	FunctionManager(VariableManager* variableManager, Compiler* compiler, Map* map);
	~FunctionManager();

	void addFunction(string name, func, int);
	float runFunction(string name, vector<float> args, vector<string> names);
	bool isFunction(string name);

private:
	Functions* functionSet;
	Compiler* compiler;

	unordered_map<string, func> functions;
	unordered_map<string, int> functionArgumentCount;
};