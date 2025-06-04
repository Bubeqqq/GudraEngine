#include <string>
#include <stack>
#include <iostream>

#include "Help.h"

using namespace std;

const short LETTER = 0, NUMBER = 1, ARYTH = 2, BRACKET = 3;

void loadAsStack(string op, stack<string>& loadedStack) { //separates the individual expressions (2sin(x) -> “2”, “sin”, “(”, “x”, “)”)
    short state = -1;

    string currentWord = "";

    stack<string> operation;

    for (int i = 0; i < op.length(); i++) {
        char token = op.at(i);

        if (token == ' ') { //ignore spaces
            state = -1;
            continue;
        }

        if (isNumber(token)) { //number
            if (state == NUMBER || currentWord == "") { //previous tokens are numbers
                currentWord.append(string(1, token)); //add to the number
                state = NUMBER;
            }
            else { //earlier tokens are something else
                operation.push(currentWord); //write down the current word and start a new number
                currentWord = string(1, token);
                state = NUMBER;
            }
        }
        else if (isLetter(token)) { //litera
            if (state == LETTER || currentWord == "") {//previous tokens are letters
                currentWord.append(string(1, token));//add to letters
                state = LETTER;

                if (token == '[') { //--------------------------------- Do sprawdzenia
                    int brackets = 1;
                    while (brackets > 0) {
                        token = op.at(++i);
                        if (token == '[') {
                            brackets++;
                        }
                        else if (token == ']') {
                            brackets--;
                        }
                        currentWord.append(string(1, token));
                    }
                    state = -1;
                }
            }
            else {//earlier tokens are something else
                operation.push(currentWord);//write down the current number and start a new word
                currentWord = string(1, token);
                state = LETTER;
            }
        }
        else if (token == '(' || token == ')') { //bracket
            if (currentWord != "") //if the current expression is not empty then save it
                operation.push(currentWord);

            currentWord = string(1, token);

            state = BRACKET;
        }
        else if (token == '\'') {
            if (currentWord != "") { //if the current expression is not empty then save it
                operation.push(currentWord);
                currentWord = "";
            }

            state = -1;

            int count = 0;

            while (count < 2) { //adds everything until the second '\''
                currentWord += string(1, token);

                if (token == '\'')
                    count++;

                if (i + 1 >= op.size())
                    break;

                if (count < 2)
                    token = op.at(++i);
            }
        }
        else { //arithmetic operation
            if (state == ARYTH || currentWord == "") {//previous tokens are operations
                currentWord.append(string(1, token));//add to operation
                state = ARYTH;
            }
            else {//earlier tokens are something else
                operation.push(currentWord);//save the current tokens and start a new arithmetic operation
                currentWord = string(1, token);
                state = ARYTH;
            }
        }
    }

    if (currentWord != "") { //if there is an expression left, save it
        operation.push(currentWord);
    }

    loadedStack = operation;
}