#pragma once

#include <iostream>
#include<Windows.h>
#include<chrono>
#include<string>
#include<ctime>

using theTime = std::chrono::steady_clock::time_point;
using namespace std;

class Map {
public:
	Map();
	~Map();

	bool tick();
	void frame();
	void setSize(int x, int y);

	void setAt(int x, int y, char c);
	char getAt(int x, int y);
	void eraseMap();
	void drawLine(int x1, int y1, int x2, int y2, char c);
	void setFrameRate(int frames);

	bool working = true;
	bool error = false;
private:
	int width = 10, height = 10;
	char** map;

	chrono::milliseconds frameTime = 50ms;
	chrono::steady_clock::time_point start = chrono::high_resolution_clock::now();
	chrono::duration<double> timer = chrono::duration<double>::zero();
};