//: C11:HowMany.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Class counts its objects
#include <fstream>
using namespace std;
ofstream out("HowMany.out");

class HowMany {
  static int object_count;
public:
  HowMany() {
    object_count++;
  }
  static void print(const char* msg = 0) {
    if(msg) out << msg << ": ";
    out << "object_count = "
         << object_count << endl;
  }
  ~HowMany() {
    object_count--;
    print("~HowMany()");
  }
};

int HowMany::object_count = 0;

// Pass and return BY VALUE:
HowMany f(HowMany x) {
  x.print("x argument inside f()");
  return x;
}

int main() {
  HowMany h;
  HowMany::print("after construction of h");
  HowMany h2 = f(h);
  HowMany::print("after call to f()");
} ///:~
