#ifndef BFENUMTRANS_HPP
#define BFENUMTRANS_HPP

#include <vector>
#include <iostream>
using std::ostream;
using std::vector;


namespace Translocation
{

class Permutation;
class Chromosome;
struct TransParam;

class GenePosition;
class Cycle;
class Component;
class ChromTree; //??


class BFEnumTrans
{
    struct BackPerm{
        Chromosome* chrom1;
        Chromosome* chrom2;
        ChromTree*  chree1;
        ChromTree*  chree2;
    };
    BackPerm m_backPerm;
	GenePosition* m_genePosit;
	Component* m_firstComps;
    int m_cycleNumber;
    int m_distance;
    int m_transNumber;
public:
    BFEnumTrans();
    ~BFEnumTrans();

    int getDistance(Permutation& firstPerm);//const ??
    int updateDistance(Permutation& firstPerm);
    int findAllTrans(Permutation& firstPerm,ostream& fout, bool wrt);//const ??
	static void getAllTrans(const Permutation& permut, vector<TransParam>& trps);
	friend ostream& operator<<(ostream& os, const BFEnumTrans& bfEnumTrans);

protected:
private:
    void doTransParam(Permutation& perm,GenePosition& gnpst, TransParam& trprm,ostream& fout,bool wrt);

};
}//namespace
#endif // BFENUMTRANS_HPP
