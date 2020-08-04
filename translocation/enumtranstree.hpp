#ifndef ENUMTRANSTREE_HPP
#define ENUMTRANSTREE_HPP
#include "permutation.hpp"
#include "transparam.hpp"
#include <vector>
#include <iostream>
//#include <fstream>
//using std::ofstream;
using std::ostream;
using std::vector;

namespace Translocation
{
struct TransParam;
class Permutation;
class EnumTrans;

class EnumTransTree
{
    struct TransTreeNode{
        TransTreeNode* parent;
        TransParam transprm;
        vector<TransTreeNode*> child;
    };


    int m_initDistance;
    int m_tranSeqNumber;
    TransTreeNode* m_root;
public:
    EnumTransTree();
    ~EnumTransTree();
    void getAllTransSequence(const Permutation& permut, ostream& os, bool wrt=true);
    friend ostream& operator<<(ostream& os, const EnumTransTree& enumTransTree);
private:
    void doTrans(vector<TransParam>& trsprms, TransTreeNode* fttnode);

};

}//namespace
#endif // ENUMTRANSTREE_HPP
