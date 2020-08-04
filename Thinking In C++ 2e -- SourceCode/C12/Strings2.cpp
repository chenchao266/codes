//: C12:Strings2.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// With auto type conversion
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
  operator const char*() const { return s; }
};

int main() {
  Stringc s1("hello"), s2("there");
  strcmp(s1, s2); // Standard C function
  strspn(s1, s2); // Any string function!
} ///:~
