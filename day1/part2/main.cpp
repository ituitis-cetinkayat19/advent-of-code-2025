#include <iostream>
#include <fstream>
using namespace std;

void calculateMove(int& currentPos, int& totalZeros, const char& moveDirection, const int& moveLength) {
  if (moveDirection == 'L') {
    int oldPos = currentPos;
    currentPos -= moveLength;
    if (currentPos <= 0) {
      totalZeros += (oldPos != 0 ? 1 : 0) + abs(currentPos / 100);
      currentPos = ((currentPos % 100) + 100) % 100;
    }
  } else if (moveDirection == 'R') {
    currentPos += moveLength;
    totalZeros += currentPos / 100;
    currentPos %= 100;
  }
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