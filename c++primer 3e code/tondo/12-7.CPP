#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    cout << "please enter a few integers:" << endl;

    istream_iterator<int> input(cin), eos;
    ofstream efile("evenfile");
    ostream_iterator<int> evenfile(efile, "\n");
    ofstream ofile("oddfile");
    ostream_iterator<int> oddfile(ofile, "\n");

    vector<int> vi;

    // 將整數讀入 vector vi
    copy(input, eos, back_inserter(vi));
    vector<int>::iterator iter = vi.begin();

    // 將奇數拷貝至 ofile
    while ((iter = find_if(iter, vi.end(),
                           bind2nd(modulus<int>(), 2)))
            != vi.end()) {
        copy(iter, iter+1, oddfile);
        iter++;
    }

    iter = vi.begin();

    // 將偶數拷貝至 efile
    while ((iter = find_if(iter, vi.end(),
                           not1(bind2nd(modulus<int>(), 2))))
            != vi.end()) {
        copy(iter,iter+1, evenfile);
        iter++;
    }
    cout << "results in 'evenfile' and 'oddfile'\n";

    return 0;
}
