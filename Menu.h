#pragma once
#include <iostream>
#include<Windows.h>
#include<chrono>
#include<thread>
#include<string>
#include<fstream>
#include<conio.h>
#include<ctime>
#include<vector>
#include "Compiler.h"

using namespace std;

class Menu
{
private:
	string file1Name;
	string file2Name;
	string file3Name;
	HANDLE c1;
	COORD topLeft;
	CONSOLE_CURSOR_INFO cursor;
	vector<string> lines1;
	vector<string> lines2;
	vector<string> lines3;
	vector<string> map;
	string path;
	fstream plik1;
	fstream plik2;
	fstream plik3;
public:
	void setC();
	vector<string> write(vector<string> test1);
	void setup(Compiler* compiler);
	void show(Compiler* compiler);
	void EndOfProgram(Compiler* compiler);
	char finish();
	char menu(char opt);
	friend void clearScreen();
	friend string createFolder(string path);
	char niceMenu(vector<string> options);
	Menu() {};
	~Menu() {};
};