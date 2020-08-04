//: C16:TStash.h
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// PSTASH using templates
#ifndef TSTASH_H
#define TSTASH_H
#include "../require.h"
#include <cstdlib>

// More convenient than nesting in TStash:
enum Owns { no = 0, yes = 1, Default };
// Declaration required:
template<class Type, int sz> class TStashIter;

template<class Type, int chunksize = 20>
class TStash {
  int quantity;
  int next;
  Owns _owns; // Flag
  void inflate(int increase = chunksize);
protected:
  Type** storage;
public:
  TStash(Owns owns = yes);
  ~TStash();
  Owns owns() const { return _owns; }
  void owns(Owns newOwns) { _owns = newOwns; }
  int add(Type* element);
  int remove(int index, Owns d = Default);
  Type* operator[](int index);
  int count() const { return next; }
  friend class TStashIter<Type, chunksize>;
};

template<class Type, int sz = 20>
class TStashIter {
  TStash<Type, sz>& ts;
  int index;
public:
  TStashIter(TStash<Type, sz>& TS)
    : ts(TS), index(0) {}
  TStashIter(const TStashIter& rv)
    : ts(rv.ts), index(rv.index) {}
  // Jump interator forward or backward:
  void forward(int amount) {
    index += amount;
    if(index >= ts.next) index = ts.next -1;
  }
  void backward(int amount) {
    index -= amount;
    if(index < 0) index = 0;
  }
  // Return value of ++ and -- to be
  // used inside conditionals:
  int operator++() {
    if(++index >= ts.next) return 0;
    return 1;
  }
  int operator++(int) { return operator++(); }
  int operator--() {
    if(--index < 0) return 0;
    return 1;
  }
  int operator--(int) { return operator--(); }
  operator int() {
    return index >= 0 && index < ts.next;
  }
  Type* operator->() {
    Type* t = ts.storage[index];
    if(t) return t;
    require(0,"TStashIter::operator->return 0");
    return 0; // To allow inlining
  }
  // Remove the current element:
  int remove(Owns d = Default){
    return ts.remove(index, d);
  }
};

template<class Type, int sz>
TStash<Type, sz>::TStash(Owns owns) : _owns(owns) {
  quantity = 0;
  storage = 0;
  next = 0;
}

// Destruction of contained objects:
template<class Type, int sz>
TStash<Type, sz>::~TStash() {
  if(!storage) return;
  if(_owns == yes)
    for(int i = 0; i < count(); i++)
      delete storage[i];
  free(storage);
}

template<class Type, int sz>
int TStash<Type, sz>::add(Type* element) {
  if(next >= quantity)
    inflate();
  storage[next++] = element;
  return(next - 1); // Index number
}

template<class Type, int sz>
int TStash<Type, sz>::remove(int index, Owns d){
  if(index >= next || index < 0)
    return 0;
  switch(d) {
    case Default:
      if(_owns != yes) break;
    case yes:
      delete storage[index];
    case no:
      storage[index] = 0; // Position is empty
  }
  return 1;
}

template<class Type, int sz> inline
Type* TStash<Type, sz>::operator[](int index) {
  // Remove check for shipping application:
  require(index >= 0 && index < next);
  return storage[index];
}

template<class Type, int sz>
void TStash<Type, sz>::inflate(int increase) {
  void* v =
    realloc(storage, (quantity+increase)*sizeof(Type*));
  require(v != 0);  // Was it successful?
  storage = (Type**)v;
  quantity += increase;
}
#endif // TSTASH_H ///:~
