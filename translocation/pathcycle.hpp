#ifndef PATHCYCLE_HPP
#define PATHCYCLE_HPP

#include <list>
#include <vector>
#include <utility>
#include <iostream>
using std::ostream;
using std::pair;
using std::vector;
using std::list;
#include "transparam.hpp"
#include "geneposition.hpp"
#include "gpermutation.hpp"
using namespace Translocation;

namespace GeneralTranslocation
{

    enum Tail { non, A, B };
    enum PathType { CYCLE, AA, AB, BB };	
struct GeneralInterval{
	short int begin;
	short int end;
	PathType type;
};
ostream& operator<<(ostream& os,const GeneralInterval& gintvl);
class PathCycle
{

	struct Path{
		PathType type;
		bool intraChrom;//intra chromosome
		Position beginPosit;	// 2*oldgeneIndex || 2*oldgeneIndex+1 // geneIndex/2->oldgeneIndex
		Position endPosit;
		vector<Position> path;

		//Path* next;
	};
	friend ostream& operator<<(ostream& os,const Path& paths);

	vector<pair<Path*, int> > idxPath;//next*
	//vector<pair<int,vector<Path*> > > comps;//next*
    list<Path*> m_AAPath;
    list<Path*> m_ABPath;
    list<Path*> m_BBPath;
    list<Path*> m_cycles;
    int m_AANumber;
    int m_ABNumber;
    int m_BBNumber;
    int m_cycleNumber;
public:

	int getPathCyleNumber()const{
	    return m_cycleNumber + m_AANumber + m_ABNumber;
    }
	void getPathCycles(const GeneralPermutation& permut, const GenePosition& genePosit,vector<TransParam>& trsprms, bool fill=true);//

	int findComps(const GeneralPermutation& permut, const GenePosition& genePosit, const int chn, vector<GeneralInterval>& comps, short int* compBegin, short int* compEnd)const;
	void optimizeCaps(GeneralPermutation& permut);
	//static int updateC(const Permutation& permut,const GenePosition& genePosit);
	friend ostream& operator<<(ostream& os, const PathCycle& cycles);
	PathCycle();
	~PathCycle();
private:
	void splitCycles(const GeneralPermutation& permut, const GenePosition& genePosit, vector<Position>& oneCycle);
	void getProperTransParam(vector<Position>& oneCycle, vector<TransParam>& trsprms);
	void constructInterleave(const GenePosition& genePosit);
	PathCycle(const PathCycle&);
	PathCycle& operator=(const PathCycle&);
};
}
#endif // PATHCYCLE_HPP

