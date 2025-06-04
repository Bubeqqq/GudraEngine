#include "Menu.h"
#include "Map.h"
#include "Compiler.h"

void MoveCursorToEnd()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		COORD newCursorPos;
		newCursorPos.X = 0;                            // start of line
		newCursorPos.Y = csbi.dwCursorPosition.Y + 2;  // move to next line
		SetConsoleCursorPosition(hConsole, newCursorPos);
	}
}

int main()
{
	char endProgram = '0';

	Menu menu;
	cout << R"( $$$$$$\                  $$\                          $$$$$$$$\                     $$\                     
$$  __$$\                 $$ |                         $$  _____|                    \__|                    
$$ /  \__|$$\   $$\  $$$$$$$ | $$$$$$\  $$$$$$\        $$ |      $$$$$$$\   $$$$$$\  $$\ $$$$$$$\   $$$$$$\  
$$ |$$$$\ $$ |  $$ |$$  __$$ |$$  __$$\ \____$$\       $$$$$\    $$  __$$\ $$  __$$\ $$ |$$  __$$\ $$  __$$\ 
$$ |\_$$ |$$ |  $$ |$$ /  $$ |$$ |  \__|$$$$$$$ |      $$  __|   $$ |  $$ |$$ /  $$ |$$ |$$ |  $$ |$$$$$$$$ |
$$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |     $$  __$$ |      $$ |      $$ |  $$ |$$ |  $$ |$$ |$$ |  $$ |$$   ____|
\$$$$$$  |\$$$$$$  |\$$$$$$$ |$$ |     \$$$$$$$ |      $$$$$$$$\ $$ |  $$ |\$$$$$$$ |$$ |$$ |  $$ |\$$$$$$$\ 
 \______/  \______/  \_______|\__|      \_______|      \________|\__|  \__| \____$$ |\__|\__|  \__| \_______|
                                                                           $$\   $$ |                        
                                                                           \$$$$$$  |                        
                                                                            \______/                         )" << std::endl;

	cout << endl << "Nacisnij ENTER" << endl;
	while (true) {
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			break;
	}

	while (endProgram != '4') {
		int n = 0;
		if (endProgram != '5') {
			endProgram = menu.menu(endProgram);

		}
		if (endProgram == '4') {
			continue;
		}

		Map* map = new Map();
		Compiler* compiler = new Compiler(map);

		menu.setup(compiler);
		compiler->checked = false;

		while (map->working && !map->error) {
			if (map->tick()) {
				menu.show(compiler);
			}
		}

		if (!map->error) {
			compiler->checked = false;
			menu.EndOfProgram(compiler);
			map->frame();
		}

		while (_kbhit()) {
			_getch();  // odczytaj i odrzuæ
		}

		MoveCursorToEnd();

		cout << endl << "Nacisnij ENTER" << endl;
		while (true) {
			if (GetAsyncKeyState(VK_RETURN) & 0x8000)
				break;
		}

		delete compiler;
		delete map;

		endProgram = menu.finish();
	}
}