//: C13:Stack4Test.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
//{L} Stack4
// Test new Stack
#include "Stack4.h"
#include "../require.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
  // Could also use command-line argument:
  ifstream file("Stack4Test.cpp");
  assure(file, " Stack4Test.cpp");
  Stack textlines;
  string line;
  while(getline(file, line))
    textlines.push(new string(line));
  // Pop lines from the Stack and print them:
  string* s;
  while((s = (string*)textlines.pop()) != 0)
    cout << *s << endl;
} ///:~
