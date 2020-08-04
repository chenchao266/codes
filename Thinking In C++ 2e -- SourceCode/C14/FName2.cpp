//: C14:FName2.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Subtyping solves the problem
#include "../require.h"
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;

class FName2 : public ifstream {
  static const int bsize = 100;
  char buf[bsize];
  ostrstream fname;
  int nameset;
public:
  FName2() : fname(buf, bsize), nameset(0) {}
  FName2(const char* filename)
    : ifstream(filename), fname(buf, bsize) {
      assure(*this, filename);
      fname << filename << ends;
      nameset = 1;
  }
  const char* name() const { return buf; }
  void name(const char* newname) {
    if(nameset) return; // Don't overwrite
    fname << newname << ends;
    nameset = 1;
  }
};

int main() {
  FName2 file("FName2.cpp");
  assure(file, "FName2.cpp");
  cout << "name: " << file.name() << endl;
  const int bsize = 100;
  char buf[bsize];
  file.getline(buf, bsize); // This works too!
  file.seekg(-200, ios::end);
  cout << file.rdbuf() << endl;
} ///:~
