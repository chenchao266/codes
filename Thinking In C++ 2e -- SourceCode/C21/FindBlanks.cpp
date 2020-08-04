//: C21:FindBlanks.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Demonstrate mem_fun_ref() with string::empty()
#include "../require.h"
#include <algorithm>
#include <list>
#include <string>
#include <fstream>
#include <functional>
using namespace std;

typedef list<string>::iterator LSI;

LSI blank(LSI begin, LSI end) {
   return find_if(begin, end, 
     mem_fun_ref(&string::empty));
}

int main(int argc, char* argv[]) {
  requireArgs(argc, 1);
  ifstream in(argv[1]);
  assure(in, argv[1]);  
  list<string> ls;
  string s;
  while(getline(in, s))
    ls.push_back(s);
  LSI lsi = blank(ls.begin(), ls.end());
  while(lsi != ls.end()) {
    *lsi = "A BLANK LINE";
    lsi = blank(lsi, ls.end());
  }
  string f(argv[1]);
  f += ".out";
  ofstream out(f.c_str());
  copy(ls.begin(), ls.end(), 
    ostream_iterator<string>(out, "\n"));
} ///:~
