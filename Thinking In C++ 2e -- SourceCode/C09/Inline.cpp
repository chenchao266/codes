//: C09:Inline.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Inlines inside classes
#include <iostream>
using namespace std;

class Point {
  int i, j, k;
public:
  Point() { i = j = k = 0; }
  Point(int ii, int jj, int kk) {
    i = ii;
    j = jj;
    k = kk;
  }
  void print(const char* msg = "") const {
    if(*msg) cout << msg << endl;
    cout << "i = " << i << ", "
         << "j = " << j << ", "
         << "k = " << k << endl;
  }
};

int main() {
  Point p, q(1,2,3);
  p.print("value of p");
  q.print("value of q");
} ///:~
