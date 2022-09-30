# Automat do gier
Automat do gier, jednoręki bandyta, 777

Automat do gier jest wersją "gry jednorekibandyta przepisaną na C++ z dwudziestoma liniami wygrywającymi
"Gra" losuje domyślnie 100 tysięcy wierszy na pięciu bębnach spośród ośmiu symboli/cyfr.

Stawka wynosi 100, kredyt 500 tysięcy


Wszystkie losowania i zmieniającą się wartość kredytu można zapisać do pliku używając opcji `-plik`

Stosunek zwrotu jest na poziomie 96%.

Częstotliwość trafień wynosi 30%.



**Kompilacja:**

`c++ automatDoGier.cpp -o automatDoGier --std=c++17`

**Uruchomienie:**

`./automatDoGier`

lub

`./automatDoGier -gier 100000 -kredyt 500000 -plik plik.txt -bufor 1000`

lub

`./automatDoGier -symbole 4,4,6,0,3,6,6,6,6,6,3,2,6,7,4`

wykonanie polecenia poda wygraną za układ:

`4,4,6,0,3`

`6,6,6,6,6`

`3,2,6,7,4`


![alt text](https://github.com/ato-zen/automatDoGier/blob/main/wyniki.png?raw=true)

Przykładowy stan kredytu dla kilku tysięcy gier

![alt text](https://github.com/ato-zen/automatDoGier/blob/main/kredyt.png?raw=true)

