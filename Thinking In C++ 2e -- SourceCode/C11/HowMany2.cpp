//: C11:HowMany2.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// The copy-constructor
#include <fstream>
#include <cstring>
using namespace std;
ofstream out("HowMany2.out");

class HowMany2 {
  static const int bufsize = 30;
  char name[bufsize]; // Object identifier
  static int object_count;
public:
  HowMany2(const char* id = 0) {
    if(id) strncpy(name, id, bufsize);
    else *name = 0;
    ++object_count;
    print("HowMany2()");
  }
  // The copy-constructor:
  HowMany2(const HowMany2& h) {
    strncpy(name, h.name, bufsize);
    strncat(name, " copy", bufsize - strlen(name));
    ++object_count;
    print("HowMany2(HowMany2&)");
  }
  // Can't be static (printing name):
  void print(const char* msg = 0) const {
    if(msg) out << msg << endl;
    out << '\t' << name << ": "
        << "object_count = "
        << object_count << endl;
  }
  ~HowMany2() {
    --object_count;
    print("~HowMany2()");
  }
};

int HowMany2::object_count = 0;

// Pass and return BY VALUE:
HowMany2 f(HowMany2 x) {
  x.print("x argument inside f()");
  out << "returning from f()" << endl;
  return x;
}

int main() {
  HowMany2 h("h");
  out << "entering f()" << endl;
  HowMany2 h2 = f(h);
  h2.print("h2 after call to f()");
  out << "call f(), no return value" << endl;
  f(h);
  out << "after call to f()" << endl;
} ///:~
