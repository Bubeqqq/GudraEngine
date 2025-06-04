#include "Functions.h"

Functions::Functions(VariableManager* variableManager, Map* map)
{
    this->variableManager = variableManager;
    this->map = map;
    srand(static_cast<unsigned int>(time(nullptr)));
}

float Functions::isKeyPressed(vector<float> args, vector<string> names)
{
    int vkCode = static_cast<int>(args[0]);

    return (GetAsyncKeyState(static_cast<char>(args[0])) & 0x8000) != 0;
}

float Functions::testowa(vector<float> args, vector<string> names)
{
    //cout << "Testowa Funkcja -" << names[1] << "-" << endl;
    /*
        w args s¹ wszystkie parametry ju¿ obliczone, czyli same liczby
        char - wartoœæ liczbowa
        bool jest zapisywany jako 0 albo 1

        w names s¹ parametry nieobliczone,
        czyli je¿eli ktoœ poda liczbê a, to nie podstawi jej wartoœci,
        tylko jest tam sama nazwa a

        trzeba sprawdzaæ czy jest odpowiednia liczba argumentów,
        jeszcze nie mam b³êdów, wiêc nic siê nie zrobi je¿eli jest za ma³o

    */

    //to co zwraca funkcja
    return 5.0f;
}

float Functions::setAt(vector<float> args, vector<string> names)
{
    int x = args[0];
    int y = args[1];
    char c = args[2];

    map->setAt(x, y, c);

    return 0.0f;
}

float Functions::end(vector<float> args, vector<string> names)
{
    map->working = false;
    return 0.0f;
}

float Functions::clear(vector<float> args, vector<string> names)
{
    map->eraseMap();
    return 0.0f;
}

float Functions::setFPS(vector<float> args, vector<string> names)
{
    map->setFrameRate(args[0]);
    return 0.0f;
}

float Functions::setMapSize(vector<float> args, vector<string> names)
{
    map->setSize(args[0], args[1]);
    return 0.0f;
}

float Functions::getAt(vector<float> args, vector<string> names)
{
    return map->getAt(args[0], args[1]);
}

float Functions::drawLine(vector<float> args, vector<string> names)
{
    map->drawLine(args[0], args[1], args[2], args[3], args[4]);
    return 0.0f;
}

float Functions::insert(vector<float> args, vector<string> names)
{
    float at = args[1];
    float value = args[2];

    Variable* variable = variableManager->getVariable(names[0]);

    if (variable == nullptr)
        return 0.0f;

    if (at > variable->size)
        return 0.0f;

    if (at > 0) {
        variable->insert(at, variable->getVariable(variable->getType(), value));
    }
    else if (at == 0) {
        if (variable->getType() == GINT) {
            Int* var = dynamic_cast<Int*>(variable);
            variable->insert(1, variable->getVariable(variable->getType(), var->value));
            var->value = value;
        }
        else if (variable->getType() == GFLOAT) {
            Float* var = dynamic_cast<Float*>(variable);
            variable->insert(1, variable->getVariable(variable->getType(), var->value));
            var->value = value;
        }
        else if (variable->getType() == GBOOL) {
            Bool* var = dynamic_cast<Bool*>(variable);
            variable->insert(1, variable->getVariable(variable->getType(), var->value));
            var->value = value;
        }
        else if (variable->getType() == GCHAR) {
            Char* var = dynamic_cast<Char*>(variable);
            variable->insert(1, variable->getVariable(variable->getType(), var->value));
            var->value = value;
        }
    }

    return 0.0f;
}

//can't delete the index 0
float Functions::remove(vector<float> args, vector<string> names)
{
    Variable* variable = variableManager->getVariable(names[0]);
    if (variable == nullptr)
        return 0.0f;

    int at = args[1];

    if (at > 0 && at < variable->size) {
        variable->deleteAt(at);
    }

    return 0.0f;
}

float Functions::getVariableSize(vector<float> args, vector<string> names)
{
    Variable* variable = variableManager->getVariable(names[0]);
    if (variable == nullptr)
        return 0.0f;
    return variable->size;
}

float Functions::random(vector<float> args, vector<string> names)
{
    int i = rand() % (((int)args[1] - (int)args[0] + 1)) + (int)args[0];
    return i;
}

float Functions::printInt(vector<float> args, vector<string> names)
{
    string s = "";
    int x = args[2];
    while (x > 0) {
        s += string(1, x % 10 + '0');
        x /= 10;
    }

    for (int i = s.size() - 1; i >= 0; i--) {
        map->setAt(args[0] + s.size() - i - 1, args[1], s.at(i));
    }
    
    return 0.0f;
}
