//: C20:assocGen.h
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// The fill_n() and generate_n() equivalents 
// for associative containers.
#ifndef ASSOCGEN_H
#define ASSOCGEN_H

template<class Assoc, class Count, class T>
void 
assocFill_n(Assoc& a, Count n, const T& val) {
  for (; 0 < n; --n)
    a.insert(val);
}

template<class Assoc, class Count, class Gen>
void assocGen_n(Assoc& a, Count n, Gen g) {
  for (; 0 < n; --n)
    a.insert(g());
}
#endif // ASSOCGEN_H ///:~
