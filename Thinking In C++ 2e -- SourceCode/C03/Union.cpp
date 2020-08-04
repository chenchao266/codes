//: C03:Union.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// The size and simple use of a union
#include <iostream>
using namespace std;

union packed { // Declaration similar to a class
  char i;
  short j;
  int k;
  long l;
  float f;
  double d;  
  // The union will be the size of a 
  // double, since that's the largest element
};  // Semicolon ends a union, like a struct

int main() {
  cout << "sizeof(packed) = " 
    << sizeof(packed) << endl;
  packed x;
  x.i = 'c';
  cout << x.i << endl;
  x.d = 3.14159;
  cout << x.d << endl;
} ///:~
