//: C19:UsingTypename.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Using 'typename' in the template argument list

template<typename T> class X { }; 

int main() {
  X<int> x;
} ///:~
