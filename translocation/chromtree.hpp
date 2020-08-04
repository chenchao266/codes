#ifndef CHROMTREE_HPP
#define CHROMTREE_HPP

#include <utility>
#include <vector>
#include <list>
#include <iostream>
using std::ostream;
using std::list;
using std::vector;
using std::pair;
#include"geneposition.hpp"
namespace Translocation
{
struct TransParam;
struct Interval;
class Permutation;
class Component;
struct ChromRange;
class ChromTree {
    enum NodeStyle { Square, Round };
	struct TreeNode {
		Interval intvl;
		TreeNode * parent;
		list<TreeNode *> child;//vector;

		NodeStyle style;
	};
	struct ChromTreeNode {
		TreeNode* root;//temp perm tree
		
		short int leafNumber;
		
		Interval intvl;
		vector<Interval> leafs;
		
		void deleteTree();
	};
    int treeNumber;
    int chromLeafNumber;

    list<ChromTreeNode*> trees;//vector
    ChromTree(const ChromTree& rchtree);
    ChromTree& operator=(const ChromTree& rchtree);
public:
    ChromTree();
    ~ChromTree();
;
    const int getTreeNumber()const{
        return treeNumber;
    }
    const int getChromLeafNumber()const{
        return chromLeafNumber;
    }

    void constructTP(const vector<Interval>& comps, const short int* start, const short int* end, const int length);
    void pruneTP(const int length);
    void deleteTP();
    void clearTrees();

    void countChromLeafNumber();
    friend int  getAllMergeTransParam(const Permutation& permut, const Component& comps, vector<TransParam>& trsprms);
    friend int  getAllCutTransParam(const Permutation& permut,  const Component& comps, vector<TransParam>& trsprms);
    friend int  getNonLeaf(const Permutation& permut, const ChromRange& chromrg1,  const Component& comps, vector<TransParam>& trsprms);

    friend ostream& operator<<(ostream& os, const ChromTree& chtree);

};

}//end of namespace
#endif

