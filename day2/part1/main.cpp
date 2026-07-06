#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
using namespace std;

/*
2 digits -> 9

4 digits -> 90

6 digits -> 900

n digits -> 9 * 10 ^ (n/2 - 1)

100100 
101101
102102
199199
*/

long long longPow(long long base, long long exponent) {
  long long result = 1;
  while(exponent > 0) {
    result *= base;
    exponent--;
  }
  return result;
}

vector<pair<long long, long long>> splitInput(const string& lineText) {
  vector<pair<long long, long long>> vector;
  stringstream ss(lineText);
  string pairText;
  while (getline(ss, pairText, ',')) {
    pair<long long, long long> pair;
    int splitIndex = pairText.find('-');
    pair.first = stoll(pairText.substr(0, splitIndex));
    pair.second = stoll(pairText.substr(splitIndex + 1));
    vector.push_back(pair);
  }
  return vector;
}

int getDigits(long long number) {
  int digits = 1;
  while (number % 10 != number) {
    number /= 10;
    digits++;
  }
  return digits;
}

long long getStartNumber(const int& curDigits, const long long& first) {
  int halfDigit = curDigits / 2;
  if (curDigits != getDigits(first)) {
    return longPow(10, halfDigit - 1);
  }
  // Ex: first: 1234 -> startPart 12? -> 1212 < 1234 -> startPart 13
  // startPart = first / 10 ^ halfDigit
  long long startPart = first / longPow(10, halfDigit);
  if (startPart + startPart * longPow(10, halfDigit) < first) startPart++;
  return startPart;
}

long long getEndNumber(const int& curDigits, const long long& second) {
  int halfDigit = curDigits / 2;
  if (curDigits != getDigits(second)) {
    return longPow(10, halfDigit) - 1;
  }
  // Ex: second: 4321 -> endPart 43? -> 4343 > 4321 -> endPart 42
  // endPart = second / 10 ^ halfDigit
  long long endPart = second / longPow(10, halfDigit);
  if (endPart + endPart * longPow(10, halfDigit) > second) endPart--;
  return endPart;
}

long long invalidIdTotal(const long long& first, const long long& second) {
  int firstDigits = getDigits(first);
  int secondDigits = getDigits(second);
  long long total = 0;
  for (int curDigits = firstDigits; curDigits <= secondDigits; curDigits++) {
    if (curDigits % 2 == 1) continue;
    int halfDigit = curDigits / 2;
    long long startNumber = getStartNumber(curDigits, first);
    long long endNumber = getEndNumber(curDigits, second);
    for (long long cur = startNumber; cur <= endNumber; cur++) {
      long long invalidId = cur + cur * longPow(10, halfDigit);
      total += invalidId;
    }
  }
  return total;
}

int main() {
  string lineText;
  ifstream InputFile("input.txt");
  getline(InputFile, lineText);
  vector<pair<long long, long long>> ranges = splitInput(lineText);
  long long total = 0;
  for (int i = 0; i < ranges.size(); i++) {
    pair<long long, long long> range = ranges[i];
    total += invalidIdTotal(range.first, range.second);
  }
  cout << "Total: " << total;
  return 0;
}