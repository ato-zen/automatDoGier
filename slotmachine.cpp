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
#include "automatDoGier.hpp"

#define SYMBOLS "-symbols"
#define CREDIT  "-credit"
#define BUFFER  "-buffer"
#define GAMES   "-games"
#define FILE_ARG "-file"

using namespace std;

int calculateWin();

struct {
  vector<string> symbols;
  int b5x3[15];
  string file;
  bool file_b = false;
  int buffer = 0;
  int credit = 500000, games = 100000, bet = 100, symbol[8] = {0,1,2,3,4,5,6,7};
} u;

unsigned long wins[8][5], row[5], winner, hits, total, totalWins, rtp, hf;

void error(string errorMessage){ cout << errorMessage; exit(0); }

void play(){
  stringstream ss;
  ofstream fileStream;
  int position[5], win;
  int rowX3[15];

  cout << setprecision(2) << fixed;

  srand(time(NULL));

  if(u.file_b){
    try {
      fileStream.open(u.file);
      if (fileStream.fail()) throw u.file;
    } catch (string e) {
      error("Cannot open file: " + e + "\n");
    }
  }

  for (size_t i = 0; i < 5; i++) {
    for (size_t j = 0; j < DB + 2; j++) {
      if(j == 0) b2[i][DB + 2] = beben[i][0];
      else if(j == DB + 2) b2[i][0] = beben[i][DB];
      else b2[i][j] = beben[i][j - 1];
    }
  }

  cout << "Games: " << u.games << ", Bet: " << u.bet << ", Credit: " << u.credit << "\n";

  for (int i = 0; i < u.games; i++) {
    total += u.bet;
    u.credit -= u.bet;

    // draw positions on 5 reels
    for (size_t j = 0; j < 5; j++) position[j] = rand() % DB;

    // create a row consisting of three (k) rows and 5 reels
    for (size_t k = -1, j = 0; j < 15; j++) {
      if(j % 10 == 0 || j % 5 == 0) k++;
      u.b5x3[j] = u.symbol[b2[j % 5][position[j % 5] + k]];
    }

    win = calculateWin();

    if(u.file_b){
      if(u.buffer){
        ss << i << ",";
        for (size_t j = 0; j < 15; j++) ss << u.b5x3[tp2[j]] << ",";
        ss << u.credit << "," << win << endl;
        if(i % u.buffer) {
          fileStream << ss.str();
          ss.str("");
        }
      } else {
        ss << i << ", ";
        for (size_t j = 0; j < 15; j++) ss << u.b5x3[tp2[j]] << ",";
        ss << u.credit << "," << win << endl;
        fileStream << ss.str();
        ss.str("");
      }
    }
  }

  if(u.file_b && u.buffer) fileStream << ss.str();

  cout << "Total Bet: " << total << ", Total Wins: " << totalWins << ", Balance: " << u.credit << "\n";
  cout << "Number of Wins:\n\t[x1]\t[x2]\t[x3]\t[x4]\t[x5]\n";
  for (int i = 0; i < 8; i++) cout << i << ":\t" << wins[i][0] << ",\t" << wins[i][1]
    << ",\t" << wins[i][2] << ",\t" << wins[i][3] << ",\t" << wins[i][4] << "\n";
  cout << "RTP: " << (100.0 * totalWins / total) << "\n";
  cout << "HF: " << (100.0 * hf / u.games) << "\n";

  if(u.file_b) fileStream.close();
}

int calculateWin(){
  int win = 0, totalWin = 0, i = 0;
  bool hit = false;
  hits = 0;

  // calculate win for special symbol 7
  winner = 7;

  for (size_t i = 0; i < 15; i++) {
    if(u.b5x3[i] == winner) hits++;
    if(hits > 4) break; // no need to count further, we do not expect more hits
  }

  if(hits > 0) // if there is at least one hit
  if(--hits > 1){ // decrease by one to read the correct value from the table
    win = stawki[winner][hits];
    totalWin = win;
    totalWins += win;
    u.credit += win;
    wins[winner][hits]++;
    hit = true;
  }

  for (size_t i = 0; i < 20; i++) {
    // always the first one wins in 20 layouts
    winner = u.b5x3[w20[i][0]];
    if(winner == 7) break; // this symbol "7" has already been counted
    hits = 0;

    // count how many times the first one repeats
    for (size_t j = 1; j < 5; j++) {
      if(u.b5x3[w20[i][j]] == winner) hits++;
      else break; // if the symbol is different, do not check further
    }

    // if 0 occurs at least twice it is already a "win"
    // other symbols must occur at least 3 times
    if((winner == 0 && hits == 1) || hits > 1) {
      win = stawki[winner][hits];
      totalWin += win;
      totalWins += win;
      u.credit += win;
      wins[winner][hits]++;
      hit = true;
    }
  }

  // even several hits are treated as one win
  if(hit) hf++;

  return totalWin;
};

void split(const string s1, const string s2) {
    regex re(s2);
    copy(sregex_token_iterator(s1.begin(), s1.end(), re, -1),
       sregex_token_iterator(), back_inserter(u.symbols));
};

int main(int argc , char **argv){
  enum argument {games = 1, credit, file, buffer, symbols};
  map<string, argument> arguments;

  auto start = chrono::high_resolution_clock::now();

  int k, tab[15];

  if((argc % 2) == 1){

    arguments[GAMES]    = games;
    arguments[CREDIT]   = credit;
    arguments[FILE_ARG] = file;
    arguments[BUFFER]   = buffer;
    arguments[SYMBOLS]  = symbols;

    for (int number, i = 1; i < argc; i += 2) {

      switch(arguments[argv[i]]){
        case games:
          number = atoi(argv[i + 1]);
          if(number <= 0) error("Games must be more than 0, you provided "s + argv[i + 1] + "\n");
          else u.games = number;
          break;

        case credit:
          number = atoi(argv[i + 1]);
          if(number <= 0) error("Credit must be more than 0, you provided "s + argv[i + 1] + "\n");
          else u.credit = number;
          break;

        case file:
          u.file = argv[i + 1];
          u.file_b = true;
          break;

        case buffer:
          number = atoi(argv[i + 1]);
          if(number <= 0) error("Buffer must be a natural number, you provided "s + argv[i + 1] + "\n");
          else u.buffer = number;
          break;

        case symbols:
          split(argv[i + 1], "[,]+");

          number = u.symbols.size();

          if(number != 15)
            error("Vector must contain 15 digits, contains "s + to_string(number) + "\n");

          number = 0;
          k = 0;

          for(auto j: u.symbols) {
            number = stoi(j);
            if((number < 0) || (number > 7))
              error("Vector value is out of range\nPosition: "s + to_string(k) + ", value: " + to_string(number) + "\n");
            else tab[k++] = number;
          }

          // arrange in the order needed to calculate the win
          for (size_t i = 0; i < 15; i++) u.b5x3[i] = tab[tp1[i]];

          cout << calculateWin() << endl;
          return 0;
          break;

        default:
          error("Unknown argument: "s + argv[i] + "\nExample usage:\n"
            + argv[0] + " " + GAMES + " 100000 " + CREDIT + " 500000 " + FILE_ARG + " file.txt\n");
          break;
        }
      }

    } else {
      error("Provide the correct number of arguments\nExample usage:\n"s
        + argv[0] + " " + GAMES + " 100000 " + CREDIT + " 500000 " + FILE_ARG + " file.txt\n");
  }

  play();

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
  cout << "Program execution time: " << (duration / 1000.0) << "s\n";

  return 0;
}
