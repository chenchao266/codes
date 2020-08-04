//: C12:Refcount.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Reference count, copy-on-write
#include "../require.h"
#include <cstring>
using namespace std;

class Counted {
  class MemBlock {
    static const int size = 100;
    char c[size];
    int refcount;
  public:
    MemBlock() {
      memset(c, 1, size);
      refcount = 1;
    }
    MemBlock(const MemBlock& rv) {
      memcpy(c, rv.c, size);
      refcount = 1;
    }
    void attach() { ++refcount; }
    void detach() {
      require(refcount != 0);
      // Destroy object if no one is using it:
      if(--refcount == 0) delete this;
    }
    int count() const { return refcount; }
    void set(char x) { memset(c, x, size); }
    // Conditionally copy this MemBlock.
    // Call before modifying the block; assign
    // resulting pointer to your block;
    MemBlock* unalias() {
      // Don't duplicate if not aliased:
      if(refcount == 1) return this;
      --refcount;
      // Use copy-constructor to duplicate:
      return new MemBlock(*this);
    }
  }* block;
public:
  Counted() {
    block = new MemBlock; // Sneak preview
  }
  Counted(const Counted& rv) {
    block = rv.block; // Pointer assignment
    block->attach();
  }
  void unalias() { block = block->unalias(); }
  Counted& operator=(const Counted& rv) {
    // Check for self-assignment:
    if(&rv == this) return *this;
    // Clean up what you're using first:
    block->detach();
    block = rv.block; // Like copy-constructor
    block->attach();
    return *this;
  }
  // Decrement refcount, conditionally destroy
  ~Counted() { block->detach(); }
  // Copy-on-write:
  void write(char value) {
    // Do this before any write operation:
    unalias();
    // It's safe to write now.
    block->set(value);
  }
};

int main() {
  Counted A, B;
  Counted C(A);
  B = A;
  C = C;
  C.write('x');
} ///:~
