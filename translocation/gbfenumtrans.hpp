#ifndef GBFENUMTRANS_HPP
#define GBFENUMTRANS_HPP

#include <vector>
#include <iostream>
using std::ostream;
using std::vector;
#include "transparam.hpp"
#include "chromosome.hpp"
#include "gpermutation.hpp"
#include "geneposition.hpp"

using namespace Translocation;

namespace GeneralTranslocation
{

class PathCycle;
class GeneralComponent;
class GeneralChromTree; //??


class GeneralBFEnumTrans
{
    struct BackPerm{
        Chromosome* chrom1;
        Chromosome* chrom2;
        GeneralChromTree*  chree1;
        GeneralChromTree*  chree2;
    };
    BackPerm m_backPerm;
	GenePosition* m_genePosit;
	PathCycle* m_pathCycle;
	GeneralComponent* m_firstComps;
    int m_cycleNumber;
    int m_distance;
	int lf;
    int m_transNumber;
public:
    GeneralBFEnumTrans();
    ~GeneralBFEnumTrans();

    int getDistance(GeneralPermutation& firstPerm, GeneralPermutation& rperm);//const ??
    int updateDistance(GeneralPermutation& firstPerm, GeneralPermutation& rperm);
    int findAllTrans(GeneralPermutation& firstPerm, GeneralPermutation& rperm, ostream& fout, bool wrt);//const ??
	friend ostream& operator<<(ostream& os, const GeneralBFEnumTrans& bfEnumTrans);

protected:
private:
    void doTransParam(GeneralPermutation& perm, GeneralPermutation& rperm, GenePosition& gnpst, TransParam& trprm,ostream& fout,bool wrt);

};
}//namespace
#endif // BFENUMTRANS_HPP
