#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

bool canAccess(const vector<vector<char>>& grid, int& x, int& y) {
  if (grid[x][y] != '@') return false;
  int nearbyCount = 0;
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size() || (x == i && y == j)) continue;
      if (grid[i][j] == '@') nearbyCount++;
    }
  }
  return nearbyCount < 4;
}

int main() {
  string lineText;
  ifstream InputFile("input.txt");
  vector<vector<char>> grid;
  while (getline(InputFile, lineText)) {
    vector<char> row;
    for (char& c : lineText) {
      row.push_back(c);
    }
    grid.push_back(row);
  }
  int total = 0;
  for (int x = 0; x < grid.size(); x++) {
    for (int y = 0; y < grid[x].size(); y++) {
      if (canAccess(grid, x, y)) total += 1;
    }
  }
  cout << "Total: " << total;
  return 0;
}