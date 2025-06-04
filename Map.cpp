#include "Map.h"

Map::Map()
{
	system("cls");
	setSize(10, 10);
}

Map::~Map()
{
	for (int i = 0; i < width; i++) {
		delete[] map[i];
	}
	delete[] map;
}

bool Map::tick()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
		working = false;
	}

	timer += chrono::high_resolution_clock::now() - start;
	start = chrono::high_resolution_clock::now();
	if (timer >= frameTime) {
		timer -= frameTime;
		frame();
		return true;
	}
	return false;
}

void Map::frame()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left;
	int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top;

	COORD topLeft = { 0, 0 };
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(hConsole, &cursor);
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(hConsole, &cursor);

	SetConsoleCursorPosition(hConsole, topLeft);

	for (int y = 0; y < min(height, consoleHeight); y++) {
		for (int x = 0; x < min(width, consoleWidth); x++) {
			cout << map[y][x];
		}
		cout << endl;
	}
}

void Map::setSize(int x, int y)
{
	if (map != nullptr) {
		for (int i = 0; i < width; i++) {
			delete map[i];
		}
		delete map;
	}

	this->width = x;
	this->height = y;
	map = new char* [height];

	for (int i = 0; i < height; i++) {
		map[i] = new char[width];
	}
}

// Set a character at (x, y)
void Map::setAt(int x, int y, char c) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		map[y][x] = c;
	}
}

// Get the character at (x, y)
char Map::getAt(int x, int y) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		return map[y][x];
	}
	return '\0';  // Return null character if out of bounds
}

// Fill the map with spaces
void Map::eraseMap() {
	for (int y = 0; y < height; y++) {
		memset(map[y], ' ', width);
	}
}

// Draw a line using Bresenham's algorithm
void Map::drawLine(int x1, int y1, int x2, int y2, char c) {
	int dx = abs(x2 - x1), sx = (x1 < x2) ? 1 : -1;
	int dy = -abs(y2 - y1), sy = (y1 < y2) ? 1 : -1;
	int err = dx + dy;

	while (true) {
		setAt(x1, y1, c);
		if (x1 == x2 && y1 == y2) break;
		int e2 = 2 * err;
		if (e2 >= dy) { err += dy; x1 += sx; }
		if (e2 <= dx) { err += dx; y1 += sy; }
	}
}

void Map::setFrameRate(int frames)
{
	frameTime = 1000ms / frames;
}
