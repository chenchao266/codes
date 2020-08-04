//: C11:Pmem.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Pointers to members

class Widget {
public:
  void f(int);
  void g(int);
  void h(int);
  void i(int);
};

void Widget::h(int) {}

int main() {
  Widget w;
  Widget* wp = &w;
  void (Widget::*pmem)(int) = &Widget::h;
  (w.*pmem)(1);
  (wp->*pmem)(2);
} ///:~
