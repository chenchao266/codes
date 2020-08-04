//: C15:Early.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Early binding & virtuals
#include <iostream>
using namespace std;

class Base {
public:
  virtual int f() const { return 1; }
};

class Derived : public Base {
public:
  int f() const { return 2; }
};

int main() {
  Derived d;
  Base* b1 = &d;
  Base& b2 = d;
  Base b3;
  // Late binding for both:
  cout << "b1->f() = " << b1->f() << endl;
  cout << "b2.f() = " << b2.f() << endl;
  // Early binding (probably):
  cout << "b3.f() = " << b3.f() << endl;
} ///:~
