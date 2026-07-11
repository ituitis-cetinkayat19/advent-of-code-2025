#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

int getDigits(long long number) {
  int digits = 1;
  while (number % 10 != number) {
    number /= 10;
    digits++;
  }
  return digits;
}

bool checkIfSameParts(const string& stringId, int& partSize) {
  int index = 0;
  vector<string> parts;
  while(index < stringId.size()) {
    parts.push_back(stringId.substr(index, partSize));
    index += partSize;
  }
  if (parts.size() <= 1) return false;
  string checkPart = parts[0];
  for (int i = 1; i < parts.size(); i++) {
    if (checkPart != parts[i]) return false;
  }
  return true;
}

bool isInvalidId(const long long& id) {
  int digits = getDigits(id); // Ex: id = 123123, digits = 6
  string stringId = to_string(id);
  for (int i = 1; i < digits; i++) {
    if (digits % i != 0) continue;
    if (checkIfSameParts(stringId, i)) return true;
  }
  return false;
}

long long invalidIdTotal(const string& first, const string& second) {
  long long total = 0;
  long long firstLong = stoll(first);
  long long secondLong = stoll(second);
  for (long long i = firstLong; i <= secondLong; i++) {
    if (isInvalidId(i)) total += i;
  }
  return total;
}

vector<pair<string, string>> splitInput(const string& lineText) {
  vector<pair<string, string>> vector;
  stringstream ss(lineText);
  string pairText;
  while (getline(ss, pairText, ',')) {
    int splitIndex = pairText.find('-');
    string first = pairText.substr(0, splitIndex);
    string second = pairText.substr(splitIndex + 1);
    pair<string, string> pair = {first, second};
    vector.push_back(pair);
  }
  return vector;
}

// works but slow
int main() {
  string lineText;
  ifstream InputFile("input.txt");
  getline(InputFile, lineText);
  vector<pair<string, string>> ranges = splitInput(lineText);
  long long total = 0;
  for (pair<string, string>& range : ranges) {
    total += invalidIdTotal(range.first, range.second);
  }
  cout << "Total: " << total;
  return 0;
}