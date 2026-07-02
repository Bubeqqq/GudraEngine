Dokumentacja Projektu „GudraEngine"

Wydzia³ Matematyki stosowanej

Informatyka (Profil Praktyczny)

Rok studencki 2024/2025

B³a¿ej Skorzysko

Bartosz Wiêcek

1.  Opis „GudraEngine"

**GudraEngine** to prosty silnik stworzony z myœl¹ o tworzeniu gier
konsolowych, ale sprawdzi siê równie¿ w projektach algorytmicznych i
innych aplikacjach tekstowych. Silnik pozwala u¿ytkownikowi utworzyæ
nowy projekt w dowolnie wybranym folderze, a nastêpnie edytowaæ trzy
podstawowe pliki: **start**, **klatka** i **koniec**, które zawieraj¹
kod steruj¹cy dzia³aniem aplikacji.

Kod pisany jest w dedykowanym jêzyku **C\--**, który oferuje podstawowe
elementy potrzebne do programowania: cztery typy danych, obs³ugê tablic,
instrukcje warunkowe, pêtle, oraz funkcje przyjmuj¹ce argumenty i
zwracaj¹ce wartoœci.

a)  /\*Dok³adny opis edytora \*/

b)  Jêzyk C\--

- Kod Ÿród³owy jêzyka znajduje siê w folderze C\--. Najwa¿niejszym
  komponentem jest klasa Compiler, odpowiedzialna za interpretacjê kodu.
  Kompilator przetwarza kod linijka po linijce, analizuj¹c pierwszy
  token i podejmuj¹c odpowiednie dzia³anie.

<!-- -->

- Wspó³pracuje z czterema klasami pomocniczymi:

<!-- -->

- variableManager -- zarz¹dza zmiennymi,

- functionManager -- obs³uguje funkcje i ich wywo³ania,

- calculator -- odpowiada za obliczenia matematyczne i logiczne,

- Logs -- rejestruje b³êdy i zatrzymuje wykonanie w przypadku
  niepowodzeñ.

> **Szczegó³y dzia³ania:**

- **Instrukcja warunkowa jezeli**: sprawdza warunek i pomija kod w
  nawiasach, jeœli nie jest spe³niony.

- **Pêtla dopoki**: zapamiêtuje punkt startowy i powtarza blok dopóki
  warunek jest prawdziwy.

- **Deklaracje zmiennych**: s¹ przetwarzane, jeœli linia nie jest
  instrukcj¹ ani funkcj¹.

- **Zarz¹dzanie zmiennymi**: ka¿da zmienna jest traktowana jak tablica,
  nawet te proste.

- **Klasa Calculator**

- To kluczowy element jêzyka --- odpowiada za obliczenia. Wykorzystuje
  algorytm **Shunting Yard** do konwersji wyra¿eñ do notacji
  postfiksowej, co umo¿liwia obliczanie wartoœci za pomoc¹ stosu.
  Obs³uguje zmienne, sta³e oraz wywo³ania funkcji.

c)  Wstêpne za³o¿enia projektu

> Celem by³o stworzenie gry **Snake** w ca³oœci w silniku
> **GudraEngine**, z wykorzystaniem dedykowanego jêzyka. Projekt
> wymaga³:

- dynamicznie odœwie¿anej mapy,

- reakcji na wejœcie u¿ytkownika,

- zmiennych i tablic do przechowywania pozycji wê¿a,

- obs³ugi klawiszy, warunków i pêtli.

2.  Instrukcja u¿ytkownika

> **a) Instrukcje**

- **jezeli(warunek)**

> Wykonuje kod w bloku, jeœli warunek jest prawdziwy.
>
> jezeli(a \> 5)
>
> {
>
> // kod
>
> }

- **dopoki(warunek)**

> Powtarza kod tak d³ugo, jak warunek jest spe³niony.
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
> Dostêpne typy danych:

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
> Aby zmienna zachowa³a wartoœæ miêdzy plikami, nale¿y dodaæ modyfikator
> globalny:
>
> globalny liczba a = 1
>
> **c) Tablice**
>
> Tablice tworzy siê automatycznie poprzez przypisanie wartoœci do
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
> Funkcje wywo³uje siê przez nazwê i nawiasy z parametrami:
>
> nazwa_funkcji\[parametr1, parametr2\]
>
> **Lista funkcji:**

+--------------+-------------------------+-----------------------------------------------------------+
|              | **Nazwa**               | **Dzia³anie**                                             |
+==============+=========================+===================================+=======================+
|              | klawisz\[\'A\'\]        | Zwraca prawda jeœli klawisz jest naciœniêty               |
+--------------+-------------------------+-----------------------------------------------------------+
|              | ustawna\[x, y, z\]      | Ustawia znak z na mapie w pozycji (x, y)                  |
+--------------+-------------------------+-----------------------------------------------------------+
|              | zakoncz\[\]             | Koñczy dzia³anie programu                                 |
+--------------+-------------------------+-----------------------------------+-----------------------+
| wyczysc\[\]                            | Czyœci ekran/mapê                 |                       |
+----------------------------------------+-----------------------------------+-----------------------+
| ustawfps\[fps\]                        | Ustawia liczbê klatek na sekundê  |                       |
+----------------------------------------+-----------------------------------+-----------------------+
| ustawwielkoscmapy\[szer, wys\]         | Ustawia rozmiar mapy              |                       |
+----------------------------------------+-----------------------------------+-----------------------+
| znakna\[x, y\]                         | Zwraca znak z pozycji (x, y)      |                       |
+----------------------------------------+-----------------------------------+-----------------------+
| linia\[x1, y1, x2, y2, z\]             | Rysuje liniê znakiem z            |                       |
+----------------------------------------+-----------------------------------+-----------------------+
| wstaw\[t, i, v\]                       | Wstawia wartoœæ v w indeksie i    |                       |
|                                        | tablicy t                         |                       |
+----------------------------------------+-----------------------------------+-----------------------+
| usun\[t, i\]                           | Usuwa element i z tablicy t       |                       |
+----------------------------------------+-----------------------------------+-----------------------+
| rozmiar\[t\]                           | Zwraca d³ugoœæ tablicy t          |                       |
+----------------------------------------+-----------------------------------+-----------------------+
| losowa\[min, max\]                     | Zwraca losow¹ wartoœæ z zakresu   |                       |
+----------------------------------------+-----------------------------------+-----------------------+
| wyswietlliczbe\[x, y, l\]              | Wyœwietla liczbê l na mapie od    |                       |
|                                        | (x, y)                            |                       |
+----------------------------------------+-----------------------------------+-----------------------+

3.  Ciekawostki

- Nazwa jêzyka **C\--** to ¿artobliwa kontra do jêzyka C++, gdzie ++
  sugeruje rozwój -- tutaj mamy \--, czyli uproszczenie.

- Choæ jêzyk nie jest szybki (ok. 20 000 instrukcji/sekundê), sprawdza
  siê w grach konsolowych.

- Pierwsze testy wykonalnoœci projektu odby³y siê dopiero po 2
  miesi¹cach prac.

- Pierwszym dzia³aj¹cym programem by³ klasyczny **Snake**.

- Do debugowania u¿ywano funkcji wywal, która dzieli³a przez 0 i celowo
  wywo³ywa³a b³¹d.

4.  Posumowanie i wnioski

> GudraEngine w pe³ni zrealizowa³ nasze za³o¿enia projektowe. Uda³o siê
> stworzyæ jêzyk, edytor i silnik pozwalaj¹cy na tworzenie dynamicznych
> gier konsolowych. Stworzenie gry Snake w naszym w³asnym jêzyku by³o
> ambitnym celem -- i zosta³ on osi¹gniêty.
