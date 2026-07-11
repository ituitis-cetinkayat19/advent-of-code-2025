#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Splitter {
  public:
    long long value;
    int i;
    int j;
    Splitter* left;
    Splitter* right;
    Splitter(int x, int y, long long val) : i(x), j(y), value(val), left(nullptr), right(nullptr) {}
    Splitter() {}
};

long long calculateResult(const vector<string>& map) {
  vector<Splitter> splitters;
  pair<int, int> start;
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      if (map[i][j] == 'S') start = {i , j};
      else if (map[i][j] == '^') {
        Splitter newSplitter = Splitter(i, j, 0);
        if (splitters.size() == 0) {
          newSplitter.value = 1;
        } else {
          for (int i = splitters.size() - 1; i >= 0; i--) {
            Splitter& oldSplitter = splitters[i]; // a splitter can have multiple parents of same direction, but it can only have one child per direction
            if (oldSplitter.j == newSplitter.j) break; // another splitter directly above, end parent check
            if (oldSplitter.i < newSplitter.i) {
              if (oldSplitter.j == newSplitter.j - 1 && oldSplitter.right == nullptr) {
                oldSplitter.right = &newSplitter;
                newSplitter.value += oldSplitter.value;
              } else if (oldSplitter.j == newSplitter.j + 1 && oldSplitter.left == nullptr) {
                oldSplitter.left = &newSplitter;
                newSplitter.value += oldSplitter.value;
              }
            }
          }
        }
        splitters.push_back(newSplitter);
      }
    }
  }
  long long total = 0;
  for (const Splitter& splitter : splitters) {
    if (splitter.left == nullptr) total += splitter.value;
    if (splitter.right == nullptr) total += splitter.value;
  }
  return total;
}

int main() {
  string lineText;
  ifstream InputFile("input.txt");
  vector<string> map;

  while (getline(InputFile, lineText)) { // for every splitter, calculate how many ways you can reach that splitter
    map.push_back(lineText);
  }

  cout << "Total: " << calculateResult(map);
  
  return 0;
}