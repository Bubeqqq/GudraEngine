Dokumentacja Projektu „GudraEngine"

Wydział Matematyki stosowanej

Informatyka (Profil Praktyczny)

Rok studencki 2024/2025

Błażej Skorzysko

Bartosz Więcek

1.  Opis „GudraEngine"

**GudraEngine** to prosty silnik stworzony z myślą o tworzeniu gier
konsolowych, ale sprawdzi się również w projektach algorytmicznych i
innych aplikacjach tekstowych. Silnik pozwala użytkownikowi utworzyć
nowy projekt w dowolnie wybranym folderze, a następnie edytować trzy
podstawowe pliki: **start**, **klatka** i **koniec**, które zawierają
kod sterujący działaniem aplikacji.

Kod pisany jest w dedykowanym języku **C\--**, który oferuje podstawowe
elementy potrzebne do programowania: cztery typy danych, obsługę tablic,
instrukcje warunkowe, pętle, oraz funkcje przyjmujące argumenty i
zwracające wartości.

a)  Edytor zawartości plików, oraz menu

> Klasa Menu odpowiada za menu, edytor plików,
>
> tworzenie i otwieranie folderu, oraz plików txt.

- Menu

<!-- -->

- Odpowiada za nawigację po programie. Umożliwia wybranie ścieżki
  utworzenia folderu z plikami txt (gdy folderu o danej nazwie już
  istnieje, program dodaje znak "\_" do nazwy tak długo, aż będzie
  możliwe utworzenie folderu), lub wybranie istniejącego (jeśli
  istnieje). Menu dostosowuje się również do rozmiaru okna konsoli.

  - Edytor

    - Pozwala na zmianę zawartości pliku w programie bez konieczności
      otwierania plików txt za pomocą np. notatnika. Zawartość pliku
      zostaje wczytana do vectora, którego zawartość następnie możemy
      edytować, po czym do pliku zostaje zapisany nowy tekst. Każdą
      linie tekstu można zmienić w dowolnym miejscu bez konieczności
      zatwierdzania wprowadzonego tekstu klawiszem Enter. Ilość, oraz
      długość linii jest dopasowywana do rozmiaru okna konsoli, a w
      przypadku jego zmiany naciśnięcie dowolnego klawisza spowoduje
      przeskalowanie wyświetlanego tekstu do nowego rozmiaru okna.

b)  Język C\--

- Kod źródłowy języka znajduje się w folderze C\--. Najważniejszym
  komponentem jest klasa Compiler, odpowiedzialna za interpretację kodu.
  Kompilator przetwarza kod linijka po linijce, analizując pierwszy
  token i podejmując odpowiednie działanie.

<!-- -->

- Współpracuje z czterema klasami pomocniczymi:

<!-- -->

- variableManager -- zarządza zmiennymi,

- functionManager -- obsługuje funkcje i ich wywołania,

- calculator -- odpowiada za obliczenia matematyczne i logiczne,

- Logs -- rejestruje błędy i zatrzymuje wykonanie w przypadku
  niepowodzeń.

> **Szczegóły działania:**

- **Instrukcja warunkowa jezeli**: sprawdza warunek i pomija kod w
  nawiasach, jeśli nie jest spełniony.

- **Pętla dopoki**: zapamiętuje punkt startowy i powtarza blok dopóki
  warunek jest prawdziwy.

- **Deklaracje zmiennych**: są przetwarzane, jeśli linia nie jest
  instrukcją ani funkcją.

- **Zarządzanie zmiennymi**: każda zmienna jest traktowana jak tablica,
  nawet te proste.

- **Klasa Calculator**

> To kluczowy element języka --- odpowiada za obliczenia. Wykorzystuje
> algorytm **Shunting Yard** do konwersji wyrażeń do notacji
> postfiksowej, co umożliwia obliczanie wartości za pomocą stosu.
> Obsługuje zmienne, stałe oraz wywołania funkcji.

c)  Opis klasy Calculator:

- LoadAsStack

> Ten kod służy do rozdzielania wzoru funkcji na pojedyncze słowa
> kluczowe oraz przygotowuje je do dalszych operacji.
>
> • Funkcja addMultiplyBetweenWordAndNumber: Wstawia operator \* między
> liczbami a literami (np. 2sin(x) → 2 \* sin(x)).
>
> • sortMinuses: Przypisuje znaki minus do liczb lub zmiennych (np. -x
> zamiast + - x).
>
> • loadAsStack: Rozdziela wyrażenie na tokeny i przekształca je w stos.
> Łączy ze sobą liczby, litery oraz inne znaki występujące obok siebie.

- Check

> Kod sprawdza poprawność składniową wprowadzonego działania
> matematycznego, analizując tokeny (liczby, operatory, nawiasy,
> funkcje) w stosie. Główna funkcja check weryfikuje, czy wyrażenie
> spełnia reguły składniowe, np. poprawne sparowanie nawiasów,
> odpowiednie rozmieszczenie operatorów oraz użycie funkcji (np. sin,
> cos) z otwierającym nawiasem. Funkcje pomocnicze, takie jak isWord,
> isNumber, czy isOpenBracket, klasyfikują tokeny i identyfikują ich
> rodzaj, co pozwala dokładnie analizować składnię. Jeśli napotkany
> zostanie błąd (np. brak nawiasu, nieznana operacja, źle użyta
> funkcja), program wyświetla błąd.

- ShuntingYardAlgorithm

> Zamienia wyrażenie z notacji infiksowej na notację postfiksową.
> Algorytm używa 3 stosów (wynik, kolejka, funkcja). Iterujemy po każdym
> wyrażeniu w funkcji. Nazywamy to wyrażenie token.
>
> 1\. Jeśli token jest liczbą:
>
> • Dodaj go bezpośrednio do wyniku.
>
> 2\. Jeśli token jest operatorem:
>
> • Jeśli kolejka jest pusta, umieść operator w kolejce.
>
> • W przeciwnym razie:
>
> -- Porównaj jego priorytet z operatorem na szczycie kolejki.
>
> -- Jeśli priorytet operatora w kolejce jest wyższy lub równy, zdejmuj
> operatory z kolejki i umieszczaj je w wyniku, aż warunek przestanie
> być spełniony.
>
> -- Umieść nowy operator w kolejce.
>
> 3\. Jeśli token jest nawiasem otwierającym :
>
> • Umieść go w kolejce.
>
> 4\. Jeśli token jest nawiasem zamykającym :
>
> • Zdejmuj operatory z kolejki i umieszczaj je w kolejce wyjściowej, aż
> napotkasz nawias otwierający.
>
> • Usuń nawias otwierający z kolejki (nie dodawaj go do wyniku).
>
> 5\. Po przeanalizowaniu wszystkich tokenów:
>
> • Przenieś wszystkie pozostałe operatory z kolejki do wyniku.

- CalculateFunction

> Funkcja CalculateAnswer przechodzi przez wszystkie wyrażenia w
> działaniu. Liczby przenosi do kolejki. Jeżeli napotka działanie, to
> pobiera 2 liczby z kolejki i wykonuje odpowiadające działaniu
> operacje. Wszystkie działania są napisane w pliku Math.cpp. Jeżeli w
> kolejce jest tylko 1 liczba, zastępuje 2 liczbę słowem „buf" (niektóre
> działania takie jak np. pierwiastek wymagają tylko 1 liczby). Jeżeli
> zostały podane 2 liczby, a funkcja potrzebuje tylko 1, liczba ta wraca
> z powrotem do kolejki przed wynikiem. Pod koniec w kolejce zostaje
> tylko 1 liczba i jest ona wynikiem danego działania.

- Funkcjonalność

> Dzięki tej klasie program bez problemu potrafi poradzić sobie ze
> złożonymi działaniami uwzględniając działania +, -, \*, /, ˆ, sin,
> cos, tan, cot, arcsin, arccos, arctan, arccot, ln, log, abs, pierw
> oraz działania logiczne &&, \|\|, ==, !=, nie, \>, \<, \>=, \<=.
>
> Przykładowe działania:
>
> 4+3\*sin(x)+6
>
> nie(5\>6) && (5 == 7)

d)  Wstępne założenia projektu

> Celem było stworzenie gry **Snake** w całości w silniku
> **GudraEngine**, z wykorzystaniem dedykowanego języka. Projekt
> wymagał:

- dynamicznie odświeżanej mapy,

- reakcji na wejście użytkownika,

- zmiennych i tablic do przechowywania pozycji węża,

- obsługi klawiszy, warunków i pętli.

2.  Instrukcja użytkownika

> **a) Instrukcje**

- **jezeli(warunek)**

> Wykonuje kod w bloku, jeśli warunek jest prawdziwy.
>
> jezeli(a \> 5)
>
> {
>
> // kod
>
> }

- **dopoki(warunek)**

> Powtarza kod tak długo, jak warunek jest spełniony.
>
> dopoki(a \> 6)
>
> {
>
> // kod
>
> }
>
> **b) Zmienne**
>
> Dostępne typy danych:

- liczba --- np. 1, 2, 5

- rzeczywista --- np. 1.12, 4.323

- znak --- np. \'A\', \'B\'

- logiczny --- prawda, falsz

> **Deklaracja:**
>
> liczba a = 5
>
> znak b = \'A\' + \'C\' + 1
>
> **Modyfikacja:**
>
> liczba a = 1
>
> liczba a = a + 1
>
> **Zmienne globalne:**
>
> Aby zmienna zachowała wartość między plikami, należy dodać modyfikator
> globalny:
>
> globalny liczba a = 1
>
> **c) Tablice**
>
> Tablice tworzy się automatycznie poprzez przypisanie wartości do
> indeksu:
>
> liczba a = 1
>
> liczba a\[1\] = 5
>
> liczba a\[5\] = 4
>
> Wynik: a = {1, 5, 0, 0, 0, 4}
>
> **d) Funkcje**
>
> Funkcje wywołuje się przez nazwę i nawiasy z parametrami:
>
> nazwa_funkcji\[parametr1, parametr2\]
>
> **Lista funkcji:**

| **Nazwa** | **Działanie** |
| :--- | :--- |
| `klawisz['A']` | Zwraca prawda jeśli klawisz jest naciśnięty |
| `ustawna[x, y, z]` | Ustawia znak z na mapie w pozycji (x, y) |
| `zakoncz[]` | Kończy działanie programu |
| `wyczysc[]` | Czyści ekran/mapę |
| `ustawfps[fps]` | Ustawia liczbę klatek na sekundę |
| `ustawwielkoscmapy[szer, wys]` | Ustawia rozmiar mapy |
| `znakna[x, y]` | Zwraca znak z pozycji (x, y) |
| `linia[x1, y1, x2, y2, z]` | Rysuje linię znakiem z |
| `wstaw[t, i, v]` | Wstawia wartość v w indeksie i tablicy t |
| `usun[t, i]` | Usuwa element i z tablicy t |
| `rozmiar[t]` | Zwraca długość tablicy t |
| `losowa[min, max]` | Zwraca losową wartość z zakresu |
| `wyswietlliczbe[x, y, l]` | Wyświetla liczbę l na mapie od (x, y) |

e)  Edytor

- Aby poruszać się po zawartości tekstu należy używać strzałek

- W celu zapisania zmian należy nacisnąć przycisk Escape

- Edytor posiada podstawowe funkcjonalności jak przejście do nowej linii
  klawiszem Enter oraz usunięcie znaków przy użyciu klawisza Shift.
  Klawisz Tab nie ma żadnej funkcjonalności.

- Dostępne są tylko podstawowe litery, więc np. litera ą nie działa

f)  Menu

- W celu poruszania się po menu należy użyć strzałek. Aby zatwierdzić
  wybraną opcje należy nacisnąć klawisz Enter.

g)  Odtwarzanie projektu

> Projekt można wyłączyć w każdym momencie naciskając klawisz ESC.
> Reszta musi zostać napisana za pomocą kodu.

3.  Ciekawostki

- Nazwa języka **C\--** to żartobliwa kontra do języka C++, gdzie ++
  sugeruje rozwój -- tutaj mamy \--, czyli uproszczenie.

- Choć język nie jest szybki (ok. 20 000 instrukcji/sekundę), sprawdza
  się w grach konsolowych.

- Pierwsze testy wykonalności projektu odbyły się dopiero po 2
  miesiącach prac.

- Pierwszym działającym programem był klasyczny **Snake**.

- Do debugowania używano funkcji wywal, która dzieliła przez 0 i celowo
  wywoływała błąd.

4.  Posumowanie i wnioski

> GudraEngine w pełni zrealizował nasze założenia projektowe. Udało się
> stworzyć język, edytor i silnik pozwalający na tworzenie dynamicznych
> gier konsolowych. Stworzenie gry Snake w naszym własnym języku było
> ambitnym celem -- i został on osiągnięty. Mimo tego, że udało nam się
> osiągnąć nam cel, niektóre aspekty nie zostały ukończone. Nieukończone
> plany:

- Funkcja importująca zdjęcia w formacie ASCII oraz wyświetlająca je w
  podanym miejscu,

- Deklarowanie swoich funkcji,

- Funkcje odtwarzające dźwięki,

- Funkcje pozwalające na zmienianie koloru wyświetlanych części mapy,

- Dodatkowa konsola do wyświetlania wiadomości za pomocą kodu
