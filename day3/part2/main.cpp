#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

long long combineDigits(const vector<int>& digits) {
  string joinedDigits;
  for (const int& digit : digits) {
    joinedDigits += to_string(digit);
  }
  return stoll(joinedDigits);
}

void calculateNextJoltage(vector<int>& selectedJoltages, const int& it) {
  selectedJoltages.push_back(it);
  for (int i = 0; i < selectedJoltages.size() - 1; i++) {
    if (selectedJoltages[i] < selectedJoltages[i + 1]) {
      selectedJoltages.erase(selectedJoltages.begin() + i);
      return;
    }
  }
  selectedJoltages.erase(selectedJoltages.begin() + selectedJoltages.size() - 1);
}

long long calculateBankJoltage(const vector<int>& bank) {
  if (bank.size() == 0) return 0;
  if (bank.size() <= 12) return combineDigits(bank);
  vector<int> selectedJoltages;
  for (int i = 0; i < 12; i++) {
    selectedJoltages.push_back(bank[i]);
  }
  for (int i = 12; i < bank.size(); i++) { //in selectedJoltages remove the first number thats less than its successor
    calculateNextJoltage(selectedJoltages, bank[i]);
  }
  return combineDigits(selectedJoltages);
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
  long long totalJoltage = 0;
  for (int i = 0; i < banks.size(); i++) {
    long long bankJoltage = calculateBankJoltage(banks[i]);
    cout << "Bank " << i + 1 << " joltage: " << bankJoltage << "\n";
    totalJoltage += bankJoltage;
  }
  cout << "Total: " << totalJoltage;
  return 0;
}