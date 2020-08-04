//: C13:Newdel.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Simple demo of new & delete
#include <iostream>
using namespace std;

class Tree {
  int height;
public:
  Tree(int height) {
    height = height;
  }
  ~Tree() { cout << "*"; }
  friend ostream&
  operator<<(ostream& os, const Tree* t) {
    return os << "Tree height is: "
              << t->height << endl;
  }
};

int main() {
  Tree* t = new Tree(40);
  cout << t;
  delete t;
} ///:~
