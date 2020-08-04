//: C08:Castaway.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// "Casting away" constness

class Y {
  int i;
public:
  Y();
  void f() const;
};

Y:: Y() { i = 0; }

void Y::f() const {
//!    i++; // Error -- const member function
    ((Y*)this)->i++; // OK: cast away const-ness
}

int main() {
  const Y yy;
  yy.f(); // Actually changes it!
} ///:~
