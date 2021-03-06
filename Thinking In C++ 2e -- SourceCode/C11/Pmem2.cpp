//: C11:Pmem2.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Pointers to members
#include <iostream>
using namespace std;

class Widget {
  void f(int) const {cout << "Widget::f()\n";}
  void g(int) const {cout << "Widget::g()\n";}
  void h(int) const {cout << "Widget::h()\n";}
  void i(int) const {cout << "Widget::i()\n";}
  static const int _count = 4;
  void (Widget::*fptr[_count])(int) const;
public:
  Widget() {
    fptr[0] = &Widget::f; // Full spec required
    fptr[1] = &Widget::g;
    fptr[2] = &Widget::h;
    fptr[3] = &Widget::i;
  }
  void select(int i, int j) {
    if(i < 0 || i >= _count) return;
    (this->*fptr[i])(j);
  }
  int count() { return _count; }
};

int main() {
  Widget w;
  for(int i = 0; i < w.count(); i++)
    w.select(i, 47);
} ///:~
