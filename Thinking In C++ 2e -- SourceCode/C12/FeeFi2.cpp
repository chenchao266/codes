//: C12:FeeFi2.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Copying vs. initialization

class Fi {};

class Fee {
public:
  Fee(int) {}
  Fee(const Fi&) {}
};

class Fo {
  int i;
public:
  Fo(int x = 0) { i = x; }
  operator Fee() const { return Fee(i); }
};

int main() {
  Fo fo;
  Fee fiddle = fo;
} ///:~
