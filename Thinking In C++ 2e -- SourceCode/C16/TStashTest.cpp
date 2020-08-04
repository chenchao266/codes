//: C16:TStashTest.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Test TStash
#include "TStash.h"
#include "../require.h"
#include <fstream>
#include <vector>
#include <string>
using namespace std;
ofstream out("tstest.out");

class Int {
  int i;
public:
  Int(int ii = 0) : i(ii) {
    out << ">" << i << endl;
  }
  ~Int() { out << "~" << i << endl; }
  operator int() const { return i; }
  friend ostream&
    operator<<(ostream& os, const Int& x) {
      return os << x.i;
  }
};

int main() {
  TStash<Int> intStash; // Instantiate for Int
  for(int i = 0; i < 30; i++)
    intStash.add(new Int(i));
  TStashIter<Int> intIter(intStash);
  intIter.forward(5);
  for(int j = 0; j < 20; j++, intIter++)
    intIter.remove(); // Default removal
  for(int k = 0; k < intStash.count(); k++)
    if(intStash[k]) // Remove() causes "holes"
      out << *intStash[k] << endl;

  ifstream file("TStashTest.cpp");
  assure(file, "TStashTest.cpp");
  // Instantiate for String:
  TStash<string> stringStash;
  string line;
  while(getline(file, line))
    stringStash.add(new string(line));
  for(int u = 0; u < stringStash.count(); u++)
    if(stringStash[u])
      out << *stringStash[u] << endl;
  TStashIter<string> it(stringStash);
  int n = 25;
  it.forward(n);
  while(it) {
    out << n++ << ": " << it->c_str() << endl;
    it++;
  }
} ///:~
