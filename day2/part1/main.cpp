#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

/*
2 basamaklı -> 9

4 basamaklı -> 90

6 basamaklı -> 900

n basamaklı -> 9 * 10 ^ (n/2 - 1)

100100 
101101
102102
199199
*/

vector<pair<long, long>> splitInput(const string& lineText) {
  vector<pair<long, long>> vector;
  stringstream ss(lineText);
  string pairText;
  while (getline(ss, pairText, ',')) {
    pair<long, long> pair;
    int splitIndex = pairText.find('-');
    pair.first = stol(pairText.substr(0, splitIndex));
    pair.second = stol(pairText.substr(splitIndex + 1));
    vector.push_back(pair);
  }
  return vector;
}

int main() {
  string lineText;
  ifstream InputFile("test-input.txt");
  getline(InputFile, lineText);
  vector<pair<long, long>> ranges = splitInput(lineText);
  return 0;
}