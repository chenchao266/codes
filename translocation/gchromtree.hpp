#ifndef GCHROMTREE_HPP
#define GCHROMTREE_HPP

#include <utility>
#include <vector>
#include <list>
#include <iostream>
using std::ostream;
using std::list;
using std::vector;
using std::pair;
#include "chromrange.hpp"
#include "pathcycle.hpp"
using namespace Translocation;

namespace GeneralTranslocation
{

struct GeneralInterval;
class GeneralPermutation;
class GeneralComponent;
enum EITYPE { Non, Real, Strong, Semi, Weak };
class GeneralChromTree {
    enum NodeStyle { Square, Round };
	struct TreeNode {
		GeneralInterval ginterval;
		TreeNode * parent;
		list<TreeNode *> child;//vector;

		NodeStyle style;
	};
	struct ChromTreeNode {
		TreeNode* root;//temp perm tree
		short int semiRealNode;// semi node contain real leaf!!
		short int leafNumber;//leafs.size();
		short int semiLeafNumber;//semiLeafs.size();

		GeneralInterval gintvl;
		vector<GeneralInterval> leafs;
		vector<GeneralInterval> semiLeafs;//LeafType
		void deleteTree();
	};
    int treeNumber;// has rmsp
    int chromLeafNumber;// rmsp

	int semiTreeNumber;// has smsp
    int chromSemiLeafNumber;// smsp
	int realSemiTreeNumber; // <= semiTreeNumber !!// has rmsp and smsp no ssp
    int semiRealTreeNumber; // <= semiTreeNumber !!// has rmsp and ssp (may has smsp)
    list<ChromTreeNode*> trees;//vector
    GeneralChromTree(const GeneralChromTree& rchtree);
    GeneralChromTree& operator=(const GeneralChromTree& rchtree);
public:
    GeneralChromTree();
    ~GeneralChromTree();

    const int getTreeNumber()const{
        return treeNumber;
    }
    const int getChromLeafNumber()const{
        return chromLeafNumber;
    }
    const int getSemiTreeNumber()const{
        return semiTreeNumber;
    }
    const int getChromSemiLeafNumber()const{
        return chromSemiLeafNumber;
    }
	const int getRealSemiTreeNumber()const{
        return realSemiTreeNumber;
    }
	const int getSemiRealTreeNumber()const{
        return semiRealTreeNumber;
    }
    void constructTP(const vector<GeneralInterval>& comps, const short int* start, const short int* end, const int length);
    void splitTP(const int length);
	void pruneTP(const int length);
    void deleteTP();
    void clearTrees();
	EITYPE getEIType()const;
    void countChromLeafNumber();
    friend int  getAllMergeTransParam(const GeneralPermutation& permut, const GeneralComponent& comps, vector<TransParam>& trsprms);
    friend int  getAllCutTransParam(const GeneralPermutation& permut,  const GeneralComponent& comps, vector<TransParam>& trsprms);
    friend int  getNonLeaf(const GeneralPermutation& permut, const ChromRange& chromrg1,  const GeneralComponent& comps, vector<TransParam>& trsprms);

    friend ostream& operator<<(ostream& os, const GeneralChromTree& chtree);

};

}//end of namespace
#endif

