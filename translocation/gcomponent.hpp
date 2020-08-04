#ifndef GCOMPONENT_HPP
#define GCOMPONENT_HPP

#include <list>
#include <vector>
#include <utility>
#include <iostream>
using std::ostream;
using std::pair;
using std::vector;
using std::list;
#include "gchromtree.hpp"
#include "chromrange.hpp"
#include "geneposition.hpp"
using namespace Translocation;

namespace GeneralTranslocation
{

struct GeneralInterval;
class GeneralPermutation;
class GeneralChromTree;

class PathCycle;

class GeneralComponent
{
    
	GeneralChromTree* m_chromTrees;//??
	int m_totalLeafNumber; //rmsp
	int m_totalTreeNumber; //tree has rmsp
	int m_chromHasLeaf;//has rmsp??

	int m_chromHasSemiLeaf;//has smsp
	int m_totalSemiLeafNumber; //smsp
	int m_totalSemiTreeNumber; //tree has smsp
	int m_totalRealSemiTreeNumber;// <= m_totalSemiTreeNumber !!// tree has rmsp and smsp 
	int m_totalSemiRealTreeNumber;// <= m_totalSemiTreeNumber !!// tree has rmsp and ssp (may has smsp)
    //list<int> m_chromIndex;//chromosome has leaf index ??!!
	vector<GeneralInterval> m_chromComps;// temp components
	short int* m_compBegin;   //temp start position -> component index
	short int* m_compEnd;     //temp end position -> component index
public:
    GeneralComponent();
	~GeneralComponent();

	int  generalAllLeafNumber();

	int  countgf(EITYPE ei,int treenum,int leafnum,int semitreenum,int semileafnum,int realsemitreenum,int semirealtreenum)const;

	int  updategf(const GeneralPermutation& permut, int chi1, int chi2, GeneralChromTree& chrtr1, GeneralChromTree& chrtr2);
	
	void createTotalTree(const GeneralPermutation& permut, const GenePosition& genePosit, const PathCycle& paths);
    void getCaseValue(int& newLeaf, int& newf)const;
    friend int  getAllMergeTransParam(const GeneralPermutation& permut, const GeneralComponent& comps, vector<TransParam>& trsprms);
    friend int  getAllCutTransParam(const GeneralPermutation& permut,  const GeneralComponent& comps, vector<TransParam>& trsprms);
    friend int  getNonLeaf(const GeneralPermutation& permut, const ChromRange& chromrg1,  const GeneralComponent& comps, vector<TransParam>& trsprms);
	friend ostream& operator<<(ostream& os, const GeneralComponent& comps);

private:
	void updateChromLeaf(const GeneralPermutation& permut, int chn, GeneralChromTree& chrtr, int& leafch, int& treech, int& semileafch, int& semitreech, int& rstreech, int& srtreech);
	
	void createChromTree(const GeneralPermutation& permut,const GenePosition& genePosit, const PathCycle& paths, int chn, GeneralChromTree& chrtr);
	EITYPE getEIType(int chi1, int chi2, GeneralChromTree& chrtr1, GeneralChromTree& chrtr2)const;
    EITYPE getEIType()const;
	GeneralComponent(const GeneralComponent& rcomps);
	GeneralComponent& operator=(const GeneralComponent&);

};

}//end of namespace
#endif
