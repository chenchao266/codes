//: C10:Depend.h
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Static initialization technique
#ifndef DEPEND_H
#define DEPEND_H
#include <iostream>
extern int x; // Declarations, not definitions
extern int y;

class Initializer {
  static int init_count;
public:
  Initializer() {
    std::cout << "Initializer()" << std::endl;
    // Initialize first time only
    if(init_count++ == 0) {
      std::cout << "performing initialization"
           << std::endl;
      x = 100;
      y = 200;
    }
  }
  ~Initializer() {
    std::cout << "~Initializer()" << std::endl;
    // Clean up last time only
    if(--init_count == 0) {
      std::cout << "performing cleanup" 
        << std::endl;
      // Any necessary cleanup here
    }
  }
};

// The following creates one object in each
// file where DEPEND.H is included, but that
// object is only visible within that file:
static Initializer init;
#endif // DEPEND_H ///:~
