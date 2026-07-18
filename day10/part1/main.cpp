#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <set>
using namespace std;

class Button { // displays which indices a button toggles
  public:
    set<int> toggles;
    
    Button(const set<int>& toggles) : toggles(toggles) {}
};

class Machine {
  public:
    vector<bool> indicators; // shows the state of its indicators, true for on, false for off
    set<int> requiredIndicators; // shows which indices must be on
    vector<Button> buttons;

    Machine(const int indicatorCount, const set<int>& requiredIndicators, const vector<Button>& buttons) // a machine has count amount of indicators which are off at start
      : indicators(indicatorCount, false), requiredIndicators(requiredIndicators), buttons(buttons) {}

    bool conditionMet(const vector<int>& buttonPressIndices) { // buttonPressIndices shows which button indices will be pressed
      for (const int index : buttonPressIndices) { // for each index press that button and toggle the indicators of the button
        pressButton(index);
      }

      for (int i = 0; i < indicators.size(); i++) {
        if (requiredIndicators.find(i) != requiredIndicators.end()) { // check if indicator index i is required
          if (indicators[i] == false) { // if its required but off, reset the state and return false
            return handleFail();
          } 
        } else {
          if (indicators[i] == true) { // if its not required but on, reset the state and return false
            return handleFail();
          } 
        }
      }
      return true; // condition is met
    }

    void pressButton(const int buttonIndex) { // toggle every indicator index that the button switches
      const Button& button = buttons[buttonIndex];
      for (const int index : button.toggles) {
        indicators[index] = !indicators[index]; 
      }
    }

    bool handleFail() {
      resetIndicators();
      return false;
    }

    void resetIndicators() { // reset all indices to off
      indicators = vector<bool>(indicators.size(), false);
    }
};

void pressButtons(Machine& machine, const int minPress, const int startIndex, vector<int> buttonPressIndices, bool& done) {
  if (done) return; // if a previous recursion set done, no need to calculate
  if (minPress == 0) { // if all presses are done, check if condition is met
    if (machine.conditionMet(buttonPressIndices)) {
      done = true;
    }
    return; // return regardless of done
  }
  for (int i = startIndex; i < machine.buttons.size(); i++) { // min press is reduced, index is increased to not press the same button twice, button index added to buttons to push list
    buttonPressIndices.push_back(i);
    pressButtons(machine, minPress - 1, i + 1, buttonPressIndices, done);
    buttonPressIndices.pop_back(); // pushed version should be sent to next recursion only
  }
}

// start with 1 press, check combination of buttons, if all combinations fail try again with more presses
int calculateMinPress(Machine& machine) {
  int minPress = 1; 
  bool done = false;
  while (!done) {
    pressButtons(machine, minPress, 0, {}, done);
    if (done) break;
    minPress++;
  }
  return minPress;
}

int main()
{
  string lineText;
  ifstream InputFile("input.txt");
  vector<Machine> machines;

  while (getline(InputFile, lineText))
  {
    string indicatorString;
    stringstream ss(lineText);
    set<int> requiredIndicators;
    vector<Button> buttons;
    
    ss >> indicatorString; // obtained indicator part of the line
    int indicatorCount = indicatorString.size() - 2; // -2 to ignore parantheses

    for (int i = 1; i < indicatorString.size() - 1; i++) {
      if (indicatorString[i] == '#') {
        requiredIndicators.insert(i - 1); // get required indicator indices (-1 because 0-indexed)
      }
    }

    string buttonPart;
    string buttonToggleNumber;

    while (ss >> buttonPart) { // keep reading words until end of line
      if (buttonPart[0] != '(') { // ignore joltage part
        break;
      } else {
        set<int> buttonToggles;
        buttonPart = buttonPart.substr(1, buttonPart.size() - 2); // remove parantheses
        stringstream buttonStream(buttonPart);
        while (getline(buttonStream, buttonToggleNumber, ',')) { // get a button's each toggling index
          buttonToggles.insert(stoi(buttonToggleNumber));
        }
        buttons.push_back(Button(buttonToggles));
      }
    }
    machines.push_back(Machine(indicatorCount, requiredIndicators, buttons));
  }

  /*
  for (const Machine& machine : machines) {
    cout << "Machine indicator count: " << machine.indicators.size() << ", required indicators: ";
    for (const int indicator : machine.requiredIndicators) {
      cout << indicator << " ";
    }
    cout << ", buttons: ";
    for (const Button& button : machine.buttons) {
      for (const int toggle : button.toggles) {
        cout << toggle << " ";
      }
      cout << ", ";
    }
    cout << "\n";
  }
  */
  
  int total = 0;
  for (Machine& machine : machines) {
    total += calculateMinPress(machine);
  }

  cout << "Total: " << total;

  return 0;
}