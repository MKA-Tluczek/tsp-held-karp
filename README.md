# Travelling Salesman Problem - Held-Karp
Aplikacja rozwiązująca problem komiwojażera za pomocą algorytmu Helda-Karpa.

## Uruchomienie
Aby uruchomić aplikację, w tym samym katalogu co aplikacja musi znajdować się plik `settings.ini`.

W pierwszej linii tego pliku należy podać nazwę pliku wyjściowego, do którego zostaną zapisane rozwiązania.

Każda kolejna linia zawiera nazwę pliku z którego aplikacja ma wczytać informacje o grafie, po czym liczbę całkowitą, ile razy aplikacja ma obliczyć rozwiązanie danego grafu.[^1]

## Struktura plików wejściowych i wyjściowych
Plik wejściowy zaczyna się od jednej liczby `N` - liczby wierzchołków w grafie.

Następnie reszta pliku to `N` wierszy, zawierających `N` liczb każdy. `I`-ta liczba w `J`-tym wierszu zawiera odległość od wierzchołka `J` do wierzchołka `I`.

Plik wyjściowy zawiera po jednym wierszu na każde uruchomienie algorytmu. Każdy wiersz zawiera następujące informacje, oddzielone przecinkami:
* Nazwa pliku wejściowego
* Czas obliczenia rozwiązania w sekundach
* Długość optymalnej ścieżki
* Kolejne wierzchołki optymalnej ścieżki (indeksy liczone od zera)

[^1]: Funkcja wielokrotnego obliczania tej samej instancji została zaimplementowana w celu szacowania średniej długości działania algorytmu.

<sub>Aplikację wykonano w listopadzie 2023 w ramach kursu "Projektowanie efektywnych algorytmów".</sub>
