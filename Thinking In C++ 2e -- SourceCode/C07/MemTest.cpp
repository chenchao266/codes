//: C07:MemTest.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Testing the Mem class
//{L} Mem
#include "Mem.h"
#include <cstring>
#include <iostream>
using namespace std;

class myString {
  Mem* buf;
public:
  myString();
  myString(char* str);
  ~myString();
  void concat(char* str);
  void print(ostream& os);
};

myString::myString() {  buf = 0; }

myString::myString(char* str) {
  buf = new Mem(strlen(str) + 1);
  strcpy((char*)buf->pointer(), str);
}

void myString::concat(char* str) {
  if(!buf) buf = new Mem();
  strcat((char*)buf->pointer(
    buf->msize() + strlen(str) + 1), str);
}

void myString::print(ostream& os) {
  if(!buf) return;
  os << buf->pointer() << endl;
}

myString::~myString() { delete buf; }

int main() {
  myString s("My test string");
  s.print(cout);
  s.concat(" some additional stuff");
  s.print(cout);
  myString s2;
  s2.concat("Using default constructor");
  s2.print(cout);
} ///:~
