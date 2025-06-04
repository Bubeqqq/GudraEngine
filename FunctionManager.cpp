#include "FunctionManager.h"
#include "Functions.h"

#include "Compiler.h"

FunctionManager::FunctionManager(VariableManager* variableManager, Compiler* compiler, Map* map)
{
	this->functionSet = new Functions(variableManager, map);
	this->compiler = compiler;

	//addFunction("TAK JAK SIÊ MA J¥ WYWO£YWAÆ", &Functions::TAK JAK SIE NAZYWA FUNCKCJA W OBIEKCIE)
	addFunction("klawisz", &Functions::isKeyPressed, 1);
	addFunction("test", &Functions::testowa, 2);
	addFunction("ustawna", &Functions::setAt, 3);
	addFunction("zakoncz", &Functions::end, 0);
	addFunction("wyczysc", &Functions::clear, 0);
	addFunction("ustawfps", &Functions::setFPS, 1);
	addFunction("ustawwielkoscmapy", &Functions::setMapSize, 2);
	addFunction("znakna", &Functions::getAt, 2);
	addFunction("linia", &Functions::drawLine, 5);
	addFunction("wstaw", &Functions::insert, 3);
	addFunction("usun", &Functions::remove, 2);
	addFunction("rozmiar", &Functions::getVariableSize, 1);
	addFunction("losowa", &Functions::random, 2);
	addFunction("wyswietlliczbe", &Functions::printInt, 3);
}

FunctionManager::~FunctionManager()
{
	delete functionSet;
}

void FunctionManager::addFunction(string name, func f, int x) //adds function to the dictionary of the compiler
{
	functions[name] = f;
	functionArgumentCount[name] = x;
}

float FunctionManager::runFunction(string name, vector<float> args, vector<string> names)
{
	if (functionArgumentCount[name] != args.size()) {
		compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Nieodpowiednia liczba Argumentow", compiler->currentLine));
		return -1;
	}

	return (functionSet->*functions[name])(args, names);
}

bool FunctionManager::isFunction(string name)
{
	return functions.count(name);
}
