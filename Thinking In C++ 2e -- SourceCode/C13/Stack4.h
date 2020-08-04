//: C13:Stack4.h
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// New version of Stack
#ifndef STACK4_H
#define STACK4_H

class Stack {
  struct Link {
    void* data;
    Link* next;
    Link(void* dat, Link* nxt) {
      data = dat;
      next = nxt;
    }
  }* head;
public:
  Stack() { head = 0; }
  ~Stack();
  void push(void* dat) {
    head = new Link(dat,head);
  }
  void* peek() const { return head->data; }
  void* pop();
};
#endif // STACK4_H ///:~
