#include <iostream>
#include <string>
using namespace std;

int main()
{
  // (1)
  {
  string s;
  int cnt = 0;
  while (cin >> s)
      cout << ++cnt << "\t" << s << endl;
  }

  // (2)
  {
  string s;

  while (getline(cin, s))
      cout << s << endl;
  }
}
