#include "VariableManager.h"

VariableManager::VariableManager()
{
}

VariableManager::~VariableManager()
{
    for (auto it : variables) {
        variables[it.first]->destroyAll = true;
        delete it.second;
    }

    for (auto it : globalVariables) {
        globalVariables[it.first]->destroyAll = true;
        delete it.second;
    }
}

void VariableManager::showAllVariables() //prints out in the console values of all the variables
{
    cout << "Local: " << endl;

    for (const auto& pair : variables) {
        if (variables[pair.first]->next == nullptr) { //variable
            cout << pair.first << "=";
            pair.second->print();
            cout << endl;
        }
        else { //array
            cout << pair.first;
            variables[pair.first]->printAll();
        }
    }

    cout << "Global: " << endl;

    for (const auto& pair : globalVariables) {
        if (globalVariables[pair.first]->next == nullptr) { //variable
            cout << pair.first << "=";
            pair.second->print();
            cout << endl;
        }
        else { //array
            cout << pair.first;
            globalVariables[pair.first]->printAll();
        }
    }
}

//adding variables

void VariableManager::AddVariable(Variable* var, string name)
{
    variables[name] = var;
}

void VariableManager::AddGlobalVariable(Variable* var, string name)
{
    globalVariables[name] = var;
}

//return variable name
Variable* VariableManager::getVariable(string name)
{
    if (variables.count(name))
        return variables[name];
    else if (globalVariables.count(name)) {
        return globalVariables[name];
    }

    return nullptr;
}

//returns variable name from the global variables
Variable* VariableManager::getGlobalVariable(string name)
{
    if (globalVariables.count(name)) {
        return globalVariables[name];
    }

    return nullptr;
}

//returns variable name from the local variables
Variable* VariableManager::getLocalVariable(string name)
{
    if (variables.count(name))
        return variables[name];

    return nullptr;
}

bool VariableManager::isVariable(string name)
{
    return variables.count(name) || globalVariables.count(name);
}

void VariableManager::clearLocalVariables()
{
    for (const auto& pair : variables) {
        delete(variables[pair.first]);
    }
    variables.clear();
}

//insert variable at the index. Works for only index >= 1
void Variable::insert(int index, Variable* variable)
{
    Variable* next = this;

    for (int i = 0; i < index - 1; i++) {
        next = next->next;
    }

    Variable* afterNext = next->next;

    next->next = variable;
    variable->next = afterNext;

    size++;
}

void Variable::set(int index, Variable* value) //sets the value of the variable at the right index
{
    if (index < size) { //variable already exist
        Variable* previous = this, * next = previous->next;

        for (int i = 0; i < index - 1; i++) {
            previous = next;
            next = previous->next;
        }
        Variable* after = next->next; //exchange variables
        previous->next = value;
        value->next = after;
        if (next != nullptr)
            delete(next);
        return;
    }

    //variable not exist

    Variable* next = this;
    for (int i = 0; i < index - 1; i++) {
        if (next->next == nullptr) { //if the variables between not exist
            next->next = getVariable(value->getType(), 0); //create a empty one
            size++;
        }

        next = next->next;
    }
    size++;
    next->next = value; //insert value at the end
}

//returns variable with a type and a value
Variable* Variable::getVariable(VariableTypes vt, float number)
{
    switch (vt) {
    case GFLOAT:
        return new Float(number);
    case GCHAR:
        return new Char((char)number);
    case GBOOL:
        return new Bool((bool)number);
    default:
        return new Int((int)number);
    }


}

Variable::~Variable()
{
    if (next != nullptr && destroyAll) {
        next->destroyAll = true;
        delete(next);
    }
}

//return variable at the right index
Variable* Variable::getValue(int index)
{
    Variable* next = this;

    for (int i = 0; i < index; i++) {
        next = next->next;
    }

    return next;
}

//removes a variable at the right index. Can't delete a variable at 0
void Variable::deleteAt(int index)
{
    Variable* next = this;

    for (int i = 0; i < index - 1; i++) {
        next = next->next;
    }
    Variable* deleted = next->next;
    next->next = deleted->next;
    delete(deleted);
    size--;
}

//delete every variable, besides the first one
void Variable::deleteAll()
{
    Variable* next = this;

    for (int i = 1; i < size; i++)
        deleteAt(i);

    size = 1;
}

//prints the numbers from the variable to the console
void Variable::printAll()
{
    Variable* next = this;

    cout << " : [";

    for (int i = 0; i < size; i++) {
        next->print();
        if (i < size - 1) cout << ",";
        next = next->next;
    }
    cout << "]" << endl;
}
