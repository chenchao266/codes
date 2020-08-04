//: C15:Addv.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Adding virtuals in derivation
#include <iostream>
using namespace std;

class Base {
  int i;
public:
  Base(int ii) : i(ii) {}
  virtual int value() const { return i; }
};

class Derived : public Base {
public:
  Derived(int ii) : Base(ii) {}
  int value() const {
    return Base::value() * 2;
  }
  // New virtual function in the Derived class:
  virtual int shift(int x) const {
    return Base::value() << x;
  }
};

int main() {
  Base* B[] = { new Base(7), new Derived(7) };
  cout << "B[0]->value() = "
       << B[0]->value() << endl;
  cout << "B[1]->value() = "
       << B[1]->value() << endl;
//! cout << "B[1]->shift(3) = "
//!      << B[1]->shift(3) << endl; // Illegal
} ///:~
