#pragma once

#include "VariableManager.h"
#include "Map.h"

#include <cstdlib>
#include <ctime>
#include <vector>
#include <Windows.h>

//extern "C" __declspec(dllimport) short __stdcall GetAsyncKeyState(int vKey);

class Functions {
public:
	Functions(VariableManager* variableManager, Map* map);

	//functions
	float isKeyPressed(vector<float> args, vector<string> names);
	float testowa(vector<float> args, vector<string> names);
	float setAt(vector<float> args, vector<string> names);
	float end(vector<float> args, vector<string> names);
	float clear(vector<float> args, vector<string> names);
	float setFPS(vector<float> args, vector<string> names);
	float setMapSize(vector<float> args, vector<string> names);
	float getAt(vector<float> args, vector<string> names);
	float drawLine(vector<float> args, vector<string> names);
	float insert(vector<float> args, vector<string> names);
	float remove(vector<float> args, vector<string> names);
	float getVariableSize(vector<float> args, vector<string> names);
	float random(vector<float> args, vector<string> names);
	float printInt(vector<float> args, vector<string> names);

private:
	VariableManager* variableManager;
	Map* map;
};