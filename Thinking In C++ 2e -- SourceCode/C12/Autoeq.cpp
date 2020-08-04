//: C12:Autoeq.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Automatic operator=()
#include <iostream>
using namespace std;

class Bar {
public:
  Bar& operator=(const Bar&) {
    cout << "inside Bar::operator=()" << endl;
    return *this;
  }
};

class MyType {
  Bar b;
};

int main() {
  MyType a, b;
  a = b; // Prints: "inside Bar::operator=()"
} ///:~
