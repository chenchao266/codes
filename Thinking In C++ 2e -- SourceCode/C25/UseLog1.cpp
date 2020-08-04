//: C25:UseLog1.cpp {O}
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
#include "LogFile.h"
using namespace std;

void f() {
  logfile() << __FILE__ << endl;
} ///:~
