
#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    // double (*mfunc[4])(double) = { abs, sin, cos, sqrt };
    // ���Gabs �O int(*)(int)�A���O double(*)(double).
    double (*mfunc[3])(double) = { sin, cos, sqrt };

    // cout << (*mfunc[3])(97.9) << endl;
    cout << (*mfunc[2])(97.9) << endl;

    // �t�@�س�_�覡
    cout << mfunc[2](97.9) << endl;

    return 0;
}
