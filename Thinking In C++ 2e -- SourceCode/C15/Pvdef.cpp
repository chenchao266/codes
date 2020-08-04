//: C15:Pvdef.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Pure virtual base definition
#include <iostream>
using namespace std;

class Base {
public:
  virtual void v() const = 0;
  virtual void f() const = 0;
  // Inline pure virtual definitions illegal:
  //!  virtual void g() const = 0 {}
};

// OK, not defined inline
void Base::f() const {
  cout << "Base::f()\n";
}

void Base::v() const { cout << "Base::v()\n";}

class D : public Base {
public:
  // Use the common Base code:
  void v() const { Base::v(); }
  void f() const { Base::f(); }
};

int main() {
  D d;
  d.v();
  d.f();
} ///:~
