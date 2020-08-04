//: C15:Pvdest.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Pure virtual destructors
// require a function body.
#include <iostream>
using namespace std;

class Base {
public:
  virtual ~Base() {
    cout << "~Base()" << endl;
  }
};

class Derived : public Base {
public:
  ~Derived() {
    cout << "~Derived()" << endl;
  }
};

int main() {
  Base* bp = new Derived; // Upcast
  delete bp; // Virtual destructor call
} ///:~
