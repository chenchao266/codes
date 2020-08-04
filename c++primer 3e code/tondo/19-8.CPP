#include <iostream>
#include <stdexcept>
#include <cstdlib>
using namespace std;

int main()
{
    try {
        // use of the C++standard library
    }
    catch (const exception &e) {
        cerr << "exception: " << e.what() << endl;
        abort();
    }
    return 0;
}
