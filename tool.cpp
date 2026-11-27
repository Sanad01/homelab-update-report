#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

const int readUpgrade(ifstream& file);

/*
 *@desc: Reads a file to get the latest upgrad WITH AN END-DATE
 *@param: fName - the name of the file to be read
 *@return: returns the number of lines read
 */
int readFile(string fName) {

  ifstream file(fName);

  if (!file.is_open()) {
    throw runtime_error("Couldn't open file\n");
  }

  const int linesRead = readUpgrade(file);
  
  file.close();

  return linesRead;
}

const int readUpgrade(ifstream &file) {
  int linesRead = 0;
  string line = "";
  string incompleteUpgrade;
  string completeUpgrade;

  while (getline(file, line)) {
    ++linesRead;

    if (line.find("Start-Date:") == 0) {
      incompleteUpgrade.clear();
    }

    if (line.find("Upgrade:") == 0) {
      incompleteUpgrade = line;
    }

    if (line.find("End-Date:") == 0) {
      completeUpgrade = incompleteUpgrade;
    }
  }

  const string prefix = "Upgrade: ";
  const size_t nameStart = prefix.size();
  const size_t nameEnd = completeUpgrade.find(" (", nameStart);
  const size_t comma = completeUpgrade.find(", ", nameEnd);
  const size_t closeParen = completeUpgrade.find(')', nameEnd);

  if (nameEnd != string::npos && closeParen != string::npos &&
      comma < closeParen) {
    const int oldVersionStart = nameEnd + 2; // Skip " ("
    const int newVersionStart = comma + 2;   // Skip ", "

    const string packageName =
        completeUpgrade.substr(nameStart, nameEnd - nameStart);
    const string oldVersion =
        completeUpgrade.substr(oldVersionStart, comma - oldVersionStart);
    const string newVersion =
        completeUpgrade.substr(newVersionStart, closeParen - newVersionStart);

    cout << packageName << "  " << oldVersion << " → " << newVersion << '\n';
  }
  return linesRead;
}

int main() {
  int lines = readFile("test.txt");
  cout <<  "lines read: " << lines << '\n';
  return 0;
}  
