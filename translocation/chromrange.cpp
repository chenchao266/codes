#include "chromrange.hpp"
#include "transparam.hpp"
#include "permutation.hpp"

namespace Translocation
{
ostream& operator<<(ostream& os, const ChromRange& chromrg)
{
  os<<chromrg.chromIndex<<"."<<chromrg.startIndex<<"~"
    <<chromrg.endIndex<<"\n";
  return os;
}
int getBlackPair(const ChromRange& chromrg1, const ChromRange& chromrg2, vector<TransParam>& trsprms)
{
    TransParam mtrpr;
    if(chromrg1.chromIndex<chromrg2.chromIndex){//make sure chrom1<chrom2

        for (int i=chromrg1.startIndex; i<chromrg1.endIndex; ++i)
            for (int j=chromrg2.startIndex; j<chromrg2.endIndex; ++j) {
                mtrpr.chromIndex1=chromrg1.chromIndex;
                mtrpr.geneIndex1=i;
                mtrpr.chromIndex2=chromrg2.chromIndex;
                mtrpr.geneIndex2=j;
                mtrpr.transStyle=PrefixPrefix;//??
                trsprms.push_back(mtrpr);

                mtrpr.transStyle=PrefixSuffix;
                trsprms.push_back(mtrpr);
            }

    }else if(chromrg1.chromIndex>chromrg2.chromIndex){
        for (int i=chromrg2.startIndex; i<chromrg2.endIndex; ++i)
            for (int j=chromrg1.startIndex; j<chromrg1.endIndex; ++j) {
                mtrpr.chromIndex1=chromrg2.chromIndex;
                mtrpr.geneIndex1=i;
                mtrpr.chromIndex2=chromrg1.chromIndex;
                mtrpr.geneIndex2=j;
                mtrpr.transStyle=PrefixPrefix;//??
                trsprms.push_back(mtrpr);

                mtrpr.transStyle=PrefixSuffix;
                trsprms.push_back(mtrpr);
            }

    }
    return (chromrg1.endIndex - chromrg1.startIndex)*(chromrg2.endIndex - chromrg2.startIndex)*2;
}


}//namespace
