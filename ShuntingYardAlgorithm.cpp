#include "ShuntingYardAlgorithm.h"

#include "math.h"
#include "Load.h"
#include "Help.h"

#include "Compiler.h"

Calculator::Calculator(VariableManager* variableManager, FunctionManager* functionManager, Compiler* compiler)
{
    this->variableManager = variableManager;
    this->functionManager = functionManager;
    this->compiler = compiler;

    OperationPriority["("] = 0;
    OperationPriority[")"] = 0;
    OperationPriority["["] = 0;
    OperationPriority["]"] = 0;
    OperationPriority["{"] = 0;
    OperationPriority["}"] = 0;
    addFunction("+", 2, sum);
    addFunction("-", 2, substracion);
    addFunction("*", 3, multiply);
    addFunction("/", 3, divide);
    addFunction("^", 4, powC);
    addFunction("sin", 5, sinC);
    addFunction("cos", 5, cosC);
    addFunction("tan", 5, tgC);
    addFunction("cot", 5, cotC);
    addFunction("arcsin", 5, arcsinC);
    addFunction("arccos", 5, arccosC);
    addFunction("arctan", 5, arctgC);
    addFunction("arccot", 5, arccotC);
    addFunction("ln", 5, LogarithmLN);
    addFunction("log", 5, Logarithm);
    addFunction("abs", 5, absC);
    addFunction("sqrt", 5, squareRoot);
    addFunction("cbrt", 5, cubeRoot);
    addFunction("-sin", 5, MsinC);
    addFunction("-cos", 5, McosC);
    addFunction("-tan", 5, MtgC);
    addFunction("-cot", 5, McotC);
    addFunction("-arcsin", 5, MarcsinC);
    addFunction("-arccos", 5, MarccosC);
    addFunction("-arctan", 5, MarctgC);
    addFunction("-arccot", 5, MarccotC);
    addFunction("-ln", 5, MLogarithmLN);
    addFunction("-log", 5, MLogarithm);
    addFunction("-abs", 5, MabsC);
    addFunction("-sqrt", 5, MsquareRoot);
    addFunction("-cbrt", 5, McubeRoot);

    //logic

    addFunction("oraz", 3, And);
    addFunction("lub", 3, Or);
    addFunction("==", 3, Equal);
    addFunction("!=", 3, NotEqual);
    addFunction("nie", 5, Not);
    addFunction("-nie", 5, MNot);
    addFunction(">", 3, More);
    addFunction("<", 3, Less);
    addFunction(">=", 3, MoreOrEqual);
    addFunction("<=", 3, LessOrEqual);
}

void Calculator::addFunction(string sign, int priority, bool (*func)(string a, string b, string op, stack<string>& queue)) {
    OperationPriority[sign] = priority;
    OperationFunctions[sign] = func;
}

bool Calculator::isOperation(string s) {
    return OperationPriority.count(s);
}

void Calculator::checkForDoubleMinus(stack<string>& operation) {
    stack<string> checked;

    string last = "";

    while (!operation.empty()) {
        if (last == "-" && operation.top() == "-") { //if there are 2 minuses next to each other
            checked.pop(); //delete last minus
            checked.push("+"); //add + instead of 2 minuses
            last = "+";
            operation.pop();
        }
        else {
            last = operation.top(); //update the last word
            checked.push(operation.top()); //port to new
            operation.pop();
        }
    }

    operation = checked;
}

void Calculator::sortMinuses(stack<string>& st) { //Assigns minuses to numbers, e.g. “2”, “+”, “-”, “x” -> “2”, “+”, “-x”
    stack<string> result;

    reverseStack(st);

    short last = -1; //status of the last word
    int index = 0; //current word number
    while (!st.empty()) {
        short state = -1;

        char token = st.top().at(0);

        if (isNumber(token)) //determine the type of the current expression
            state = NUMBER;
        else if (isLetter(token))
            state = LETTER;
        else if (token == '(' || token == '[' || token == '{') {
            state = BRACKET;
        }

        if (token == '-' && st.top().size() == 1) {
            if (last == BRACKET || index == 0) { // - is after the parenthesis or at the beginning of the expression
                st.pop(); //remove the minus

                if (!st.empty()) { //after the minus is something else
                    if (isNumber(st.top().at(0)) || isLetter(st.top().at(0))) { //after the minus is a number or a word
                        string s = "-"; //add a minus to a number, or unknown
                        s.append(st.top());
                        st.pop();
                        result.push(s);

                        index++;
                        continue;
                    }
                    else { //after the minus is something else, such as a parenthesis
                        result.push("-");
                    }
                }
                else { //the minus is at the end
                    result.push("-");
                }
            }
        }

        last = state;
        result.push(st.top());
        st.pop();
        index++;
    }

    st = result;
}

bool Calculator::replaceVariablesWithNumbers(stack<string>& operation) {
    stack<string> result;

    while (!operation.empty()) {
        string s = operation.top().substr(0, operation.top().find('['));

        if (functionManager->isFunction(s)) { //if token is a funtion, push the result of the function
            result.push(to_string(compiler->runFunction(operation.top())));
        }
        else if (s == "prawda") {
            result.push("1");
        }
        else if (s == "falsz") {
            result.push("0");
        }
        else if (isLetter(s.at(0)) && !OperationFunctions.count(s)) { //token is a variable
            int at = stoi(calculate(compiler->getArgument(operation.top()))); //index in the array
            if (at == -1) //variable or first index in the array
                at = 0;
            Variable* v = variableManager->getVariable(s); //get first value
            if (v == nullptr) {
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Nie znaleziono zmiennej", compiler->currentLine));
                return false;
            }

            if (at >= v->size) {
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Poza zakresem Funkcji", compiler->currentLine));
                return false;
            }

            v = v->getValue(at); //get value in the array

            //push value of the variable
            if (v->getType() == VariableTypes::GFLOAT) {
                Float* value = dynamic_cast<Float*>(v);
                result.push(to_string(value->value));
            }
            else if (v->getType() == VariableTypes::GINT) {
                Int* value = dynamic_cast<Int*>(v);
                result.push(to_string(value->value));
            }
            else if (v->getType() == VariableTypes::GCHAR) {
                Char* value = dynamic_cast<Char*>(v);
                result.push(to_string((int)value->value));
            }
            else if (v->getType() == VariableTypes::GBOOL) {
                Bool* value = dynamic_cast<Bool*>(v);
                value->value ? result.push("1") : result.push("0");
            }
            else {
                //----------------------------------------- B³¹d
            }
        }
        else if (s.at(0) == '\'') { //token is a char
            if (s.size() == 1) {
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Nierozpoznano znaku", compiler->currentLine));
                return false;
            }

            result.push(to_string((int)s.at(1))); //return the integer representation of the char
        }
        else { //normal numbers
            result.push(s);
        }

        operation.pop();
    }

    reverseStack(result);
    operation = result;

    return true;
}

bool Calculator::ShuntingYardAlgorithm(stack<string>& operation) {
    stack<string> queue, result; //shunting yard algorithm

    if (!replaceVariablesWithNumbers(operation)) {
        return false;
    }

    if (operation.size() == 1) {
        return true;
    }

    reverseStack(operation);
    sortMinuses(operation);
    reverseStack(operation);

    if (!compiler->checked) {
        if (!check(operation)) {
            return false;
        }
    }

    while (!operation.empty()) { //for each expression in action
        string token = operation.top(); //expression currently in process

        if (token == "(" || token == "[" || token == "{") { //token is the beginning of the parenthesis
            queue.push(token); //add to queue
        }
        else if (token == ")" || token == "]" || token == "}") { //token is the end of the parenthesis
            string topOfQueue = queue.top();

            while (topOfQueue != "(" && topOfQueue != "[" && topOfQueue != "{") { //move expressions from the queue until it encounters the beginning of the parenthesis
                result.push(topOfQueue);
                queue.pop();

                topOfQueue = queue.top();
            }

            queue.pop(); //remove the beginning of the parenthesis from the queue
        }
        else if (isOperation(token)) { //if the token is a mathematical operation
            if (queue.empty() || OperationPriority[token] > OperationPriority[queue.top()]) { //je¿eli stack pomocniczy jest pusty, lub priorytet ostatniego wyrazu w kolejce jest ni¿szy
                queue.push(token); //dodaj token do kolejki
            }
            else {
                while (!queue.empty()) { //move all tokens from the queue that have a lower priority to the result
                    if (OperationPriority[queue.top()] < OperationPriority[token])
                        break;

                    result.push(queue.top());
                    queue.pop();
                }

                queue.push(token);//add the token to the queue
            }
        }
        else { //token is a number
            result.push(token); //add to the result
        }

        operation.pop();
    }

    while (!queue.empty()) { //when finished, add the queue to the result
        result.push(queue.top());
        queue.pop();
    }

    checkForDoubleMinus(result);

    operation = result;

    return true;
}

bool Calculator::doMath(string a, string b, string op, stack<string>& queue) {
    return OperationFunctions[op](a, b, op, queue);
}

string Calculator::calculateAnswer(stack<string>& postfix) {
    stack<string> queue, operation = postfix;

    if (postfix.size() == 1)
        return postfix.top();

    while (!operation.empty()) {
        string token = operation.top();

        if (isOperation(token)) { //if token is an arithmetic operation
            string a = queue.top(); //get 2 numbers from the queue
            queue.pop();

            string b = queue.empty() ? "buf" : queue.top(); //if the stack is empty, it means that the action requires only 1 number, so the 2nd number is replaced by a temporary “buf”

            if (b != "buf") { //if there is only 1 number at the action, replace the 2nd number with a temporary “buf”
                queue.pop();
            }

            if (!doMath(a, b, token, queue)) {
                return "not";
            }
        }
        else { //token is a number
            queue.push(token); //add to queue
        }

        operation.pop();
    }

    return queue.top(); //the only number in the queue is the result
}

bool Calculator::check(stack<string> stack) {
    vector<string> list;

    while (!stack.empty()) { //replace stack with vector
        list.push_back(stack.top());
        stack.pop();
    }

    for (int i = 0; i < list.size(); i++) {
        string word = list.at(i);

        if (isNumber(word)) //number  does not display error
            continue;

        if (isWord(word)) { //funkcjê (sin, cos, arcsin)
            if (!isOperation(word)) { //misspelling (sim, cosz)
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Nieznana funkcja", compiler->currentLine));
                //printError(list, "", "is not a known function", i, i);
                return false;
            }

            if (i == list.size() - 1 || !isOpenBracket(list.at(i + 1))) { // no parenthesis after the function ( sinx), cos2 )
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Brak nawiasow po funkcji", compiler->currentLine));
                //printError(list, "after", "the bracket was expected to open", i, i + 1);
                return false;
            }
        }
        else if (isOpenBracket(word)) { //otwarty nawias
            int brackets = 0;

            if (i == list.size() - 1 || isClosedBracket(list.at(i + 1))) { //There is no number ( (), (())) after the parenthesis )
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Nieprawidlowy nawias", compiler->currentLine));
                //printError(list, "after", "the number expected", i, i + 1);
                return false;
            }

            for (int j = i; j < list.size(); j++) { //look for the end of the parenthesis ( sin(x + 3, ((x+2)+3 ))
                if (isOpenBracket(list.at(j))) //open parenthesis found
                    brackets++;
                else if (isClosedBracket(list.at(j))) //closed parenthesis found
                    brackets--;

                if (brackets == 0) //end of parenthesis found
                    break;
            }

            if (brackets != 0) { //no end found
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Nieznaleziono zakonczenia nawiasu", compiler->currentLine));
                //printError(list, "", "bracket closure not found", i, i);
                return false;
            }
        }
        else if (isClosedBracket(word)) { //end of bracket
            int brackets = 0;

            for (int j = i; j >= 0; j--) { //look for the start of the parenthesis ( )(x+3) )
                if (isOpenBracket(list.at(j)))
                    brackets++;
                else if (isClosedBracket(list.at(j)))
                    brackets--;

                if (brackets == 0)
                    break;
            }

            if (brackets != 0) {
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Nieznaleziono otwarcia nawiasu", compiler->currentLine));
                //printError(list, "", "bracket opening not found", i, i);
                return false;
            }
        }
        else { //operations such as +, -, *
            if (!isOperation(word)) { //no such operation found (+-, &, $)
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Nierozpoznano znaku", compiler->currentLine));
                //printError(list, "", "is not a valid arithmetic operation", i, i);
                return false;
            }

            if (word.at(0) != '-') { //before - does not have to be a number
                if (i == 0) { //operation is in the early stages
                    compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Niepoprawne zastosowanie znaku", compiler->currentLine));
                    //printError(list, "before", "the number expected", i, i - 1); //errors such as +3, *5+8
                    return false;
                }
            }

            if (i == list.size() - 1) { //operation is at the end of the action 5+, 7*8*
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Niepoprawne zastosowanie znaku", compiler->currentLine));
                //printError(list, "after", "the number expected", i, i + 1);
                return false;
            }

            if (word.at(0) != '-') {
                if (!isClosedBracket(list.at(i - 1)) && !isNumber(list.at(i - 1))) { //before the operation, there is no number
                    compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Niepoprawne zastosowanie znaku", compiler->currentLine));
                    //printError(list, "before", "the number expected", i, i - 1);
                    return false;
                }
            }

            if (!isOpenBracket(list.at(i + 1)) && !isNumber(list.at(i + 1)) && !isWord(list.at(i + 1))) { //after the operation there is no number
                compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Niepoprawne zastosowanie znaku", compiler->currentLine));
                //printError(list, "after", "the number expected", i, i + 1);
                return false;
            }
        }
    }
    return true;
}

string Calculator::calculate(string func) {
    if (func == "-1")
        return "-1";

    if (func == "") {
        compiler->getLogs()->pushLog(Logs::ErrorLog(Logs::LERROR, "Brak formuly", compiler->currentLine));
        return "-1";
    }

    stack<string> operation;
    loadAsStack(func, operation);

    reverseStack(operation);

    //printStack(operation);

    if (!ShuntingYardAlgorithm(operation)) {
        return "-1";
    }

    //return "";
    return calculateAnswer(operation); //return the calculated value
}
