//: C25:UseLog2.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
//{L} UseLog1
#include "LogFile.h"
using namespace std;
void f(); // In lieu of a header file

void g() {
  logfile() << __FILE__ << endl;
} 

int main() {
  f();
  g();
} ///:~
