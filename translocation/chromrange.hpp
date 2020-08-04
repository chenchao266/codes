#ifndef CHROMRANGE_HPP
#define CHROMRANGE_HPP

#include <vector>
#include <iostream>
using std::ostream;
using std::vector;

namespace Translocation
{
struct TransParam;

struct ChromRange{
    short int chromIndex;
    short int startIndex;
    short int endIndex;
};
ostream& operator<<(ostream& os, const ChromRange& chromrg);

int getBlackPair(const ChromRange& chromrg1, const ChromRange& chromrg2, vector<TransParam>& trsprms);

}//namespace
#endif // CHROMRANGE_HPP

