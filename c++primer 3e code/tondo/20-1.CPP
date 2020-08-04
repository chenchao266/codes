#include <iostream>
#include <vector>
#include <string>
#include <complex>
#include <iterator>
using namespace std;

int main()
{
   string sa[4] = { "pooh", "tigger", "piglet", "eeyore" };
   vector< string > svec( sa, sa+4 );
   string robin( "christopher robin" );
   const char *pc = robin.c_str();
   int ival = 1024;
   char blank = ' ';
   double dval = 3.14159;
   complex<double> purei( 0, 7 );


   vector<string>::iterator iter = svec.begin();
   vector<string>::iterator iter_end = svec.end();

   for ( ; iter != iter_end; iter++)
       cout << *iter << " ";
   cout <<endl;

   ostream_iterator<string> output_string(cout, " ");
   copy(sa, sa + 4, output_string);
   cout << endl;

   copy(svec.begin(), svec.end(), output_string);
   cout << endl;

   cout << "robin:\t" << robin << endl;
   cout << "pc:\t"    << pc << endl;
   cout << "ival:\t"  << ival << endl;
   cout << "dval:\t"  << dval << endl;
   cout << "purei:\t" << purei << endl;

   cout << "pc:\t" << static_cast<void *>(const_cast<char *>(pc))
        << endl;

   cout << (ival < dval ? ival : dval) << endl;
}
