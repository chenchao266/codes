#include <string>
#include <iostream>
using namespace std;

class Base {
public:
        operator int()  {  }
        operator const char *();
        // ...
};
class Derived : public Base {
public:
        operator double();
        // ...
};

void operate( double )         { cout << "operate(double)" << endl;  }
void operate( string )         { cout << "operate(string)" << endl;  }
void operate( const Base & )   { cout << "operate(const Base&)" << endl;  }

void calc( int )               { cout << "calc(int)" << endl;  }
void calc( double )            { cout << "calc(double)" << endl;  }
void calc( const Derived & )   { cout << "calc(const Derived&)" << endl;  }

int main()
{
    Derived *pd = new Derived;
    operate( *pd );                  // operate(const Base&)

    Base *pb = new Derived;
    calc( *pb );                     // calc(int)
}
