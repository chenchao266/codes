#ifndef GENUMTRANSTREE_HPP
#define GENUMTRANSTREE_HPP

#include <vector>
#include <iostream>
//#include <fstream>
//using std::ofstream;
using std::ostream;
using std::vector;
#include "chromosome.hpp"
#include "geneposition.hpp"
#include "transparam.hpp"
#include "gpermutation.hpp"
using namespace Translocation;

namespace GeneralTranslocation
{

class GeneralEnumTrans;

class GeneralEnumTransTree
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
    GeneralEnumTransTree();
    ~GeneralEnumTransTree();
    void getAllTransSequence(const GeneralPermutation& permut, ostream& os, bool wrt=true);
    friend ostream& operator<<(ostream& os, const GeneralEnumTransTree& enumTransTree);
private:
    void doTrans(vector<TransParam>& trsprms, TransTreeNode* fttnode);

};

}//namespace
#endif // ENUMTRANSTREE_HPP
