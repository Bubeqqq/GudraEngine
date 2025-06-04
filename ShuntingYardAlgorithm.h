#pragma once

#include <string>
#include <stack>
#include "VariableManager.h"
#include "FunctionManager.h"

using namespace std;

class Compiler;

class Calculator {
public:
    Calculator(VariableManager* variableManager, FunctionManager* functionManager, Compiler* compiler);

    string calculate(string func);

private:
    VariableManager* variableManager;
    FunctionManager* functionManager;
    Compiler* compiler;

    typedef bool (*Operation)(string a, string b, string op, stack<string>& queue);

    unordered_map<string, int> OperationPriority;
    unordered_map<string, Operation> OperationFunctions;

    const short LETTER = 0, NUMBER = 1, ARYTH = 2, BRACKET = 3;

    //functions

    void addFunction(string sign, int priority, bool (*func)(string a, string b, string op, stack<string>& queue));
    bool isOperation(string s);
    bool replaceVariablesWithNumbers(stack<string>& operation);
    void checkForDoubleMinus(stack<string>& operation);
    void sortMinuses(stack<string>& st);
    bool ShuntingYardAlgorithm(stack<string>& operation);
    bool doMath(string a, string b, string op, stack<string>& queue);
    bool check(stack<string> stack);
    string calculateAnswer(stack<string>& postfix);
};