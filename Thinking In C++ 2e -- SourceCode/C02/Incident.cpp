//: C02:Incident.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Records an incident using INCR
#include <cstdlib> // Declare "system()"
using namespace std;

int main() {
  // Other code here...
  system("incr < incident.dat > incident.dat");
} ///:~
