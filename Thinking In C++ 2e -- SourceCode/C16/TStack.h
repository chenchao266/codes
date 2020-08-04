//: C16:TStack.h
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 1999
// Copyright notice in Copyright.txt
// Stack using templates
#ifndef TSTACK_H
#define TSTACK_H

// Declaration required:
template<class T> class TStackIterator;

template<class T> class TStack {
  struct Link {
    T* data;
    Link* next;
    Link(T* dat, Link* nxt) {
      data = dat;
      next = nxt;
    }
  }* head;
  int _owns;
public:
  TStack(int own = 1) : head(0), _owns(own) {}
  ~TStack();
  void push(T* dat) {
    head = new Link(dat,head);
  }
  T* peek() const { return head->data; }
  T* pop();
  int owns() const { return _owns; }
  void owns(int newownership) {
    _owns = newownership;
  }
  friend class TStackIterator<T>;
};

template<class T> T* TStack<T>::pop() {
  if(head == 0) return 0;
  T* result = head->data;
  Link* oldHead = head;
  head = head->next;
  delete oldHead;
  return result;
}

template<class T> TStack<T>::~TStack() {
  Link* cursor = head;
  while(head) {
    cursor = cursor->next;
    // Conditional cleanup of data:
    if(_owns) delete head->data;
    delete head;
    head = cursor;
  }
}

template<class T> class TStackIterator {
  TStack<T>::Link* p;
public:
  TStackIterator(const TStack<T>& tl)
    : p(tl.head) {}
  TStackIterator(const TStackIterator& tl)
    : p(tl.p) {}
  // operator++ returns boolean indicating end:
  int operator++() {
    if(p->next)
      p = p->next;
    else p = 0; // Indicates end of list
    return int(p);
  }
  int operator++(int) { return operator++(); }
  // Smart pointer:
  T* operator->() const {
    if(!p) return 0;
    return p->data;
  }
  T* current() const {
    if(!p) return 0;
    return p->data;
  }
  // int conversion for conditional test:
  operator int() const { return p ? 1 : 0; }
};
#endif // TSTACK_H ///:~
