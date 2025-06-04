#include "Menu.h"
void clearScreen() {//taki system("cls) tylko bez tego efektu przeskakujacego kursora
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;
	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
}
char Menu::niceMenu(vector<string> options) {//opcja to wybor w menu np "zakoncz" to opcja

	while (_kbhit()) {
		_getch();  // odczytaj i odrzu�
	}
	system("cls");
	char end = '0';
	COORD w = { 0,0 };
	HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(c, &cursor);
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(c, &cursor);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(c, &csbi);
	int number = options.size();
	int characters = 0;//laczna ilosc liter w menu
	int which = 0;//opcja na ktora aktualnie najechalismy
	int tab[8] = { 0,0,0,0,0,0,0,0 };//tu bedzie zapisany odlegosc danej opcji od lewej krawedzi w menu np "edytuj plik1"
	for (int n = 1; n < options.size(); n++)//do talbicy sa dodawane dlugosci opcji poprzedzajacych dana opcje czyli opcje 1 poprzedza opcja 0 itd.
	{
		for (int m = 0; m <= n - 1; m++) {
			tab[n] += options[m].size();
		}
	}
	int longest = 0;//najdluzsza opcja
	for (auto el : options) {
		longest = max(longest, el.size());
		characters += el.size();
	}
	//int left = (csbi.srWindow.Right - csbi.srWindow.Left - characters - (number + 1));
	int space = ((csbi.srWindow.Right - csbi.srWindow.Left) - characters - (number + 1)) / (number + 1);//odstep miedzy opcjami
	int height = (csbi.srWindow.Bottom - csbi.srWindow.Top) / 2;

	for (int i = 0; i < number; i++) {    //tu dodawana jest ilosc przerw az do wybranej opcji czyli jesli przerwy to "_", a opcje to 1,2 i 3
		tab[i] += (space + 1) * (i + 1);// to mozna to w konsoli powinno to tak wygladac _1_2_3_ , np. dla opcji 3 w tablicy dodane zostan�  3 przerwy
	}



	while (end != '1' && end != '2' && end != '3' && end != '4') {//dopoki nie zostanie wybrana opcja

		int oldR = csbi.srWindow.Right;//poprzedni rozmiar konsoli
		int oldL = csbi.srWindow.Left;
		int oldT = csbi.srWindow.Top;
		int oldB = csbi.srWindow.Bottom;
		GetConsoleScreenBufferInfo(c, &csbi);
		if (csbi.srWindow.Right - csbi.srWindow.Left <= longest + 2 || ((csbi.srWindow.Bottom - csbi.srWindow.Top <= 3 * number) && (csbi.srWindow.Right - csbi.srWindow.Left <= characters + number)) || csbi.srWindow.Bottom - csbi.srWindow.Top <= 2) {
			system("cls");
			cout << "zwieksz rozmiar okna konsoli";// gdy okno jest za male aby wyswietlic menu
		}
		else {
			space = ((csbi.srWindow.Right - csbi.srWindow.Left) - characters - (number + 1)) / (number + 1);//nowa spacja jest zmieni sie rozmiar okna

			height = (csbi.srWindow.Bottom - csbi.srWindow.Top) / 2;//wysokosc okna konsoli
			if (oldR != csbi.srWindow.Right || oldL != csbi.srWindow.Left || oldB != csbi.srWindow.Bottom || oldT != csbi.srWindow.Top) {
				system("cls");//gdy zmieni sie rozmiar okna konsoli
				for (int i = 0; i < 8; i++) {//czyszczenie tablicy
					tab[i] = 0;
				}
				for (int n = 1; n < options.size(); n++)
				{
					for (int m = 0; m <= n - 1; m++) {
						tab[n] += options[m].size();
					}//to samo co wyzej
				}
				for (int i = 0; i < number; i++) {
					tab[i] += (space + 1) * (i + 1);
				}
				continue;
			}
			if (csbi.srWindow.Right - csbi.srWindow.Left > characters + number) {//wszystkie opcje obok siebie
				for (int i = 1; i < height; i++) {
					cout << endl;
				}
				//poniewaz element akutalnie wybierany jest podnoszony, to wypisuje spacje az do miejsca gdzie ten powinien byc
				for (int k = -1; k <= tab[which]; k++) {
					cout << " ";
				}
				cout << options[which] << endl;
				cout << "  ";//jest bardziej rowno dzieki temu
				for (auto el : options) {


					for (int j = 0; j <= space; j++) {
						cout << " ";//wypisuje spacje 

					}
					if (options[which] == el) {//jesli jest to aktualnie wybierany element to wypisuje _ zamiast jego nazwy
						for (int j = 0; j < options[which].size(); j++) {
							cout << "-";
						}
					}
					else {
						cout << el;
					}
				}



				SetConsoleCursorPosition(c, w);//ustawia kursor na start konsoli {0,0}
			}
			else {//opcje jedna pod druga
				int z = 0;
				for (auto el : options) {
					int size = el.size();
					if (el.size() % 2 == 1) {//elementy sie rozjedzaly i byl taki efekt (znak | to scianka konsoli)
						//	gdy opcje mialy parzyste i nieparzyste dlugosci		    | 00 |
																				//  |  1 |
						size = size + 1;
					}
					tab[z] = (csbi.srWindow.Right - csbi.srWindow.Left - size) / 2;//odsted od lewej krawedzi dla kazdego elementu

					z++;
				}
				height = (csbi.srWindow.Bottom - csbi.srWindow.Top - 2 * number) / (number);//pionowa przerwa miedzy opcjami
				z = 0;

				int h = 0;

				for (auto el : options) {
					if (z == which) {//gdy jest to aktualnie wybierana opcja bedzie ona linijke wyzej
						h = 1;
					}
					else {
						h = 0;
					}
					for (int i = h; i <= height; i++) {
						cout << endl;
					}
					for (int j = 0; j <= tab[z]; j++) {//odstep od lewej krawedzi

						cout << " ";
					}

					cout << el;
					if (z == which) {
						cout << endl;

						for (int j = 0; j <= tab[z]; j++) {
							cout << " ";
						}
						for (int g = 0; g < el.size(); g++) {//wyswietlanie podkreslenia wybieranej aktualnie opcji
							cout << "-";
						}
					}
					z++;
				}
				SetConsoleCursorPosition(c, w);
			}
		}
		if (_kbhit()) {
			char symbol = _getch();

			if (symbol == -32 || symbol == 224) {//do strza?ek
				symbol = _getch();
				switch (symbol) {
				case 72://g?ra
					which--;
					break;
				case 80://d?
					which++;
					break;
				case 75://lewo
					which--;
					break;
				case 77://prawo
					which++;

					break;
				default:
					break;
				}
				system("cls");
			}
			else if (static_cast<int>(symbol) == 13) {//enter czyli zatwierdz opcje
				end = 49 + which;//which jest intem wiec do cyfry char +48 i +1 aby zgadzalo sie z reszta menu

				break;
			}
		}
		//gdy np na ostatniej opcji chcemy wybrac opcje nastepna to przejdzie do opcji zerowej i w druga strone analogicznie
		if (which < 0) {
			which = options.size() - 1;
		}
		if (which >= options.size()) {
			which = 0;
		}
		if (end != '0') {
			break;
		}
	}
	system("cls");
	cursor.bVisible = TRUE;
	SetConsoleCursorInfo(c, &cursor);
	//cin.ignore();
	return end;
}

string createFolder(string path) {
	bool isCreated = false;
	string add = "_";

	while (!isCreated) {
		system("cls");

		// Pr�ba utworzenia folderu
		if (CreateDirectoryA(path.c_str(), NULL)) {
			cout << "Folder zostal utworzony pomyslnie pod adresem: " << path << endl;
			isCreated = true;
		}
		else {
			DWORD error = GetLastError();
			if (error == ERROR_ALREADY_EXISTS) {
				// Folder ju� istnieje � dodaj sufiks i pr�buj dalej
				path += add;
			}
			else {
				cout << "B��d: Nie mo�na utworzy� folderu. Kod b��du: " << error << endl;
				break;
			}
		}

	}
	cout << "aby przejsc dalej wcisnij enter" << endl;
	char enter = 0;
	while (enter != 13) {
		enter = _getch();
	}
	return path;
}
vector<string> Menu::write(vector<string> test1) {
	while (_kbhit()) {
		_getch();  // odczytaj i odrzu�
	}
	test1.push_back("");
	bool done = false;
	string pause;
	HANDLE	c1 = GetStdHandle(STD_OUTPUT_HANDLE);
	short x = 0;
	short y = 0;
	COORD pos = { x, y };
	COORD start = { 0, 0 };
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(c1, &cursor);
	//cursor.bVisible = FALSE;
	SetConsoleCursorInfo(c1, &cursor);
	string input;
	char symbol;
	int num = 0;

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(c1, &csbi);
	SHORT topLine = csbi.srWindow.Top;
	SHORT bottomLine = csbi.srWindow.Bottom;
	SHORT right = csbi.srWindow.Right;
	SHORT left = csbi.srWindow.Left;
	SHORT xx = 0;
	for (auto el : test1) {

		for (int i = 0; i < el.length(); i++) {//usuwa dziwne znakki
			if (static_cast<int>(el[i]) < 33 || static_cast<int>(el[i]) > 126) {
				el.erase(i, 1);
			}
		}
	}
	SetConsoleCursorPosition(c1, start);//kursor na 0,0
	for (int j = 0; j < min(bottomLine - topLine, test1.size()); j++) {//gdy ilosc lini jest wieksza od wysokosci konsoli to wyswietlona zostanie 
		int width = right - left;									// taka ilosc lini aby zmiescia sie w oknie
		if (width + 3 > test1[j].size()) {//gdy szerokosc okna konsoli jest wieksza niz dlugosc danej lini
			cout << test1[j] << endl;
		}
		else if (x < width - 1) {//pozycja kursora nie przy prawej krawedzi okna
			cout << test1[j].substr(0, width) << endl;
		}
		else {//gdy pozycja kursora jest przy prawej krawedzi okna to "utnie" znaki z pocz�tku i pokaze nowe znaki z konca
			int beg = x - width + 1;

			if (beg + width > test1[j].size()) {
				beg = test1[j].size() - width;
			}
			cout << test1[j].substr(beg, width) << endl;

		}
	}
	SetConsoleCursorPosition(c1, start);

	while (!done) {
		if (_kbhit()) {
			symbol = _getch();
			input = symbol;
			if (symbol == -32 || symbol == 224) {//do strza?ek
				symbol = _getch();
				switch (symbol) {
				case 72://g?ra
					y = y - 1;
					if (y < 0) y = 0;
					if (x > test1[y].size()) {
						x = test1[y].size();
					}
					break;
				case 80://d?
					y = y + 1;

					if (y >= test1.size()) {

						test1.resize(test1.size() + 1);
					}
					if (x > test1[y].size()) {
						x = test1[y].size();
					}
					break;
				case 75://lewo
					x = x - 1;
					if (x < 0) x = 0;
					break;
				case 77://prawo
					x = x + 1;
					if (x >= test1[y].size()) {
						test1[y].resize(x + 1);
					}
					break;
				}
			}//normalne znaki
			else if ((static_cast<int>(symbol) >= 32) && (static_cast<int>(symbol) <= 126)) {
				if (y >= 0 && y < test1.size() && x >= 0 && x <= test1[y].size()) {

					test1[y].insert(x, input);
				}

				for (int i = 0; i < x; i++) {//uzupe?nia puste miejsca spacjami
					if (test1[y][i] == 0) {
						test1[y][i] = ' ';
					}
				}
				x = x + 1;
				if (test1[y].size() <= x) {
					test1[y].resize(test1[y].size() + 1);
				}

			}
			else if (static_cast<int>(symbol) == 8) {//backspace
				bool wasEmpty = test1[y].empty();
				string c;
				x = x - 1;
				if (x < 0) {
					if (!wasEmpty) {//przenosi reszte tekstu do nowej lini
						c = test1[y];
						test1.erase(test1.begin() + y);
					}
					if (test1[y].empty() && test1.size() > 1) {//usuwa linie
						test1.erase(test1.begin() + y);
					}
					y = y - 1;
					if (y < 0) {
						y = 0;
					}


					if (!wasEmpty) {
						test1[y] += c;//wkleja przeniesiona zawartosc z usunietej lini
						x = x + 1;
					}
					x = test1[y].size();//ustawia kursor na koniec linii
				}
				else {

					test1[y].erase(x, 1);//usuwa pojedynczy znak
				}

			}
			else if (static_cast<int>(symbol) == 13) {//enter
				string afterCursor = test1[y].substr(x);

				test1[y] = test1[y].substr(0, x);//to co przed kursorem zostaje

				x = 0;
				y = y + 1;

				test1.push_back("");//tworzy nowa linie
				test1.emplace(test1.begin() + y, afterCursor);// wkleja zawartosc ktora byla za kursorem

			}
			else if (static_cast<int>(symbol) == 27) {//escape
				done = true;
				system("cls");
			}

			num = 1;//tego chyba nie musialo byc i tego if wtedy tez ale dziala to zostawiam
			symbol = 0;
			input.clear();
			if (num) {//wy?wietlenie zawarto?ci i usuni?cia pustych znak?w
				//clearScreen();
				cursor.bVisible = FALSE;
				SetConsoleCursorInfo(c1, &cursor);
				system("cls");
				SHORT oldR = right;
				SHORT oldL = left;
				SHORT oldT = topLine;
				SHORT oldB = bottomLine;
				GetConsoleScreenBufferInfo(c1, &csbi);
				right = csbi.srWindow.Right;
				left = csbi.srWindow.Left;
				topLine = csbi.srWindow.Top;
				bottomLine = csbi.srWindow.Bottom;
				if (oldT != topLine || oldB != bottomLine || oldL != left || oldR != right) {//gdy zmienil sie rozmiar okna konsoli
					system("cls");
				}
				SetConsoleCursorPosition(c1, start);
				int diff = bottomLine - topLine;//wysokosc okna konsoli
				int size = lines1.size();
				int range = 0;//srodek konsoli w pionie
				int width = right - left;//szerokosc okna konsoli
				if (diff % 2 == 0) {
					range = diff / 2;
				}
				else {
					range = (diff - 1) / 2;
				}
				int start = 0;
				int extra = 0;
				if (y - range < 0) {
					extra = range - y;// w przypadku gdy kursor nie jest na srodku tylko np w 1 linii
				}//extra to dodatkowe linie np z dolu gdy nie ma zadnych linii nad kursorem

				start = y - range + extra;
				for (int j = start; j < min(start + diff, test1.size()); j++) {	//analogicznie jak w linii 278
					if (width + 3 > test1[j].size()) {
						cout << test1[j] << endl;
					}
					else if (x < width - 1) {
						cout << test1[j].substr(0, width) << endl;
					}
					else {
						int beg = x - width + 1;

						if (beg + width > test1[j].size()) {
							beg = test1[j].size() - width;
						}
						cout << test1[j].substr(beg, width) << endl;

					}
				}//start //klatka //koniec
				short yy = range;
				xx = width - 1;
				if (y > range) {//gdy kursor jest w linii za polowa wysokosci konsoli
					pos = { x, yy };
				}
				else if (x > width) {//gdy kusor jest dalej w poziomie niz szerokosc konsoli
					pos = { xx, y };
				}
				else if (y > range && x > width) {//gdy oba
					pos = { xx, yy };
				}
				else {
					pos = { x, y };
				}
				cursor.bVisible = TRUE;
				SetConsoleCursorInfo(c1, &cursor);
				SetConsoleCursorPosition(c1, pos);
				num = 0;



			}

		}
	}
	for (auto& el : test1) {//usuwa dziwne znaki
		for (int i = 0; i < el.length(); i++) {
			if (static_cast<int>(el[i]) < 32 || static_cast<int>(el[i]) > 126) {
				el.erase(i, 1);
				i--;
			}
		}
	}
	return test1;
}
char Menu::menu(char opt) {
	vector<string> menu1 = {
		"stworz pusty projekt",
		"otworz istniejacy projekt",
		"instrukcje",
		"zakoncz",

	};
	HANDLE	c1 = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(c1, &cursor);
	cursor.bVisible = TRUE;
	SetConsoleCursorInfo(c1, &cursor);

	char option = '0';
	file1Name = "tekst1.txt";
	file2Name = "tekst2.txt";
	file3Name = "tekst3.txt";

	lines1.clear();
	lines2.clear();
	lines3.clear();
	while (option != '1' && option != '3' && option != '2' && option != '4') {//warstwa 1 menu
		system("cls");
		if (opt != '6') {
			if (option == '9') {
				cout << "Nie udalo sie otworzyc pliku, lub znalezc folderu" << endl << "nacisnij enter aby kontynuowac" << endl;
				char enter = 0;
				while (enter != 13) {
					enter = _getch();
				}
			}

			option = niceMenu(menu1);

		}
		else {
			option = '2';
		}
		if (option == '1') {
			system("cls");
			cout << "podaj miejsce utworzenia folderu: ";
			cin >> path;

			path = createFolder(path);
			plik1.open(path + "\\" + file1Name, ios::out);
			plik2.open(path + "\\" + file2Name, ios::out);
			plik3.open(path + "\\" + file3Name, ios::out);
			if (plik1.good() && plik2.good() && plik3.good()) {

				char option2 = '0';//warstwa druga menu

				while (option2 != '1' && option2 != '3' && option2 != '2' && option2 != '4' && option2 != '5') {
					vector<string> menu2 = {
		"edytuj start",
		"edytuj klatka",
		"edytuj koniec",
		"uruchom program",
		"zakoncz"

					};
					option2 = niceMenu(menu2);


					if (option2 == '1') {
						system("cls");
						lines1 = write(lines1);//write to edycja zawarto?ci

						option2 = '0';
					}
					else if (option2 == '2') {
						system("cls");
						lines2 = write(lines2);
						option2 = '0';
					}
					else if (option2 == '3') {
						system("cls");
						lines3 = write(lines3);
						option2 = '0';
					}
					else if (option2 == '4') {
						system("cls");

					}
					else if (option2 == '5') {
						option = '4';
						break;
					}
					system("cls");
				}
				int size = lines1.size();
				for (int j = 0; j < size; j++) {
					if (lines1[j].empty()) {//usuwa puste linie

						lines1.erase(lines1.begin() + j);
						j--;
						size = lines1.size();
						continue;
					}
					for (int i = 0; i < lines1[j].length(); i++) {//usuwa inne znaki
						if (static_cast<int>(lines1[j][i]) < 32 || static_cast<int>(lines1[j][i]) > 126) {
							lines1[j].erase(i, 1);
							i--;
						}
					}
					plik1 << lines1[j] << endl;//zapis do pliku

				}
				size = lines2.size();
				plik1.close();
				for (int j = 0; j < size; j++) {//usuwa puste linie,usuwa inne znaki,zapis do pliku
					if (lines2[j].empty()) {

						lines2.erase(lines2.begin() + j);
						j--;
						size = lines2.size();
						continue;
					}
					for (int i = 0; i < lines2[j].length(); i++) {
						if (static_cast<int>(lines2[j][i]) < 32 || static_cast<int>(lines2[j][i]) > 126) {
							lines2[j].erase(i, 1);
							i--;
						}
					}
					plik2 << lines2[j] << endl;
					size = lines2.size();
				}
				size = lines3.size();
				plik2.close();
				for (int j = 0; j < size; j++) {//usuwa puste linie,usuwa inne znaki,zapis do pliku
					if (lines3[j].empty()) {

						lines3.erase(lines3.begin() + j);
						j--;
						size = lines3.size();
						continue;
					}
					for (int i = 0; i < lines3[j].length(); i++) {
						if (static_cast<int>(lines3[j][i]) < 32 || static_cast<int>(lines3[j][i]) > 126) {
							lines3[j].erase(i, 1);
							i--;
						}
					}
					plik3 << lines3[j] << endl;
					size = lines3.size();
				}
				plik3.close();
			}
			else {
				option = '9';//gdy plik1,2,3 nie s? good
				continue;
			}
		}
		else if (option == '2') {
			system("cls");
			if (opt != '6') {
				cout << "podaj gdzie znajduje sie folder: ";
				cin >> path;

			}
			string line;
			plik1.open(path + "\\" + file1Name, ios::in);
			plik2.open(path + "\\" + file2Name, ios::in);
			plik3.open(path + "\\" + file3Name, ios::in);

			if (plik1.good() && plik2.good() && plik3.good()) {//wczytanie zawarto?ci plik?w
				while (!plik1.eof()) {
					getline(plik1, line);
					lines1.push_back(line);
				}
				plik1.close();
				while (!plik2.eof()) {
					getline(plik2, line);
					lines2.push_back(line);
				}
				plik2.close();
				while (!plik3.eof()) {
					getline(plik3, line);
					lines3.push_back(line);
				}
				plik3.close();

				char option2 = '0';
				//warstwa 2 menu, opcja 2
				while (option2 != '1' && option2 != '3' && option2 != '2' && option2 != '4' && option2 != '5') {
					system("cls");
					vector<string> menu2 = {
		"edytuj start",
		"edytuj klatka",
		"edytuj koniec",
		"uruchom program",
		"zakoncz"

					};
					option2 = niceMenu(menu2);

					/*cout << "1. edytuj plik1" << endl;
					cout << "2. edytuj plik2" << endl;
					cout << "3. edytuj plik3" << endl;
					cout << "4. uruchom program" << endl;
					cout << "5. zakoncz" << endl;
					cout << "wybierz opcje: ";
					cin >> option2;*/
					if (option2 == '1') {
						system("cls");
						lines1 = write(lines1);

						option2 = '0';
					}
					else if (option2 == '2') {
						system("cls");
						lines2 = write(lines2);
						option2 = '0';
					}
					else if (option2 == '3') {
						system("cls");
						lines3 = write(lines3);
						option2 = '0';
					}
					else if (option2 == '4') {
						system("cls");

					}
					else if (option2 == '5') {
						option = '4';

					}
				}
				plik1.open(path + "\\" + file1Name, ios::out, ios::trunc);
				plik2.open(path + "\\" + file2Name, ios::out, ios::trunc);
				plik3.open(path + "\\" + file3Name, ios::out, ios::trunc);
				if (plik1.good() && plik2.good() && plik3.good()) {
					int size = lines1.size();
					for (int j = 0; j < size; j++) {//usuwa puste linie
						if (lines1[j].empty()) {

							lines1.erase(lines1.begin() + j);
							j--;
							size = lines1.size();
							continue;
						}
						for (int i = 0; i < lines1[j].length(); i++) {//usuwa inne znaki
							if (static_cast<int>(lines1[j][i]) < 32 || static_cast<int>(lines1[j][i]) > 126) {
								lines1[j].erase(i, 1);
								i--;
							}
						}
						plik1 << lines1[j] << endl;

					}
					size = lines2.size();
					plik1.close();
					for (int j = 0; j < size; j++) {//usuwa puste linie,usuwa inne znaki,zapis do pliku
						if (lines2[j].empty()) {

							lines2.erase(lines2.begin() + j);
							j--;
							size = lines2.size();
							continue;
						}
						for (int i = 0; i < lines2[j].length(); i++) {
							if (static_cast<int>(lines2[j][i]) < 32 || static_cast<int>(lines2[j][i]) > 126) {
								lines2[j].erase(i, 1);
								i--;
							}
						}
						plik2 << lines2[j] << endl;
						size = lines2.size();
					}
					size = lines3.size();
					plik2.close();
					for (int j = 0; j < size; j++) {//usuwa puste linie,usuwa inne znaki,zapis do pliku
						if (lines3[j].empty()) {

							lines3.erase(lines3.begin() + j);
							j--;
							size = lines3.size();
							continue;
						}
						for (int i = 0; i < lines3[j].length(); i++) {
							if (static_cast<int>(lines3[j][i]) < 32 || static_cast<int>(lines3[j][i]) > 126) {
								lines3[j].erase(i, 1);
								i--;
							}
						}
						plik3 << lines3[j] << endl;
						size = lines3.size();
					}
					plik3.close();
				}
				else {
					option = '9';
					continue;

				}
			}
			else {
				option = '9';
				continue;

			}
		}
		else if (option == '3') {

			while (_kbhit()) {
				_getch();  // odczytaj i odrzu�
			}
			system("cls");


			cout << "Po menu poruszaj sie za pomoca strzalek" << endl << endl;

			cout << "W edytorze uzywaj strzalek aby poruszac sie po tekscie" << endl;
			cout << "Aby wyjsc i zapisac plik nacisnij klawisz Escape" << endl;
			cout << "Zawartosc plikow mozesz rowniez edytowac za pomoca np. notatnika" << endl;
			cout << "W tym celu zakoncz program, znajdz lokalizacje folderu i wybierz plik do edycji" << endl << endl;

			cout << "Jesli zostala wyswietlona pusta mapa, oznacza to ze w kodzie jest jakis blad" << endl;

			cout << "nacisnij enter aby wrocic" << endl;
			char enter = 0;
			do {
				enter = _getch();
			} while (enter != 13);


			option = '0';
		}

		else if (option == '4') {
			break;
		}

	}
	return option;
}
void Menu::setup(Compiler* compiler) {
	//funkcja1(lines1);
	compiler->runCode(lines1);
}

void Menu::show(Compiler* compiler) {
	//funkcja2(lines2) czyli zawartosc pliku
	compiler->runCode(lines2);
}
void Menu::EndOfProgram(Compiler* compiler)
{
	compiler->runCode(lines3);
}
char Menu::finish() {//po wykonaniu programu
	char end = '0';
	HANDLE	c1 = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(c1, &cursor);
	cursor.bVisible = TRUE;
	SetConsoleCursorInfo(c1, &cursor);
	while (end != '1' && end != '2' && end != '3' && end != '4') {
		//wyswietl wartosci koncowe
		system("cls");
		vector<string> menu3 = {
			"wroc do menu",
			"wroc do edycji plikow",
			"uruchom program ponownie",
			"zakoncz"

		};
		/*cout << "1. wroc do menu" << endl;
		cout << "2. wroc do edycji plikow" << endl;
		cout << "3. uruchom program ponownie" << endl;
		cout << "4. zakoncz" << endl;
		cout << "wybierz opcje: ";
		cin >> end;*/
		end = niceMenu(menu3);

		system("cls");
		if (end == '1') {
			end = '0';
			break;
		}
		else if (end == '2') {
			end = '6';
			break;
		}
		else if (end == '3') {
			end = '5';
			break;
		}
		else if (end == '4') {
			break;
		}
	}
	return end;
}