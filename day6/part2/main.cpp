#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

long long calculateOperation(const vector<long long>& v, const string& op) {
  if (v.size() == 0) return 0;
  if (v.size() == 1) return v[0];
  long long result = v[0];
  for (int i = 1; i < v.size(); i++) {
    if (op == "*") {
      result *= v[i];
    } else if (op == "+") {
      result += v[i];
    }
  }
  return result;
}

long long calculateTotal(const vector<string>& v) {
  vector<string> ops;
  string opString = v[v.size() - 1];
  string op;
  stringstream ss(opString);
  while (ss >> op) {
    ops.push_back(op);
  }

  int lineSize = v[0].size();
  vector<long long> numbersCollected;
  long long total = 0;
  int opIndex = 0;
  for (int i = 0; i < lineSize + 1; i++) {
    string letterAtIndex;
    for (int j = 0; j < v.size() - 1; j++) {
      if (i < v[j].size() && v[j][i] != ' ') {
        letterAtIndex.push_back(v[j][i]);
      }
    }
    if (letterAtIndex.size() > 0) {
      numbersCollected.push_back(stoll(letterAtIndex));
    } else {
      total += calculateOperation(numbersCollected, ops[opIndex]);
      opIndex++;
      numbersCollected.clear();
    }
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
  
  cout << "Total: " << calculateTotal(map);
  return 0;
}