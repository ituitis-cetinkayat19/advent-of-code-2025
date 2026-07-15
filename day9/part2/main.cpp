#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

enum Direction {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

class RectangleLines { // Ex: if rectangles corners are (1, 2) (3, 4) it will store x = {1, 3} y = {2, 4} Rectangle's edges are the lines x=1, x=3, y=2, y=4 on the grid
  public:
    pair<long long, long long> x;
    pair<long long, long long> y;

    RectangleLines(const long long startX, const long long startY, const long long endX, const long long endY) {
      x = startX < endX ? make_pair(startX, endX) : make_pair(endX , startX); //sorted x lines
      y = startY < endY ? make_pair(startY, endY) : make_pair(endY, startY); //sorted y lines
    }
};

class Corner { // represents each corner on the shape
  public:
    long long x = 0;
    long long y = 0;

    Corner(const vector<long long>& coordinates) {
      if (coordinates.size() == 2) {
        x = coordinates[0];
        y = coordinates[1];
      }
    }

    Corner() {}
};

class Line { // represents each line in the shape
  public:
    Corner startCorner;
    Corner endCorner;
    Direction direction; // represents lines direction from startCorner to endCorner
    Direction shapeAt; // Represents which side of the line has the shape area

    Line(Corner startCorner, Corner endCorner) : startCorner(startCorner), endCorner(endCorner) {}
    
    Line() {}

    void calculateDirection() {
      if (startCorner.x == endCorner.x) {
        direction = startCorner.y > endCorner.y ? UP : DOWN;
      } else {
        direction = startCorner.x > endCorner.x ? LEFT : RIGHT;
      }
    }
};

// Ex: 1,2 - 3,4 => result is not 2 * 2 = 4 but 3 * 3 = 9 because coordinates are not dots
long long calculateArea(const Corner& corner1, const Corner& corner2) {
  return (1 + abs(corner1.x - corner2.x)) * (1 + abs(corner1.y - corner2.y));
}

// find the most bottom line in the shape, its shapeAt is guaranteed to be UP, from there calculate every lines shapeAt
Line findBottomLine(vector<Line>& lines) {
  long long totalY = 0;
  Line* result = &lines[0]; // default result
  for (Line& line : lines) {
    long long currentTotalY = line.startCorner.y + line.endCorner.y;
    if (currentTotalY > totalY) {
      result = &line;
      totalY = currentTotalY;
    }
  }
  return *result;
}

// given line, find its index in lines vector
int findLineIndex(const vector<Line>& lines, const Line& bottomLine) {
  for (int i = 0 ; i < lines.size(); i++) {
    if (lines[i].startCorner.x == bottomLine.startCorner.x && lines[i].startCorner.y == bottomLine.startCorner.y 
      && lines[i].endCorner.x == bottomLine.endCorner.x && lines[i].endCorner.y == bottomLine.endCorner.y) {
        return i;
      }
  }
  return -1;
}

// increment index with respect to size, set 0 if it reaches size
int incrementIndex(int lineIndex, const int size) {
  return ++lineIndex % size;
}

// used for calculating shapeAt
Direction getOpposite(const Direction direction) {
  switch (direction) {
    case UP:
      return DOWN;
    case RIGHT:
      return LEFT;
    case DOWN:
      return UP;
    case LEFT:
      return RIGHT;
  }
}

// (Which side of the prev line has area) - (Direction of prev line) - (Direction of current line)
// prevLineShapeAt - prevLineDirection - currentLineDirection
// (if currentLineDirection is equal to prevLineShapeAt, return opposite of prevLineDirection, otherwise return prevLineDirection)
// (prevLineShapeAt == currentLineDirection ? getOpposite(prevLineDirection) : prevLineDirection)
// UP - RIGHT - UP: LEFT
// UP - RIGHT - DOWN: RIGHT
// UP - LEFT - UP: RIGHT
// UP - LEFT - DOWN: LEFT

// RIGHT - UP - RIGHT: DOWN
// RIGHT - UP - LEFT: UP
// RIGHT - DOWN - RIGHT: UP
// RIGHT - DOWN - LEFT: DOWN

// DOWN - RIGHT - UP: RIGHT
// DOWN - RIGHT - DOWN: LEFT
// DOWN - LEFT - UP: LEFT
// DOWN - LEFT - DOWN: RIGHT

// LEFT - UP - RIGHT: UP
// LEFT - UP - LEFT: DOWN
// LEFT - DOWN - RIGHT: DOWN
// LEFT - DOWN - LEFT: UP
Direction calculateShapeAt(const Direction prevLineShapeAt, const Direction prevLineDirection, const Direction currentLineDirection) {
  return prevLineShapeAt == currentLineDirection ? getOpposite(prevLineDirection) : prevLineDirection;
}

// set shapeAt of every line using previous line's shapeAt and direction and current line direction
void calculateShape(vector<Line>& lines, const Line& bottomLine) {
  int lineIndex = findLineIndex(lines, bottomLine);
  int initialLineIndex = lineIndex;
  Direction prevShapeAt = UP;
  Direction prevDirection = bottomLine.direction;
  lines[lineIndex].shapeAt = prevShapeAt;
  lineIndex = incrementIndex(lineIndex, lines.size());
  while (lineIndex != initialLineIndex) { // from bottomLine's index, iterate every line and determine the shape from prev shape, prev direction and current direction
    Line& currentLine = lines[lineIndex];
    const Direction newShapeAt = calculateShapeAt(prevShapeAt, prevDirection, currentLine.direction);
    currentLine.shapeAt = newShapeAt;
    prevShapeAt = newShapeAt;
    prevDirection = currentLine.direction;
    lineIndex = incrementIndex(lineIndex, lines.size());
  }
}

// check if line has this corner as either start or end, used for ignoring shapeAt restriction
bool lineHasCorner(const Line& line, const Corner& corner) {
  return (line.startCorner.x == corner.x && line.startCorner.y == corner.y) || 
    (line.endCorner.x == corner.x && line.endCorner.y == corner.y);  
}

// if line passes on top of a rectangle line check if its shapeAt looks towards endCorner
bool shapeAtLooksAtEnd(const Line& line, const Corner& endCorner) {
  if (lineHasCorner(line, endCorner)) return true;
  const Direction shapeAt = line.shapeAt;
  switch (shapeAt) {
    case UP:
      return endCorner.y <= line.startCorner.y;
    case RIGHT:
      return endCorner.x >= line.startCorner.x;
    case DOWN:
      return endCorner.y >= line.startCorner.y;
    case LEFT:
      return endCorner.x <= line.startCorner.x;
  }
}

// conditions that prevent a rectangle, i used 3 rules for rectangle check
bool breaksRectangle(const RectangleLines& rctLines, const Line& line, const Corner& endCorner) {
  //cout << "Checking line : (" << line.startCorner.x << ", " << line.startCorner.y << ") - (" << line.endCorner.x << ", " << line.endCorner.y << ") with end corner: (" << endCorner.x << ", " << endCorner.y << ")" << "\n";
  const Direction dir = line.direction;
  long long startX = line.startCorner.x;
  long long startY = line.startCorner.y;
  long long endX = line.endCorner.x;
  long long endY = line.endCorner.y;

  // Rule 1: end corner cannot be inside rectangle
  if (rctLines.x.first < endX && endX < rctLines.x.second && rctLines.y.first < endY && endY < rctLines.y.second)
    return true;

  // Rule 2: crossing the rectangle is only allowed from the edges of it, not within (idk why start coordinates have equal to check but end ones dont :D)
  switch (dir) {
    case UP:
      if (rctLines.x.first < endX && endX < rctLines.x.second && startY >= rctLines.y.second && endY < rctLines.y.second)
        return true;
      break;
    case RIGHT:
      if (rctLines.y.first < endY && endY < rctLines.y.second && startX <= rctLines.x.first && endX > rctLines.x.first)
        return true;
      break;
    case DOWN:
      if (rctLines.x.first < endX && endX < rctLines.x.second && startY <= rctLines.y.first && endY > rctLines.y.first)
        return true;
      break;
    case LEFT:
      if (rctLines.y.first < endY && endY < rctLines.y.second && startX >= rctLines.x.second && endX < rctLines.x.second)
        return true;
      break;
  }

  // Rule 3: if line is on top of rectangleLine check shapeAt (idk why only end coordinates have equal to check :D)
  switch (dir) {
    case UP:
    case DOWN:
      if ((endX == rctLines.x.first || endX == rctLines.x.second) && 
        ((rctLines.y.first < startY && startY < rctLines.y.second) || (rctLines.y.first <= endY && endY <= rctLines.y.second)) && 
        !shapeAtLooksAtEnd(line, endCorner))
        return true;
      break;
    case RIGHT:
    case LEFT:
      if ((endY == rctLines.y.first || endY == rctLines.y.second) &&
        ((rctLines.x.first < startX && startX < rctLines.x.second) || (rctLines.x.first <= endX && endX <= rctLines.x.second)) &&
        !shapeAtLooksAtEnd(line, endCorner))
        return true;
      break;
  }
  return false;
}

// iterate every line from startIndex, set endIndex as startIndex when endIndex is reached
bool canMakeRectangle(const vector<Line>& lines, const int startLineIndex, const int endLineIndex) {
  if (startLineIndex == endLineIndex) return true; // corners are at the same line so guaranteed rectangle
  const Line& startLine = lines[startLineIndex];
  const Line& endLine = lines[endLineIndex];
  const Corner& startCorner = startLine.startCorner;
  Corner endCorner = endLine.endCorner; // will be updated in loop

  // if line after endLine has startCorner return true, to match first corner and last corner in the loop
  int nextIndexAfterEndLine = incrementIndex(endLineIndex, lines.size());
  if (lineHasCorner(lines[nextIndexAfterEndLine], startCorner)) return true;

  const RectangleLines rectangleLines = RectangleLines(startCorner.x, startCorner.y, endCorner.x, endCorner.y);

  int lineIndex = startLineIndex;
  do {
    const Line& currentLine = lines[lineIndex];
    if (breaksRectangle(rectangleLines, currentLine, endCorner)) {
      //cout << "Line: (" << currentLine.startCorner.x << ", " << currentLine.startCorner.y << ") - (" << currentLine.endCorner.x << ", " << currentLine.endCorner.y << ") breaks rectangle \n";
      return false;
    } 
    lineIndex = incrementIndex(lineIndex, lines.size());
    if (lineIndex == incrementIndex(endLineIndex, lines.size())) {
      endCorner = startCorner; // if you reach end corner set start corner as end corner to complete the loop
    }
  } while (lineIndex != startLineIndex);
  return true;
}

// at every iteration line i's start and line j's end corners are used for calculation thats why int j = i is set
long long calculateResult(const vector<Line>& lines) {
  long long maxArea = 0;
  for (int i = 0; i < lines.size(); i++) {
    const Corner& startCorner = lines[i].startCorner;
    for (int j = i; j < lines.size() - 1; j++) {
      const Corner& endCorner = lines[j].endCorner;
      //cout << "Checking corners: (" << startCorner.x << ", " << startCorner.y << ") - (" << endCorner.x << ", " << endCorner.y << ")\n";
      if (canMakeRectangle(lines, i, j)) {
        //cout << "Rectangle from: (" << startCorner.x << ", " << startCorner.y << ") - (" << endCorner.x << ", " << endCorner.y << ")\n";
        long long area = calculateArea(startCorner, endCorner);
        if (area > maxArea) {
          maxArea = area;
        }
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
  vector<Line> lines;
  vector<string> lineTypes = {"UP", "RIGHT", "DOWN", "LEFT"}; //used for cout

  while (getline(InputFile, lineText)) // corners are read
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

  if (corners.size() > 1) { // lines are calculated from corners
    for (int i = 0; i < corners.size(); i++) {
      Corner& currentCorner = corners[i];
      Corner& nextCorner = i < corners.size() - 1 ? corners[i+1] : corners[0];
      Line newLine = Line(currentCorner, nextCorner);
      newLine.calculateDirection();
      lines.push_back(newLine);
    }
  }
  const Line& bottomLine = findBottomLine(lines); // sort lines by their corners' y's to get bottom line for setting shapeAt
  calculateShape(lines, bottomLine);

  /*
  for (const Line& line : lines) {
    cout << "(" << line.startCorner.x << ", " << line.startCorner.y << ") - (" << line.endCorner.x << ", " << line.endCorner.y << ") - Direction: " << lineTypes[line.direction] << ", Shape at: " << lineTypes[line.shapeAt] << "\n";
  }
  */
  
  cout << "Result: " << calculateResult(lines);

  return 0;
}