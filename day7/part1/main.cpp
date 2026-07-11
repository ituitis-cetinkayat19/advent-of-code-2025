#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int calculateResult(vector<string>& map) {
  int total = 0;
  for (int i = 1; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      if (map[i-1][j] == 'S' || map[i-1][j] == '|') {
        if (map[i][j] == '^') {
          if (j > 0) map[i][j-1] = '|';
          if (j < map[i].size() - 1) map[i][j+1] = '|';
          total++;
        } else {
          map[i][j] = '|';
        }
      }
    }
    /*
    for (const string& s : map) {
      cout << s << "\n";
    }
    */
  }
  return total;
}

int main() {
  string lineText;
  ifstream InputFile("input.txt");
  vector<string> map;

  while (getline(InputFile, lineText)) {
    map.push_back(lineText);
  }

  cout << "Total: " << calculateResult(map);
  
  return 0;
}