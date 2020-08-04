//: C13:Newhandl.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Changing the new-handler
#include <iostream>
#include <cstdlib>
#include <new>
using namespace std;

void out_of_memory() {
  cerr << "memory exhausted!" << endl;
  exit(1);
}

int main() {
  set_new_handler(out_of_memory);
  while(1)
    new int[1000]; // Exhausts memory
} ///:~
