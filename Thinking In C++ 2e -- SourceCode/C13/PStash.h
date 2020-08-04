//: C13:PStash.h
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Holds pointers instead of objects
#ifndef PSTASH_H
#define PSTASH_H

class PStash {
  int quantity; // Number of storage spaces
  int next; // Next empty space
   // Pointer storage:
  void** storage;
  void inflate(int increase);
public:
  PStash() {
    quantity = 0;
    storage = 0;
    next = 0;
  }
  // No ownership:
  ~PStash() { delete []storage; }
  int add(void* element);
  void* operator[](int index) const; // Fetch
  // Number of elements in Stash:
  int count() const { return next; }
};
#endif // PSTASH_H ///:~
