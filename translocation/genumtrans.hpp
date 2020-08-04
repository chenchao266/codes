#ifndef GENUMTRANS_HPP
#define GENUMTRANS_HPP

#include <vector>
#include <iostream>
using std::ostream;
using std::vector;
#include "transparam.hpp"
#include "geneposition.hpp"
#include "gpermutation.hpp"
#include "chromosome.hpp"
using namespace Translocation;

namespace GeneralTranslocation
{


class PathCycle;
class GeneralComponent;
class GeneralChromTree; //??


class GeneralEnumTrans
{
    enum TransMode{Proper, Merge, Cut};
    struct BackPerm{
        Chromosome* chrom1;
        Chromosome* chrom2;
        GeneralChromTree*  chree1;
        GeneralChromTree*  chree2;
    };
    BackPerm m_backPerm;
	GenePosition* m_genePosit;
	GeneralComponent* m_firstComps;
    PathCycle* m_pathCycle;
    int m_distance;
    int m_transNumber;
	vector<TransParam> m_allProperTrans;
	vector<TransParam> m_allMergeTrans;
	vector<TransParam> m_allCutTrans;
	friend class GeneralEnumTransTree;

public:
	GeneralEnumTrans();
	~GeneralEnumTrans();

	int  getDistance(GeneralPermutation& firstPerm);//,bool fill=true //const ??
	int  findAllTrans(GeneralPermutation& firstPerm);//const ??
	void generatePerms(ostream& os, const GeneralPermutation& firstPerm)const;
    void getOneTransSequence(const GeneralPermutation& firstPermut, vector<TransParam>& sequen);
    void generateOneTransSequence(ostream& os, const GeneralPermutation& firstPerm, vector<TransParam>& sequen)const;

	friend ostream& operator<<(ostream& os, const GeneralEnumTrans& enumTrans);
private:
	void doProperTrans(GeneralPermutation& firstPerm, int newLeaf, int newf, vector<TransParam>& trsprms);

    void doTransParam(GeneralPermutation& perm, vector<TransParam>& trsprms, TransMode trmd);
	GeneralEnumTrans(const GeneralEnumTrans& rperm);
	GeneralEnumTrans& operator=(const GeneralEnumTrans& rperm);
};

}//namespace
#endif // ENUMTRANSLOCATION_HPP

