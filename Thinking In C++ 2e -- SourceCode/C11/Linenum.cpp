//: C11:Linenum.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Add line numbers
#include "../require.h"
#include <fstream>
#include <strstream>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[]) {
  requireArgs(argc, 1, "Usage: linenum file\n"
    "Adds line numbers to file");
  strstream text;
  {
    ifstream in(argv[1]);
    assure(in, argv[1]);
    text << in.rdbuf(); // Read in whole file
  } // Close file
  ofstream out(argv[1]); // Overwrite file
  assure(out, argv[1]);
  const int bsz = 100;
  char buf[bsz];
  int line = 0;
  while(text.getline(buf, bsz)) {
    out.setf(ios::right, ios::adjustfield);
    out.width(2);
    out << ++line << ") " << buf << endl;
  }
} ///:~
