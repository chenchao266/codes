//: C15:Slice.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Object slicing
#include <iostream>
using namespace std;

class Base {
  int i;
public:
  Base(int ii = 0) : i(ii) {}
  virtual int sum() const { return i; }
};

class Derived : public Base {
  int j;
public:
  Derived(int ii = 0, int jj = 0)
    : Base(ii), j(jj) {}
  int sum() const { return Base::sum() + j; }
};

void call(Base b) {
  cout << "sum = " << b.sum() << endl;
}

int main() {
  Base b(10);
  Derived d(10, 47);
  call(b);
  call(d);
} ///:~
