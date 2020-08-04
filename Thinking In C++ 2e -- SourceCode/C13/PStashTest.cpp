//: C13:PStashTest.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
//{L} PStash
// Test of pointer Stash
#include "PStash.h"
#include "../require.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
  PStash intStash;
  // 'new' works with built-in types, too. Note
  // the "pseudo-constructor" syntax:
  for(int i = 0; i < 25; i++)
    intStash.add(new int(i));
  for(int u = 0; u < intStash.count(); u++)
    cout << "intStash[" << u << "] = "
         << *(int*)intStash[u] << endl;

  ifstream infile("PStashTest.cpp");
  assure(infile, "PStashTest.cpp");
  PStash stringStash;
  string line;
  while(getline(infile, line))
    stringStash.add(new string(line));
  // Print out the strings:
  for(int v = 0; stringStash[v]; v++)
    cout << "stringStash[" << v << "] = "
         << *(string*)stringStash[v] << endl;
} ///:~
