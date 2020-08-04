#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <list>
#include <vector>
#include <utility>
#include <iostream>
using std::ostream;
using std::pair;
using std::vector;
using std::list;

namespace Translocation
{
struct Interval;
struct TransParam;
class Permutation;
class ChromTree;
struct ChromRange;
class GenePosition;
class PathCycle;

class Component
{
	ChromTree* m_chromTrees;//??
	int m_totalLeafNumber;
	int m_totalTreeNumber;
	int m_chromHasLeaf;

    //list<int> m_chromIndex;//chromosome has leaf index ??!!
	vector<Interval> m_chromComps;// temp components
	short int* m_compBegin;   //temp start position -> component index
	short int* m_compEnd;     //temp end position -> component index
public:
    Component();
	~Component();

	int  totalLeafNumber();

	int  countf(int leafnum, int treenum)const;
	int  countgf(int treenum,int leafnum,int semitreenum,int semileafnum)const;

	int  updatef(int leafch, int treech);
	void updateChromLeaf(const Permutation& permut, int chn, ChromTree& chrtr, int& leafch, int& treech);
	void createTotalTree(const Permutation& permut);
	
    void getCaseValue(int& newLeaf, int& newf)const;
    friend int  getAllMergeTransParam(const Permutation& permut, const Component& comps, vector<TransParam>& trsprms);
    friend int  getAllCutTransParam(const Permutation& permut,  const Component& comps, vector<TransParam>& trsprms);
    friend int  getNonLeaf(const Permutation& permut, const ChromRange& chromrg1,  const Component& comps, vector<TransParam>& trsprms);
	friend ostream& operator<<(ostream& os, const Component& comps);

private:
	void createChromTree(const Permutation& permut, int chn, ChromTree& chrtr);
	
	int  findComps(const Permutation& permut, const int chn, vector<Interval>& comps, short int* compBegin, short int* compEnd);
    
	Component(const Component& rcomps);
	Component& operator=(const Component&);

};

}//end of namespace
#endif
