//: C10:StaticArray.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Initializing static arrays

class Values {
  // static consts can be initialized in-place:
  static const int scSize = 100;
  // Automatic counting works with static consts:
  static const float scTable[] = {
    1.1, 2.2, 3.3, 4.4
  };
  static const char scLetters[] = {
    'a', 'b', 'c', 'd', 'e',
    'f', 'g', 'h', 'i', 'j'
  };
  // Non-const statics must be 
  // initialized externally:
  static int size;
  static float table[4];
  static char letters[10];
};

int Values::size = 100;

float Values::table[4] = {
  1.1, 2.2, 3.3, 4.4
};

char Values::letters[10] = {
  'a', 'b', 'c', 'd', 'e',
  'f', 'g', 'h', 'i', 'j'
};

int main() { Values v; } ///:~
