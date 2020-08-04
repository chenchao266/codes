//: C12:Fanout.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Type conversion fanout

class A {};
class B {};

class C {
public:
  operator A() const;
  operator B() const;
};

// Overloaded h():
void h(A);
void h(B);

int main() {
  C c;
//! h(c); // Error: C -> A or C -> B ???
} ///:~
