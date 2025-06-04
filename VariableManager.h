#pragma once

#include <unordered_map>
#include <string>
#include <iostream>;

using namespace std;

enum VariableTypes {
	GINT, GFLOAT, GCHAR, GBOOL
};

class Variable {
public:
	virtual VariableTypes getType() = 0;
	virtual void print() = 0;

	~Variable();

	//array

	int size = 1;
	bool destroyAll = false;

	Variable* next = nullptr;

	Variable* getValue(int i);
	void deleteAt(int i);
	void deleteAll();
	void printAll();
	void insert(int i, Variable* variable);
	void set(int i, Variable* value);
	Variable* getVariable(VariableTypes vt, float number);
};

class Int : public Variable {
public:
	int value;

	Int(int val) : value(val) {};

	void print() { cout << value; };

	VariableTypes getType()
	{
		return VariableTypes::GINT;
	}
};

class Char : public Variable {
public:
	char value;

	Char(char val) : value(val) {};

	void print() { cout << value; };

	VariableTypes getType()
	{
		return VariableTypes::GCHAR;
	}
};

class Float : public Variable {
public:
	float value;

	Float(float val) : value(val) {};

	void print() { cout << value; };

	VariableTypes getType()
	{
		return VariableTypes::GFLOAT;
	}
};

class Bool : public Variable {
public:
	bool value = false;

	Bool(bool val) : value(val) {};

	void print() { cout << (value ? "prawda" : "falsz"); };

	VariableTypes getType()
	{
		return VariableTypes::GBOOL;
	}
};

class VariableManager
{
public:
	VariableManager();
	~VariableManager();

	void showAllVariables();

	//add variables

	void AddVariable(Variable* var, string name);

	void AddGlobalVariable(Variable* var, string name);

	Variable* getVariable(string name);

	Variable* getGlobalVariable(string name);

	Variable* getLocalVariable(string name);

	bool isVariable(string name);

	void clearLocalVariables();
private:
	//normal
	unordered_map<string, Variable*> variables;
	//global
	unordered_map<string, Variable*> globalVariables;
};