//: C12:Strings1.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// No auto type conversion
#include "../require.h"
#include <cstring>
#include <cstdlib>
using namespace std;

class Stringc {
  char* s;
public:
  Stringc(const char* S = "") {
    s = (char*)malloc(strlen(S) + 1);
    require(s != 0);
    strcpy(s, S);
  }
  ~Stringc() { free(s); }
  int strcmp(const Stringc& S) const {
    return ::strcmp(s, S.s);
  }
  // ... etc., for every function in string.h
};

int main() {
  Stringc s1("hello"), s2("there");
  s1.strcmp(s2);
} ///:~
