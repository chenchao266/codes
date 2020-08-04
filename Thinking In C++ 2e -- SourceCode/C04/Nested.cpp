//: C04:Nested.cpp {O}
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Linked list with nesting
#include "Stack.h"
#include "../require.h"
using namespace std;

void 
Stack::Link::initialize(void* dat, Link* nxt) {
  data = dat;
  next = nxt;
}

void Stack::initialize() { head = 0; }

void Stack::push(void* dat) {
  Link* newLink = new Link();
  newLink->initialize(dat, head);
  head = newLink;
}

void* Stack::peek() { return head->data; }

void* Stack::pop() {
  if(head == 0) return 0;
  void* result = head->data;
  Link* oldHead = head;
  head = head->next;
  delete oldHead;
  return result;
}

void Stack::cleanup() {
  Link* cursor = head;
  while(head) {
    cursor = cursor->next;
    delete head->data; // Assumes a 'new'!
    delete head;
    head = cursor;
  }
  head = 0; // Officially empty
} ///:~
