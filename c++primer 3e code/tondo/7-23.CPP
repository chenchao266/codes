
#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    // double (*mfunc[4])(double) = { abs, sin, cos, sqrt };
    // 註：abs 是 int(*)(int)，不是 double(*)(double).
    double (*mfunc[3])(double) = { sin, cos, sqrt };

    // cout << (*mfunc[3])(97.9) << endl;
    cout << (*mfunc[2])(97.9) << endl;

    // 另一種喚起方式
    cout << mfunc[2](97.9) << endl;

    return 0;
}
