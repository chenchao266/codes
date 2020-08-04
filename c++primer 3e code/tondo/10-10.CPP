#include <string>
#include <iostream>
using namespace std;

template <class Type>
int count(Type *a, int sz, const Type &which)
{
    int n = 0;
    for (int i = 0; i < sz; ++i)
        if (a[i] == which)
            n++;
    return n;
}

// 以下 explicit specialization 可有可無。
template <class Type>
int count(Type *a, int sz, const char *which)
{
    int n = 0;
    for (int i = 0; i < sz; ++i)
        if (strcmp(a[i], which) == 0)
            n++;
    return n;
}

int main()
{
    double a[] = { 10., 20., 30., 40., 50., 30., 70., 80., 30. };
    cout << "count(a, 9, 30.) should return 3: "
         << count(a, 9, 30.) << endl;

    int b[] = { 10, 20, 30, 40, 50, 30, 70, 80, 30 };
    cout << "count(b, 9, 30) should return 3: "
         << count(b, 9, 30) << endl;

    char c[] = { 'a', 'a', 'c', 'a' };
    cout << "count(c, 4, 'a') should return 3: "
         << count(c, 4, 'a') << endl;

    string d[] = { "aa", "aa", "aa" };
    cout << "count(d, 3, \"aa\") should return 3: "
         << count(d, 3, string("aa")) << endl;

    string e[] = { "bb", "aa", "aa" };
    cout << "count(e, 3, \"aa\") should return 2: "
         << count(e, 3, string("aa")) << endl;

    return 0;
}
