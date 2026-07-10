#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>
#include <math.h>
using namespace std;

long long longPow(long long base, long long exponent) {
  long long result = 1;
  while(exponent > 0) {
    result *= base;
    exponent--;
  }
  return result;
}

int getDigits(long long number) {
  int digits = 1;
  while (number % 10 != number) {
    number /= 10;
    digits++;
  }
  return digits;
}

bool multipleDoesNotExist(const int& i, const vector<int>& v) {
  for (const int& x : v) {
    if (x % i == 0) return false;
  }
  return true;
}

vector<int> getSubDigits(const int& curDigits) {
  if (curDigits == 1) return {};
  vector<int> result;
  for (int i = curDigits - 1; i > 1; i--) {
    if (curDigits % i == 0 && multipleDoesNotExist(i, result)) {
      result.push_back(i);
    }
  }
  if (result.size() == 0) return {1};
  return result;
}

long long getRepeatingNumber(const long long& part, const int& length) {
  //Ex: part = 12, length = 6 -> 121212
  int partDigits = getDigits(part);
  int repeat = length / partDigits;
  string result = "";
  for (int i = 0; i < repeat; i++) {
    result.append(to_string(part));
  }
  return stoll(result);
}

long long getStartNumber(const int& subDigit, const int& curDigits, const long long& first) {
  int firstDigits = getDigits(first);
  if (curDigits > firstDigits) return longPow(10, subDigit - 1);
  //Ex: subDigit = 2, first = 123456 -> startNumber = 12? -> 121212 < 123456 -> startNumber = 13
  //startNumber = first (subDigit) digits of (first)
  //startNumber = first / 10 ^ (firstDigits - subDigit)
  long long startNumber = first / longPow(10, firstDigits - subDigit);
  if (getRepeatingNumber(startNumber, firstDigits) < first) startNumber++;
  return startNumber;
}

long long getEndNumber(const int& subDigit, const int& curDigits, const long long& second) {
  int secondDigits = getDigits(second);
  if (curDigits < secondDigits) return longPow(10, subDigit) - 1;
  //Ex: subDigit = 2, second = 987654 -> startNumber = 98? -> 989898 > 987654 -> startNumber = 97
  //endNumber = first (subDigit) digits of (second)
  //startNumber = second / 10 ^ (secondDigits - subDigit)
  long long endNumber = second / longPow(10, secondDigits - subDigit);
  if (getRepeatingNumber(endNumber, secondDigits) > second) endNumber--;
  return endNumber;
}

long long invalidIdTotal(const long long& first, const long long& second) {
  int firstDigits = getDigits(first);
  int secondDigits = getDigits(second);
  long long total = 0;
  set<long long> invalidIds; // collect invalid ids in a set to not repeat 
  for (int curDigits = firstDigits; curDigits <= secondDigits; curDigits++) {
    vector<int> subDigits = getSubDigits(curDigits); // Ex: curDigits = 100 -> subDigits = [20, 50], subDigits 2, 5 and 10 will repeat
    for (int& subDigit : subDigits) {
      long long startNumber = getStartNumber(subDigit, curDigits, first);
      long long endNumber = getEndNumber(subDigit, curDigits, second);
      for (long long cur = startNumber; cur <= endNumber; cur++) {
        long long invalidId = getRepeatingNumber(cur, curDigits);
        if (invalidIds.find(invalidId) == invalidIds.end()) {
          total += invalidId;
          invalidIds.insert(invalidId);
        }
      }
    }
  }
  return total;
}

vector<pair<long long, long long>> splitInput(const string& lineText) {
  vector<pair<long long, long long>> vector;
  stringstream ss(lineText);
  string pairText;
  while (getline(ss, pairText, ',')) {
    int splitIndex = pairText.find('-');
    long long first = stoll(pairText.substr(0, splitIndex));
    long long second = stoll(pairText.substr(splitIndex + 1));
    pair<long long, long long> pair = {first, second};
    vector.push_back(pair);
  }
  return vector;
}

// much much much faster than first solution :D
int main() {
  string lineText;
  ifstream InputFile("input.txt");
  getline(InputFile, lineText);
  vector<pair<long long, long long>> ranges = splitInput(lineText);
  long long total = 0;
  for (pair<long long, long long>& range : ranges) {
    total += invalidIdTotal(range.first, range.second);
  }
  cout << "Total: " << total;
  return 0;
}