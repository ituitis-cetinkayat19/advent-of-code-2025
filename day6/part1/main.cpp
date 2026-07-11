#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

long long calculateResult(const vector<string>& v) {
  if (v.size() <= 1) return 0;
  string op = v[v.size() - 1];
  long long startValue = stoll(v[0]);
  for (int i = 1; i < v.size() - 1; i++) {
    if (op == "*") {
      startValue *= stoll(v[i]);
    } else if (op == "+") {
      startValue += stoll(v[i]);
    }
  }
  return startValue;
}

int main() {
  string lineText;
  ifstream InputFile("input.txt");
  vector<vector<string>> map;

  getline(InputFile, lineText);
  string word;
  stringstream ss(lineText);
  while (ss >> word) {
    vector<string> row = {word};
    map.push_back(row);
  }

  while (getline(InputFile, lineText)) {
    int index = 0;
    stringstream ss(lineText);
    while (ss >> word) {
      map[index].push_back(word);
      index++;
    }
  }
  /*
  for (const vector<string>& v : map) {
    for (const string& s : v)
      cout << s << " ";
    cout << "\n";
  }
  */
  long long total = 0;
  for (const vector<string>& v : map) {
    total += calculateResult(v);
  }
  cout << "Total: " << total;
  return 0;
}