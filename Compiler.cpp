#include "Compiler.h"

Compiler::Compiler(Map* map)
{
	this->variableManager = new VariableManager();
	this->functionManager = new FunctionManager(variableManager, this, map);
	this->calculator = new Calculator(variableManager, functionManager, this);
	this->map = map;

	log = new Logs();

	//s¹ 4 typy danych: liczba, rzeczywista, znak, logiczny
	//je¿eli dodasz globalny przed typ, zmienna jest globalna (nie usuwa siê pod koniec kodu)
	//z ka¿dej zmiennej mo¿na zrobiæ tablicê, na górze jest przyk³ad

	//mo¿esz wywo³ywaæ funkcjê, tutaj test[8]
	//w pliku FunctionManager.cpp mo¿esz dodaæ funkcjê w konstruktorze
	//je¿eli dodajesz funkcje napisz jej treœæ w Functions.cpp i zadeklaruj j¹ w Functions.h
	//w cpp masz przyk³ad

	//dodatkowo masz 2 instrukcje jezeli oraz dopoki


	//je¿eli kompiluje siê inny kod, nale¿y ustawiæ przed tym zmienn¹ checked na false
	//je¿eli siê tego nie ustawi, program nie sprawdza poprawnoœci kodu
}

Compiler::~Compiler()
{
	delete variableManager;
	delete calculator;
	delete functionManager;

	if (log != nullptr)
		delete log;
}

void Compiler::runCode(vector<string> lines)
{
	unordered_map<int, int> whileLoops; //saves places to go back in loops
	if (log != nullptr)
		delete log;
	log = new Logs();


	for (int i = 0; i < lines.size(); i++) {
		if (lines[i].at(0) == '/') //pomijaj komentarze
			continue;

		currentLine = i;
		string firstToken = getFirstToken(lines[i]);

		if (firstToken == "jezeli") { //if command
			bool notFound = false;
			string arg = getArgument(lines[i], &notFound);

			if (notFound) {
				log->pushLog(Logs::ErrorLog(Logs::WARNING, "Brak argumentu klauzuli jezeli", i));
			}

			if (arg == "") {
				log->pushLog(Logs::ErrorLog(Logs::WARNING, "Pusty Argument", currentLine));
				arg = "0";
			}

			string value = calculator->calculate(arg); //calculates the true or false value

			int end = getIndexAfterInstruction(lines, i + 1); // go into the line after the if command

			if (end == -1) {
				log->pushLog(Logs::ErrorLog(Logs::LERROR, "Nie znaleziono ciala klauzuli", i));
			}
			else if (stod(value) != 1) { //if false
				i = end;
			}
		}
		else if (firstToken == "dopoki") { //while loop
			bool notFound = false;
			string arg = getArgument(lines[i], &notFound);

			if (arg == "") {
				log->pushLog(Logs::ErrorLog(Logs::WARNING, "Pusty Argument", currentLine));
				arg = "0";
			}

			if (notFound) {
				log->pushLog(Logs::ErrorLog(Logs::WARNING, "Brak argumentu klauzuli dopoki", i));
			}

			string value = calculator->calculate(arg); //calculate the true or false
			int index = getIndexAfterInstruction(lines, i + 1); // index of line after the while loop

			if (index == -1) {
				log->pushLog(Logs::ErrorLog(Logs::LERROR, "Nie znaleziono ciala klauzuli", i));
			}

			if (stod(value) == 1) { //if true
				whileLoops[index] = i; //saves index to go back
			}
			else {
				if (whileLoops.count(index)) { //remove index
					whileLoops.erase(index);
				}

				i = index;
			}
		}
		else if (firstToken == "}") { //end of a block
			if (whileLoops.count(i)) { //if end of a while loop
				i = whileLoops[i] - 1; //move to the beginnign of the while loop
			}
		}
		else if (firstToken == "{") {

		}
		else {
			runLine(lines[i], i); //normal command
		}

		if (log->haveLogs()) {
			log->printLogs();
			if (log->haveError()) {
				map->error = true;
				break;
			}
			else {
				log->reset();
			}
		}
	}

	checked = true;
	restart();
}

//takes the line with a function and runs it
float Compiler::runFunction(string line)
{
	vector<string> args = getArgs(line);
	return functionManager->runFunction(getFirstToken(line), calculateArgs(args), args);
}

//takes a line with a command and runs it
void Compiler::runLine(string line, int index)
{
	string token = getFirstToken(line);

	if (functionManager->isFunction(token)) {
		runFunction(line);
	}
	else
		addVariable(line, index);
}

void Compiler::addVariable(string variable, int index)
{
	string token = getFirstToken(variable);

	string line = variable;
	bool global = false;

	if (token == "globalny") { //checks for a global variable
		line = variable.substr(variable.find(' ') + 1, variable.size()); //crop the globalny
		token = getFirstToken(line);
		global = true;
	}

	int equal = line.find('=');
	int space = line.find(' ');

	if (equal == string::npos || space == string::npos || space >= equal) {
		log->pushLog(Logs::ErrorLog(Logs::LERROR, "Nierozpoznana funkcja", index));
		return;
	}

	string value = line.substr(equal + 1, line.size()); //after the equal sign
	string name = RemoveSpaces(line.substr(space + 1, equal - space - 1)); //name of the variable

	int at = stoi(calculator->calculate(getArgument(name))); //index in the array

	if (at == -1) //index not found , it is not an array
		at = 0;

	name = getFirstToken(name); //remove the array from the name, a[1] -> a

	if (token == "liczba") { //int
		Int* v = dynamic_cast<Int*>(global ? variableManager->getGlobalVariable(name) : variableManager->getLocalVariable(name));
		int result = stod(calculator->calculate(RemoveSpaces(value)));

		if (v) { //variable already exist
			if (at == 0) { //if first element of the array or normal variable
				v->value = result;
			}
			else {
				v->set(at, new Int(result)); //set the element of the array
			}
		}
		else { //variable not exist
			Int* var = new Int(result);

			if (global)
				variableManager->AddGlobalVariable(var, name);
			else
				variableManager->AddVariable(var, name);
		}
	}
	else if (token == "rzeczywista") { //float
		Float* v = dynamic_cast<Float*>(global ? variableManager->getGlobalVariable(name) : variableManager->getLocalVariable(name));
		float result = stod(calculator->calculate(RemoveSpaces(value)));

		if (v) {//variable already exist
			if (at == 0) { //if first element of the array or normal variable
				v->value = result;
			}
			else {
				v->set(at, new Float(result)); //set the element of the array
			}
		}
		else { //variable not exist
			Float* var = new Float(result);

			if (global)
				variableManager->AddGlobalVariable(var, name);
			else
				variableManager->AddVariable(var, name);
		}
	}
	else if (token == "znak") {
		Char* v = dynamic_cast<Char*>(global ? variableManager->getGlobalVariable(name) : variableManager->getLocalVariable(name));
		char result = stod(calculator->calculate(RemoveSpaces(value)));

		if (v) {
			if (at == 0) { //if first element of the array or normal variable
				v->value = result;
			}
			else {
				v->set(at, new Char(result)); //set the element of the array
			}
		}
		else { //variable not exist
			Char* var = new Char(result);

			if (global)
				variableManager->AddGlobalVariable(var, name);
			else
				variableManager->AddVariable(var, name);
		}
	}
	else if (token == "logiczny") {
		Bool* v = dynamic_cast<Bool*>(global ? variableManager->getGlobalVariable(name) : variableManager->getLocalVariable(name));
		double result = stod(calculator->calculate(RemoveSpaces(value)));

		if (v) {
			if (at == 0) { //if first element of the array or normal variable
				v->value = result == 1;
			}
			else {
				v->set(at, new Bool(result == 1)); //set the element of the array
			}
		}
		else { //variable not exist
			Bool* var = new Bool(result == 1);

			if (global)
				variableManager->AddGlobalVariable(var, name);
			else
				variableManager->AddVariable(var, name);
		}
	}
	else {
		log->pushLog(Logs::ErrorLog(Logs::LERROR, "Nierozpoznana funkcja", index));
	}
}

//return a string without every space ' '
string Compiler::RemoveSpaces(string s)
{
	string result;
	for (char c : s) {
		if (c != ' ') {
			result += c;
		}
	}
	return result;
}

//returns a substring from the beggining to the first character in afterFirstToken
string Compiler::getFirstToken(string line)
{
	if (line.length() == 0) //return empty string
		return line;

	for (int i = 0; i < line.length(); i++) {
		if (isCharIn(line.at(i), afterFirstToken))
			return line.substr(0, i);
	}

	return line;
}

//return inside of the first parrenthesies
string Compiler::getArgument(string line, bool* notFound)
{
	int openingBrackets = 0; //how many openings
	int startIndex = -1; //first opening
	for (int i = 0; i < line.length(); i++) {
		char token = line.at(i);
		if (token == '(' || token == '[') {
			openingBrackets++; //add opening

			if (startIndex == -1) //set the first one
				startIndex = i;
		}
		else if (token == ')' || token == ']') {
			openingBrackets--; //close one opening 

			if (openingBrackets == 0) { //everything is closed
				string res = line.substr(startIndex + 1, i - startIndex - 1);

				return res;
			}
		}
	}

	if (notFound != nullptr) {
		*notFound = true;
	}

	return "-1"; //argument not found

}

Logs* Compiler::getLogs()
{
	return log;
}

void Compiler::restart()
{
	variableManager->clearLocalVariables();
	currentLine = 0;
}

vector<string> Compiler::getArgs(string line) //return calculated numbers inside the argument
{
	vector<string> args;
	string s = getArgument(line);

	int start = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s.at(i) == '[') {
			int par = 1;
			i++;
			while (par > 0) {
				if (s.at(i) == '[')
					par++;
				else if (s.at(i) == ']')
					par--;
				i++;
			}
			i--;
		}
		else if (s.at(i) == ',') {
			args.push_back(s.substr(start, i - start));
			start = i + 1;
		}
	}
	if (s.size() - start > 0)
		args.push_back(s.substr(start, s.size() - start));
	return args;
}

vector<float> Compiler::calculateArgs(vector<string> arguments) //change all arguments into floats
{
	vector<float> args;

	for (string s : arguments) {
		args.push_back(stod(calculator->calculate(s)));
	}

	return args;
}

bool Compiler::isCharIn(char c, const vector<char> arr) //checks if char c is in vector arr
{
	for (int i = 0; i < arr.size(); i++) {
		if (c == arr[i])
			return true;
	}

	return false;
}


//returns index of closing of the open parenthesis provided at the index start
int Compiler::getIndexAfterInstruction(vector<string> lines, int start)
{
	int openings = 0;

	if (lines[start] != "{")
		return -1;

	for (int i = start; i < lines.size(); i++) {
		if (lines[i].size() == 0)
			continue;

		if (lines[i].at(0) == '{') {
			openings++;
		}
		else if (lines[i].at(0) == '}') {
			openings--;

			if (openings == 0)
				return i;
		}
	}

	return -1;
}


