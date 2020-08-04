#ifndef HENUMTRANS_HPP
#define HENUMTRANS_HPP

#include <vector>
#include <iostream>
using std::ostream;
using std::vector;


namespace Translocation
{

class Permutation;
class Chromosome;
struct TransParam;

struct HTransParam;
class GenePosition;
class HCycle;
class Component;
class ChromTree; //??


class HEnumTrans
{
    enum TransMode{Proper, Merge, Cut};
    struct BackPerm{
        Chromosome* chrom1;
        Chromosome* chrom2;
        ChromTree*  chree1;
        ChromTree*  chree2;
    };
    BackPerm m_backPerm;
	GenePosition* m_genePosit;
	Component* m_firstComps;
    //HCycle* m_extCycle;
    int m_distance;
    int m_transNumber;
	vector<HTransParam> m_allProperTrans;
	vector<TransParam> m_allMergeTrans;
	vector<TransParam> m_allCutTrans;
	

public:
	HEnumTrans();
	~HEnumTrans();

	int  getDistance(Permutation& firstPerm);//,bool fill=true //const ??
	int  findAllTrans(Permutation& firstPerm); 
	void getAllTrans(vector<HTransParam>& sequen)const;

	void generatePerms(ostream& os, const Permutation& firstPerm)const;

	friend ostream& operator<<(ostream& os, const HEnumTrans& enumTrans);
private:

	void doProperTrans(Permutation& firstPerm, int newLeaf, int newf, vector<HTransParam>& trsprms);

    //void doTransParam(Permutation& perm, vector<TransParam>& trsprms, TransMode trmd);
	HEnumTrans(const HEnumTrans& rperm);
	HEnumTrans& operator=(const HEnumTrans& rperm);
};

}//namespace
#endif // ENUMTRANS_HPP
