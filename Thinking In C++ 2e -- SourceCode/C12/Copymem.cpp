//: C12:Copymem.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Duplicate during assignment
#include "../require.h"
#include <cstdlib>
#include <cstring>
using namespace std;

class WithPointer {
  char* p;
  static const int blocksz = 100;
public:
  WithPointer() {
    p = (char*)malloc(blocksz);
    require(p != 0);
    memset(p, 1, blocksz);
  }
  WithPointer(const WithPointer& wp) {
    p = (char*)malloc(blocksz);
    require(p != 0);
    memcpy(p, wp.p, blocksz);
  }
  WithPointer&
  operator=(const WithPointer& wp) {
    // Check for self-assignment:
    if(&wp != this)
      memcpy(p, wp.p, blocksz);
    return *this;
  }
  ~WithPointer() {
    free(p);
  }
}; 

int main() {
  WithPointer p;
  WithPointer p2 = p; // Copy construction
  p = p2; // Assignment
} ///:~
