#include <iostream>
#include <vector>
using namespace std;

class iStack {
  public:
    iStack(int capacity)
        : _stack(capacity), _top(0) { }

    class popOnEmpty { };
    class pushOnFull {
      public:
        pushOnFull(int i) : _value(i) { }
        int value() { return _value; }
      private:
        int _value;
    };

    void pop(int &value) throw(popOnEmpty);
    void push(int value) throw(pushOnFull);

    bool full() { return _top >= _stack.size(); }
    bool empty() { return _top <= 0; }
    void display() { cout << "Stack: " << _top; }

    int size() { return _top; }

  private:
    int _top;
    vector< int > _stack;
};

inline void iStack::pop(int &value) throw (popOnEmpty)
{
    if(empty())
    throw popOnEmpty();

    value = _stack[--_top];
    cout << "iStack::pop(): " << value << endl;
}

inline void iStack::push(int value) throw (pushOnFull)
{
    cout << "iStack::push(): " << value << endl;
    if(full())

    throw pushOnFull(value);

    _stack [_top++] = value;
}

// #include "iStack.h"

int main()
{
    iStack stack(32);

    try {
        stack.display();
        for (int ix = 1; ix < 51; ++ix) {
            if(ix % 3 == 0)
                stack.push(ix);
            if(ix % 4 == 0)
                stack.display();
            if(ix % 10 == 0) {
                int dummy;
                stack.pop(dummy);
                stack.display();
            }
        }
    }

    catch(iStack::pushOnFull eObj) {
        cerr << "trying to push the value " << eObj.value()
             << "on a full stack\n";
    }
    catch(iStack::popOnEmpty) {
        cerr << "trying to pop a value on an empty stack\n";
    }

    return 0;
}
