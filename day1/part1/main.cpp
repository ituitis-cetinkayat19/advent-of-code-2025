#include <iostream>
#include <fstream>
using namespace std;

void calculateMove(int& currentPos, int& totalZeros, const char& moveDirection, const int& moveLength) {
  if (moveDirection == 'L') {
    currentPos -= moveLength;
  } else if (moveDirection == 'R') {
    currentPos += moveLength;
  }
  currentPos %= 100;
  if (currentPos == 0) totalZeros += 1;
}

int main() {
  int currentPos = 50, totalZeros = 0;
  string lineText;
  ifstream InputFile("input.txt");
  while (getline(InputFile, lineText)) {
    char moveDirection = lineText[0];
    int moveLength = stoi(lineText.substr(1));
    calculateMove(currentPos, totalZeros, moveDirection, moveLength);
  }
  cout << "Current Position: " << currentPos << "\n";
  cout << "Password: " << totalZeros << "\n";
  return 0;
}