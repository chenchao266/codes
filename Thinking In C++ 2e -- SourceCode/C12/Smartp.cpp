//: C12:Smartp.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Smart pointer example
#include <iostream>
#include <cstring>
using namespace std;

class Obj {
  static int i, j;
public:
  void f() { cout << i++ << endl; }
  void g() { cout << j++ << endl; }
};

// Static member definitions:
int Obj::i = 47;
int Obj::j = 11;

// Container:
class ObjContainer {
  static const int sz = 100;
  Obj* a[sz];
  int index;
public:
  ObjContainer() {
    index = 0;
    memset(a, 0, sz * sizeof(Obj*));
  }
  void add(Obj* obj) {
    if(index >= sz) return;
    a[index++] = obj;
  }
  friend class Sp;
};

// Iterator:
class Sp {
  ObjContainer* oc;
  int index;
public:
  Sp(ObjContainer* objc) {
    index = 0;
    oc = objc;
  }
  // Return value indicates end of list:
  int operator++() { // Prefix
    if(index >= oc->sz) return 0;
    if(oc->a[++index] == 0) return 0;
    return 1;
  }
  int operator++(int) { // Postfix
    return operator++(); // Use prefix version
  }
  Obj* operator->() const {
    if(oc->a[index]) return oc->a[index];
    static Obj dummy;
    return &dummy;
  }
};

int main() {
  const int sz = 10;
  Obj o[sz];
  ObjContainer oc;
  for(int i = 0; i < sz; i++)
    oc.add(&o[i]); // Fill it up
  Sp sp(&oc); // Create an iterator
  do {
    sp->f(); // Smart pointer calls
    sp->g();
  } while(sp++);
} ///:~
