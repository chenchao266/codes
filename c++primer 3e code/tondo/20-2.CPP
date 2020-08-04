#include <iostream>
#include <string>
using namespace std;

int main()
{
   string s1, s2, s3;
   int    i;
   double d;

   while (cin >> s1 >> d >> s2 >> i >> s3)
       cout << s1 << "\t" << d << "\t"
            << s2 << "\t" << i << "\t"
            << s3 << endl;

   if (cin.eof())
       cerr << "eof \n";
   else
       cerr << "input error \n";
}

