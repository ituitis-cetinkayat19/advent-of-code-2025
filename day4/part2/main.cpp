#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void displayGrid(const vector<vector<char>>& grid) {
  for (int x = 0; x < grid.size(); x++) {
    for (int y = 0; y < grid[x].size(); y++) {
      cout << grid[x][y];
    }
    cout << "\n";
  }
  cout << "\n\n";
}

void removeIfAble(vector<vector<char>>& grid, const int& x, const int& y, int& total) {
  if (grid[x][y] != '@') return;
  int nearbyCount = 0;
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size() || (x == i && y == j)) continue;
      if (grid[i][j] == '@') nearbyCount++;
    }
  }
  if (nearbyCount < 4) {
    grid[x][y] = '.';
    total++;
    //displayGrid(grid);
    for (int i = x - 1; i <= x + 1; i++) {
      for (int j = y - 1; j <= y + 1; j++) {
        if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size() || (x == i && y == j)) continue;
        removeIfAble(grid, i, j, total);
      }
    }
  }
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
  //displayGrid(grid);
  int total = 0;
  for (int x = 0; x < grid.size(); x++) {
    for (int y = 0; y < grid[x].size(); y++) {
      removeIfAble(grid, x, y, total);
    }
  }
  cout << "Total: " << total;
  return 0;
}