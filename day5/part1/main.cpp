#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

bool isFresh(const vector<pair<long long, long long>>& ranges, const long long& id) {
  for (const pair<long long, long long>& range : ranges) {
    long long first = range.first;
    long long second = range.second;
    if (id >= first && id <= second) return true;
  }
  return false;
}

int main() {
  string lineText;
  ifstream InputFile("input.txt");
  vector<pair<long long, long long>> freshRanges;
  vector<long long> idsToCheck;
  while (getline(InputFile, lineText) && !lineText.empty()) {
    int splitIndex = lineText.find('-');
    long long first = stoll(lineText.substr(0, splitIndex));
    long long second = stoll(lineText.substr(splitIndex + 1));
    pair<long long, long long> range = {first, second};
    freshRanges.push_back(range);
  }
  while (getline(InputFile, lineText)) {
    idsToCheck.push_back(stoll(lineText));
  }
  int total = 0;
  for (long long& id : idsToCheck) {
    if (isFresh(freshRanges, id)) total++;
  }
  cout << "Total: " << total;
  return 0;
}