#include <regex>
#include <vector>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <filesystem>
#include <string>
#include <map>
#include "automatDoGier.h"

#define SYMBOLE "-symbole"
#define KREDYT  "-kredyt"
#define BUFOR   "-bufor"
#define GIER    "-gier"
#define PLIK    "-plik"

using namespace std;
int obliczWygrana();

struct{
  vector<string> symbole;
  int b5x3[15];
  string plik;
  bool plik_b = false;
  int bufor = 0;
  int kredyt = 500000, gier = 100000, stawka = 100, znak[8] = {0,1,2,3,4,5,6,7};
} u;

unsigned long wygrane[8][5], wiersz[5], wygrywa, trafien, razem, wygranych, rtp, hf;

void blad(string blad){ cout<<blad; exit(0); }

void graj(){
  stringstream ss;
  ofstream file;
  int pozycja[5], wygrana;
  int wierszX3[15];

  cout << setprecision(2) << fixed;

  srand(time(NULL));

  if(u.plik_b){
    try {
      file.open (u.plik);
      if (file.fail()) throw u.plik;
    } catch (string e) {
      blad("Nie można otworzyć pliku: "+e+"\n");
    }
  }

  for (size_t i = 0; i < 5; i++) {
    for (size_t j = 0; j < DB+2; j++) {
      if(j==0) b2[i][DB+2] = beben[i][0];
      else if(j==DB+2) b2[i][0] = beben[i][DB];
      else b2[i][j] = beben[i][j-1];
    }
  }

  cout<<"Gier: "<<u.gier<<", Stawka: "<<u.stawka<<", Kredyt: "<<u.kredyt<<"\n";

  for (int i = 0; i < u.gier; i++) {
    razem += u.stawka;
    u.kredyt -= u.stawka;

    // wylosowanie pozycji na 5-ciu bębnach
    for (size_t j = 0; j < 5; j++) pozycja[j] = rand()%DB;

    // utworzenie wiersza składającego się z trzech (k) wierszy i 5 bębnów
    for (size_t k=-1, j = 0; j < 15; j++) {
      if(j%10==0 || j%5==0) k++;
      u.b5x3[j] = u.znak[b2[j%5][pozycja[j%5]+k]];
    }

    wygrana = obliczWygrana();

    if(u.plik_b){
      if(u.bufor){
        ss<<i<<",";
        for (size_t j = 0; j < 15; j++) ss<<u.b5x3[tp2[j]]<<",";
        ss<<u.kredyt<<","<<wygrana<<endl;
        if(i%u.bufor) {
          file<<ss.str();
          ss.str("");
        }
      } else {
        ss<<i<<", ";
        for (size_t j = 0; j < 15; j++) ss<<u.b5x3[tp2[j]]<<",";
        ss<<u.kredyt<<","<<wygrana<<endl;
        file<<ss.str();
        ss.str("");
      }
    }

  }

  if(u.plik_b && u.bufor) file<<ss.str();

  cout<<"Postawione: "<<razem<<", Wygrane: "<<wygranych<<", Bilans: "<<u.kredyt<<"\n";
  cout<<"Ilość wygranych:\n\t[x1]\t[x2]\t[x3]\t[x4]\t[x5]\n";
  for (int i=0; i < 8; i++) cout<<i<<":\t"<<wygrane[i][0]<<",\t"<<wygrane[i][1]
    <<",\t"<<wygrane[i][2]<<",\t"<<wygrane[i][3]<<",\t"<<wygrane[i][4]<<"\n";
  cout<<"RTP: "<<(100.0*wygranych/razem)<<"\n";
  cout<<"HF: "<<(100.0*hf/u.gier)<<"\n";

  if(u.plik_b) file.close();
}

int obliczWygrana(){
  int wygrana = 0, razem = 0, i = 0;
  bool hit = false;
  trafien=0;

  //obliczenie wygranej dla symbolu specjalnego 7
  wygrywa = 7;

  for (size_t i = 0; i < 15; i++) {
    if(u.b5x3[i]==wygrywa) trafien++;
    if(trafien > 4) break; // dalej już nie musimy liczyć bo więcej trafień
                           // nie przewidujemy
  }

  if( trafien > 0 ) // jeżeli jest choć jedno trafienie
  if(--trafien > 1){// zmniejsz o jeden aby odczytać właściwą wartość z tabeli
    wygrana = stawki[wygrywa][trafien];
    razem = wygrana;
    wygranych += wygrana;
    u.kredyt += wygrana;
    wygrane[wygrywa][trafien]++;
    hit = true;
  }


  for (size_t i = 0; i < 20; i++) {
    // wygrywa zawsze pierwszy w 20-stu układach
    wygrywa = u.b5x3[w20[i][0]];
    if(wygrywa == 7) break; // ten symbol "7" już był liczony
    trafien=0;

    // zliczenie ile razy ten pierwszy się powtórzy
    for (size_t j=1; j < 5; j++) {
      if(u.b5x3[w20[i][j]]==wygrywa) trafien++;
      else break; // jeżeli znak jest inny to dalej nie sprawdzamy
    }

    // jeżeli 0 wystąpi choć dwa razy to już jest "wygrana"
    // pozostałe znaki muszą wystąpić co najmniej 3 razy
    if((wygrywa==0 && trafien==1) || trafien>1) {
      wygrana = stawki[wygrywa][trafien];
      razem += wygrana;
      wygranych += wygrana;
      u.kredyt += wygrana;
      wygrane[wygrywa][trafien]++;
      hit = true;
    }
  }

  // nawet jak będzie kilk wygranych traktujemy je jako jedno
  if(hit) hf++;

  return razem;
};

void split( const string s1, const string s2 )
{
    regex re(s2);
    copy( sregex_token_iterator(s1.begin(), s1.end(), re, -1),
       sregex_token_iterator(), back_inserter(u.symbole));
};

int main(int argc , char **argv){
  enum argument {gier=1, kredyt, plik, bufor, symbole};
  map<string, argument> argumenty;

  auto start = chrono::high_resolution_clock::now();

  int k, tab[15];

  if((argc % 2)==1){

    argumenty[GIER]     = gier;
    argumenty[KREDYT]   = kredyt;
    argumenty[PLIK]     = plik;
    argumenty[BUFOR]    = bufor;
    argumenty[SYMBOLE]  = symbole;

    for (int liczba, i = 1; i < argc; i+=2) {

      switch( argumenty[argv[i]] ){
        case gier:
          liczba = atoi(argv[i+1]);
          if(liczba<=0) blad("Gier musi być więcej niż 0, podałeś "s+argv[i+1]+"\n");
          else u.gier = liczba;
          break;

        case kredyt:
          liczba = atoi(argv[i+1]);
          if(liczba<=0) blad("Kredyt musi być większy niż 0, podałeś "s+argv[i+1]+"\n");
          else u.kredyt = liczba;
          break;

        case plik:
          u.plik =  argv[i+1];
          u.plik_b = true;
          break;

        case bufor:
          liczba = atoi(argv[i+1]);
          if(liczba<=0) blad("Bufor musi być liczbą naturalną, podałeś "s+argv[i+1]+"\n");
          else u.bufor = liczba;
          break;

        case symbole:
          split(argv[i+1], "[,]+");

          liczba = u.symbole.size();

          if(liczba!=15)
            blad("Wektor musi zawierać 15 cyfr, zawiera "s+to_string(liczba)+"\n");

          liczba = 0;
          k = 0;

          for(auto j: u.symbole) {
            liczba = stoi(j);
            if((liczba<0) || (liczba>7))
              blad("Wartość wektora jest poza zakresem\nPozycja: "s
                +to_string(k)+", wartość: "+to_string(liczba)+"\n");
            else tab[k++] = liczba;
          }

          //uporządkowanie w kolejności potrzebnej do wyliczenia wygranej
          for (size_t i = 0; i < 15; i++) u.b5x3[i] = tab[tp1[i]];

          cout << obliczWygrana() << endl;
          return 0;
          break;

        default:
          blad("Nieznany argument: "s+argv[i]+"\nPrzykład uruchomienia:\n"
            +argv[0]+" "+GIER+" 100000 "+KREDYT+" 500000 "+PLIK+" plik.txt\n");
          break;
        }
      }

    } else {
      blad("Podaj poprawną ilość argumentów\nPrzykład uruchomienia:\n"s
        +argv[0]+" "+GIER+" 100000 "+KREDYT+" 500000 "+PLIK+" plik.txt\n");
  }

  graj();

  auto stop = chrono::high_resolution_clock::now();
  auto czas = chrono::duration_cast<chrono::milliseconds>(stop-start).count();
  cout << "Czas wykonania programu : " << (czas/1000.0) << "s\n";

  return 0;
}
