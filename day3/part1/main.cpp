#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int combineDigits(const int& x, const int& y) {
  return 10 * x + y;
}

void calculateNextJoltage(int& first, int& second, const int& it) {
  int joltage = combineDigits(first, second);
  int leaveFirst = combineDigits(second, it);
  int leaveSecond = combineDigits(first, it);
  int nextFirst, nextSecond;
  if (leaveFirst >= joltage && leaveFirst >= leaveSecond) {
    nextFirst = second;
    nextSecond = it;
  } else if (leaveSecond >= joltage && leaveSecond >= leaveFirst) {
    nextFirst = first;
    nextSecond = it;
  } else {
    nextFirst = first;
    nextSecond = second;
  }
  first = nextFirst;
  second = nextSecond;
}

int calculateBankJoltage(const vector<int>& bank) {
  if (bank.size() == 0) return 0;
  if (bank.size() == 1) return bank[0];
  if (bank.size() == 2) return combineDigits(bank[0], bank[1]);
  int firstDigit = bank[0];
  int secondDigit = bank[1];
  for(int i = 2; i < bank.size(); i++) {
    calculateNextJoltage(firstDigit, secondDigit, bank[i]);
  }
  return combineDigits(firstDigit, secondDigit);
}

int main() {
  string lineText;
  ifstream InputFile("input.txt");
  vector<vector<int>> banks;
  while (getline(InputFile, lineText)) {
    vector<int> bank;
    for (char& c : lineText) {
      bank.push_back(c - '0');
    }
    banks.push_back(bank);
  }
  int totalJoltage = 0;
  for (int i = 0; i < banks.size(); i++) {
    int bankJoltage = calculateBankJoltage(banks[i]);
    cout << "Bank " << i + 1 << " joltage: " << bankJoltage << "\n";
    totalJoltage += bankJoltage;
  }
  cout << "Total: " << totalJoltage;
  return 0;
}