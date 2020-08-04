#include <iostream>
#include <sstream>
using namespace std;

int main()
{
    int     ival = 1024;
    double  dval = 3.14159;
    char    cval = 'a';
    char   *sval = "the end";

    ostringstream format_string;
    format_string << "ival: "   << ival
                  << "\tdval: " << dval
                  << "\tcval: " << cval
                  << "\tsval: " << sval;
    cout << format_string.str() << endl;

    return 0;
}
