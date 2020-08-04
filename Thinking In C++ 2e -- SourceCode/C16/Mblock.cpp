//: C16:Mblock.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Built-in types in templates
#include "../require.h"
#include <iostream>
using namespace std;

template<class T, int size = 100>
class Mblock {
  T array[size];
public:
  T& operator[](int index) {
    require(index >= 0 && index < size);
    return array[index];
  }
};

class Number {
  float f;
public:
  Number(float ff = 0.0f) : f(ff) {}
  Number& operator=(const Number& n) {
    f = n.f;
    return *this;
  }
  operator float() const { return f; }
  friend ostream&
    operator<<(ostream& os, const Number& x) {
      return os << x.f;
  }
};

template<class T, int sz = 20>
class Holder {
  Mblock<T, sz>* np;
public:
  Holder() : np(0) {}
  T& operator[](int i) {
    require(i >= 0 && i < sz);
    if(!np) np = new Mblock<T, sz>;
    return np->operator[](i);
  }
};

int main() {
  Holder<Number, 20> h;
  for(int i = 0; i < 20; i++)
    h[i] = i;
  for(int j = 0; j < 20; j++)
    cout << h[j] << endl;
} ///:~
