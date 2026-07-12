#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;

class Box {
  public:
    long long x = 0;
    long long y = 0;
    long long z = 0;
    bool matched = false;

    Box(const vector<long long>& coordinates) {
      if (coordinates.size() == 3) {
        x = coordinates[0];
        y = coordinates[1];
        z = coordinates[2];
      }
    }
};

class Distance {
  public:
    int start;
    int end;
    double distance;

    Distance(int start, int end, double distance) : start(start), end(end), distance(distance) {}

    bool operator<(const Distance& other) {
      return distance < other.distance;
    }
};

class Circuit {
  public:
    vector<int> boxIndices;

    Circuit(int startBoxIndex, int endBoxIndex) {
      boxIndices.push_back(startBoxIndex);
      boxIndices.push_back(endBoxIndex);
    }

    void addBox(const int& boxIndex) {
      boxIndices.push_back(boxIndex);
    }

    void pushBoxes(const vector<int>& newBoxIndices) {
      boxIndices.insert(boxIndices.end(), newBoxIndices.begin(), newBoxIndices.end());
    }

    bool hasBox(const int& boxIndex) {
      auto it = find(boxIndices.begin(), boxIndices.end(), boxIndex);
      return it != boxIndices.end();
    }

    bool operator==(const Circuit& other) {
      return boxIndices == other.boxIndices;
    }

    int getCircuitSize() {
      return boxIndices.size();
    }
};

long long longPow(long long base, long long exponent) {
  long long result = 1;
  while(exponent > 0) {
    result *= base;
    exponent--;
  }
  return result;
}

double calculateDistance(const Box& box1, const Box& box2) {
  return sqrt(longPow(abs(box1.x - box2.x), 2) + longPow(abs(box1.y - box2.y), 2) + longPow(abs(box1.z - box2.z), 2));
}

vector<Distance> getDistances(const vector<Box>& boxes) {
  vector<Distance> result;
  for (int i = 0; i < boxes.size(); i++) {
    for (int j = i + 1; j < boxes.size(); j++) {
      result.push_back(Distance(i, j, calculateDistance(boxes[i], boxes[j])));
    }
  }
  return result;
}

Circuit& findCircuit(vector<Circuit>& circuits, const int& boxIndex) {
  for (Circuit& circuit : circuits) {
    if (circuit.hasBox(boxIndex)) return circuit;
  }
  throw runtime_error("Circuit not found!");
}

bool inSameCircuit(vector<Circuit>& circuits, const int& startBoxIndex, const int& endBoxIndex) {
  for (Circuit& circuit : circuits) {
    if (circuit.hasBox(startBoxIndex) && circuit.hasBox(endBoxIndex)) return true;
  }
  return false;
}

// unmatched <-> matched (joins the circuit)
// unmatched <-> unmatched (creates a new circuit)
// matched <-> matched (continue if within a circuit, combine circuits if between circuits)
void matchBoxes(vector<Box>& boxes, vector<Distance>& distances, vector<Circuit>& circuits) {
  int connections = 0;
  for (const Distance& distance : distances) {
    if (connections >= 1000) return; // update required connections based on input
    int startIndex = distance.start;
    int endIndex = distance.end;
    Box& startBox = boxes[startIndex];
    Box& endBox = boxes[endIndex];
    //cout << startIndex << " (" << (startBox.matched ? "true" : "false") << ") " << endIndex << " (" << (endBox.matched ? "true" : "false") << ")" << "\n";
    if (startBox.matched && endBox.matched) { // both already belong to a circuit, match if they belong in different circuits
      if (inSameCircuit(circuits, startIndex, endIndex)) {
        connections++; // for some reason, we need to count nothing happens part as connection
        continue;
      }
      Circuit& startCircuit = findCircuit(circuits, startIndex);
      Circuit& endCircuit = findCircuit(circuits, endIndex);
      startCircuit.pushBoxes(endCircuit.boxIndices);
      auto it = find(circuits.begin(), circuits.end(), endCircuit);
      if (it != circuits.end()) {
        circuits.erase(it);
      }
    }
    else if (!startBox.matched && !endBox.matched) {
      Circuit newCircuit = Circuit(startIndex, endIndex); // add box indices as a new circuit
      circuits.push_back(newCircuit);
      startBox.matched = true;
      endBox.matched = true;
    }
    else if (startBox.matched && !endBox.matched) {
      Circuit& circuit = findCircuit(circuits, startIndex);
      circuit.addBox(endIndex);
      endBox.matched = true;
    }
    else if (!startBox.matched && endBox.matched) {
      Circuit& circuit = findCircuit(circuits, endIndex);
      circuit.addBox(startIndex);
      startBox.matched = true;
    }
    connections++;
    /*
    for (Circuit circuit : circuits) {
      cout << "[";
      for (int index : circuit.boxIndices) {
        cout << index << " ";
      }
      cout << "]";
    }
    cout << "\n";
    */
  }
}

int calculateResult(vector<Circuit>& circuits) {
  vector<int> circuitSizes;
  for (Circuit& circuit : circuits) {
    circuitSizes.push_back(circuit.getCircuitSize());
  }
  sort(circuitSizes.begin(), circuitSizes.end(), greater<>());
  if (circuitSizes.size() == 0) return 1;
  if (circuitSizes.size() == 1) return circuitSizes[0];
  if (circuitSizes.size() == 2) return circuitSizes[0] * circuitSizes[1];
  if (circuitSizes.size() >= 3) {
    return circuitSizes[0] * circuitSizes[1] * circuitSizes[2];
  } 
}

int main() {
  string lineText;
  ifstream InputFile("input.txt");
  vector<Box> boxes;

  while (getline(InputFile, lineText)) {
    vector<long long> coordinates;
    stringstream ss(lineText);
    string coordinate;
    while (getline(ss, coordinate, ',')) {
      coordinates.push_back(stoll(coordinate));
    }
    boxes.push_back(Box(coordinates));
  }

  vector<Distance> distances = getDistances(boxes);
  sort(distances.begin(), distances.end());
  /*
  for (Distance distance : distances) {
    cout << "start: " << distance.start << ", end: " << distance.end << ", distance: " << distance.distance << "\n";
  }
  */
  
  vector<Circuit> circuits;
  matchBoxes(boxes, distances, circuits);
  cout << "Result: " << calculateResult(circuits);
  
  return 0;
}