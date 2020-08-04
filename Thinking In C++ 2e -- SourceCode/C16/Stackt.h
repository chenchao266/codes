//: C16:Stackt.h
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Simple stack template
#ifndef STACKT_H
#define STACKT_H
template<class T> class StacktIter; // Declare

template<class T>
class Stackt {
  static const int ssize = 100;
  T stack[ssize];
  int top;
public:
  Stackt() : top(0) { stack[top] = 0; }
  void push(const T& i) {
    if(top < ssize) stack[top++] = i;
  }
  T pop() {
    return stack[top > 0 ? --top : top];
  }
  friend class StacktIter<T>;
};

template<class T>
class StacktIter {
  Stackt<T>& s;
  int index;
public:
  StacktIter(Stackt<T>& is)
    : s(is), index(0) {}
  T& operator++() { // Prefix form
    if (index < s.top - 1) index++;
    return s.stack[index];
  }
  T& operator++(int) { // Postfix form
    int returnIndex = index;
    if (index < s.top - 1) index++;
    return s.stack[returnIndex];
  }
};
#endif // STACKT_H ///:~
