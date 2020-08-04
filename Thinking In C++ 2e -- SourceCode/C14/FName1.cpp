//: C14:FName1.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// An fstream with a file name
#include "../require.h"
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;

class FName1 {
  ifstream file;
  static const int bsize = 100;
  char buf[bsize];
  ostrstream fname;
  int nameset;
public:
  FName1() : fname(buf, bsize), nameset(0) {}
  FName1(const char* filename)
    : file(filename), fname(buf, bsize) {
      assure(file, filename);
      fname << filename << ends;
      nameset = 1;
  }
  const char* name() const { return buf; }
  void name(const char* newname) {
    if(nameset) return; // Don't overwrite
    fname << newname << ends;
    nameset = 1;
  }
  operator ifstream&() { return file; }
};

int main() {
  FName1 file("FName1.cpp");
  cout << file.name() << endl;
  // Error: rdbuf() not a member:
//!  cout << file.rdbuf() << endl;
} ///:~
