#include <iostream>
#include <hash_set>
using namespace std;

int main()
{
  hash<const char*> H;
  cout << "foo -> " << H("foo") << endl;
  cout << "bar -> " << H("bar") << endl;
}
