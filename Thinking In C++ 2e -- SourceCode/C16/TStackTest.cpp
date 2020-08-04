//: C16:TStackTest.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Use template list & iterator
#include "TStack.h"
#include "../require.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
  ifstream file("TStackTest.cpp");
  assure(file, "TStackTest.cpp");
  TStack<string> textlines;
  // Read file and store lines in the list:
  string line;
  while(getline(file, line))
    textlines.push(new string(line));
  int i = 0;
  // Use iterator to print lines from the list:
  TStackIterator<string> it(textlines);
  TStackIterator<string>* it2 = 0;
  while(it) {
    cout << *it.current() << endl;
    it++;
    if(++i == 10) // Remember 10th line
      it2 = new TStackIterator<string>(it);
  }
  cout << *(it2->current()) << endl;
  delete it2;
} ///:~
