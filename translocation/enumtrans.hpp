#ifndef ENUMTRANS_HPP
#define ENUMTRANS_HPP

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


class EnumTrans
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
    //Cycle* m_extCycle;
    int m_distance;
    int m_transNumber;
	vector<TransParam> m_allProperTrans;
	vector<TransParam> m_allMergeTrans;
	vector<TransParam> m_allCutTrans;
	friend class EnumTransTree;

public:
	EnumTrans();
	~EnumTrans();

	int  getDistance(Permutation& firstPerm);//,bool fill=true //const ??
	int  findAllTrans(Permutation& firstPerm); 
	void getAllTrans(vector<TransParam>& sequen)const;
	void getNeutralTrans(vector<TransParam>& sequen)const;
	void generatePerms(ostream& os, const Permutation& firstPerm)const;
    void getOneTransSequence(const Permutation& firstPermut, vector<TransParam>& sequen);
    void generateOneTransSequence(ostream& os, const Permutation& firstPerm, vector<TransParam>& sequen)const;

	friend ostream& operator<<(ostream& os, const EnumTrans& enumTrans);
private:

	void doProperTrans(Permutation& firstPerm, int newLeaf, int newf, vector<TransParam>& trsprms);

    void doTransParam(Permutation& perm, vector<TransParam>& trsprms, TransMode trmd);
	EnumTrans(const EnumTrans& rperm);
	EnumTrans& operator=(const EnumTrans& rperm);
};

}//namespace
#endif // ENUMTRANS_HPP
