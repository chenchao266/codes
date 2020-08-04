//: C16:Recycle.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Containers & polymorphism
#include "TStack.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;
ofstream out("recycle.out");

enum TrashType { AluminumT, PaperT, GlassT };

class Trash {
  float _weight;
public:
  Trash(float wt) : _weight(wt) {}
  virtual TrashType trashType() const = 0;
  virtual const char* name() const = 0;
  virtual float value() const = 0;
  float weight() const { return _weight; }
  virtual ~Trash() {}
};

class Aluminum : public Trash {
  static float val;
public:
  Aluminum(float wt) : Trash(wt) {}
  TrashType trashType() const { return AluminumT; }
  virtual const char* name() const {
    return "Aluminum";
  }
  float value() const { return val; }
  static void value(int newval) {
    val = newval;
  }
};

float Aluminum::val = 1.67;

class Paper : public Trash {
  static float val;
public:
  Paper(float wt) : Trash(wt) {}
  TrashType trashType() const { return PaperT; }
  virtual const char* name() const {
    return "Paper";
  }
  float value() const { return val; }
  static void value(int newval) {
    val = newval;
  }
};

float Paper::val = 0.10;

class Glass : public Trash {
  static float val;
public:
  Glass(float wt) : Trash(wt) {}
  TrashType trashType() const { return GlassT; }
  virtual const char* name() const {
    return "Glass";
  }
  float value() const { return val; }
  static void value(int newval) {
    val = newval;
  }
};

float Glass::val = 0.23;

// Sums up the value of the Trash in a bin:
void sumValue(const TStack<Trash>& bin,ostream& os){
  TStackIterator<Trash> tally(bin);
  float val = 0;
  while(tally) {
    val += tally->weight() * tally->value();
    os << "weight of " << tally->name()
        << " = " << tally->weight() << endl;
    tally++;
  }
  os << "Total value = " << val << endl;
}

int main() {
  srand(time(0)); // Seed random number generator
  TStack<Trash> bin; // Default to ownership
  // Fill up the Trash bin:
  for(int i = 0; i < 30; i++)
    switch(rand() % 3) {
      case 0 :
        bin.push(new Aluminum(rand() % 100));
        break;
      case 1 :
        bin.push(new Paper(rand() % 100));
        break;
      case 2 :
        bin.push(new Glass(rand() % 100));
        break;
    }
  // Bins to sort into:
  TStack<Trash> glassBin(0); // No ownership
  TStack<Trash> paperBin(0);
  TStack<Trash> alBin(0);
  TStackIterator<Trash> sorter(bin);
  // Sort the Trash:
  // (RTTI offers a nicer solution)
  while(sorter) {
    // Smart pointer call:
    switch(sorter->trashType()) {
      case AluminumT:
        alBin.push(sorter.current());
        break;
      case PaperT:
        paperBin.push(sorter.current());
        break;
      case GlassT:
        glassBin.push(sorter.current());
        break;
    }
    sorter++;
  }
  sumValue(alBin, out);
  sumValue(paperBin, out);
  sumValue(glassBin, out);
  sumValue(bin, out);
} ///:~
