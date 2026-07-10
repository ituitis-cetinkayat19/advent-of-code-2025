#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

long long pushRange(vector<pair<long long, long long>>& ranges, pair<long long, long long>& newRange) {
  // newRange.first < old.first && newRange.second < old.first --> no collision
  // newRange.first > old.second && newRange.second > old.second --> no collision
  // newRange.first < old.first && newRange.second >= old.first && newRange.second < old.second --> left collision (remove colliding part)
  // newRange.second > old.second && newRange.first <= old.second && newRange.first > old.first --> right collision (remove colliding part)
  // newRange.first >= old.first && newRange.second <= old.second --> gets absorbed (do not add to list)
  // newRange.first <= old.first && newRange.second >= old.second --> absorbs (either remove old or create two new sub ranges as the parts that stick out)
  vector<pair<long long, long long>> extras;
  for (const pair<long long, long long>& old : ranges) {
    //cout << "New Range: " << newRange.first << "-" << newRange.second << " Old Range: " << old.first << "-" << old.second << "\n";
    if (newRange.first < old.first && newRange.second < old.first) { // no collision
      //cout << "No Collision (Left)" << "\n";
      continue; 
    } 
    else if (newRange.first > old.second && newRange.second > old.second) { // no collision
      //cout << "No Collision (Right)" << "\n";
      continue;
    }
    else if (newRange.first < old.first && newRange.second >= old.first && newRange.second < old.second) { // left collision
      //cout << "Left Collision" << "\n";
      if (old.first - 1 < newRange.first) return 0;
      newRange = pair<long long, long long>({newRange.first, old.first - 1});
    }
    else if (newRange.second > old.second && newRange.first <= old.second && newRange.first > old.first) { // right collision
      //cout << "Right Collision" << "\n";
      if (old.second + 1 > newRange.second) return 0;
      newRange = pair<long long, long long>({old.second + 1, newRange.second});
    }
    else if (newRange.first >= old.first && newRange.second <= old.second) { // gets absorbed
      //cout << "Gets Absorbed" << "\n";
      return 0;
    }
    else if (newRange.first <= old.first && newRange.second >= old.second) { // absorbs
      //cout << "Absorbs" << "\n";
      if (old.first - 1 >= newRange.first) extras.push_back({newRange.first, old.first - 1}); // left pair
      if (newRange.second >= old.second + 1) extras.push_back({old.second + 1, newRange.second}); // right pair
      break;
    }
  };
  if (extras.size() == 0) {
    ranges.push_back(newRange);
    return newRange.second - newRange.first + 1;
  }
  long long total = 0;
  for (pair<long long, long long>& extra : extras) {
    total += pushRange(ranges, extra);
  }
  return total;
}

int main() {
  string lineText;
  ifstream InputFile("input.txt");
  vector<pair<long long, long long>> freshRanges;
  long long total = 0;
  while (getline(InputFile, lineText) && !lineText.empty()) {
    int splitIndex = lineText.find('-');
    long long first = stoll(lineText.substr(0, splitIndex));
    long long second = stoll(lineText.substr(splitIndex + 1));
    pair<long long, long long> range = {first, second};
    total += pushRange(freshRanges, range);
  }
  /*
  for (const pair<long long, long long>& range : freshRanges) {
    cout << range.first << "-" << range.second << "\n";
  }
  */
  cout << "Total: " << total;
  return 0;
}