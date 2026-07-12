#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

class Corner {
  public:
    long long x = 0;
    long long y = 0;

    Corner(const vector<long long>& coordinates) {
      if (coordinates.size() == 2) {
        x = coordinates[0];
        y = coordinates[1];
      }
    }
};

// Ex: 1,2 - 3,4 => result is not 2 * 2 = 4 but 3 * 3 = 9 because coordinates are not dots
long long calculateArea(const Corner& corner1, const Corner& corner2) {
  return (1 + abs(corner1.x - corner2.x)) * (1 + abs(corner1.y - corner2.y));
}

long long calculateResult(const vector<Corner>& corners) {
  long long maxArea = 0;
  for (int i = 0; i < corners.size(); i++) {
    for (int j = i + 1; j < corners.size(); j++) {
      long long area = calculateArea(corners[i], corners[j]);
      //cout << corners[i].x << " " << corners[i].y << " - " << corners[j].x << " " << corners[j].y << " => " << area << "\n";
      if (area > maxArea) {
        maxArea = area;
      }
    }
  }
  return maxArea;
}

int main()
{
  string lineText;
  ifstream InputFile("input.txt");
  vector<Corner> corners;

  while (getline(InputFile, lineText))
  {
    vector<long long> coordinates;
    stringstream ss(lineText);
    string coordinate;
    while (getline(ss, coordinate, ',')) {
      coordinates.push_back(stoi(coordinate));
    }
    corners.push_back(Corner(coordinates));
  }

  /*
  for (const Corner& corner : corners) {
    cout << corner.x << " " << corner.y << "\n";
  }
  */

  cout << "Result: " << calculateResult(corners);

  return 0;
}