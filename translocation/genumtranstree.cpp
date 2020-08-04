#include <stdexcept>
#include <cassert>
#include "genumtranstree.hpp"

#include "gchromtree.hpp"
#include "genumtrans.hpp"

namespace GeneralTranslocation
{
GeneralEnumTransTree::GeneralEnumTransTree()
:m_initDistance(0),m_tranSeqNumber(0),m_root(NULL)
{
}

GeneralEnumTransTree::~GeneralEnumTransTree()
{

}

void GeneralEnumTransTree::getAllTransSequence(const GeneralPermutation& permut,ostream& os, bool wrt)
{
    GeneralPermutation firstPerm=permut;
    m_root=new TransTreeNode();
    //m_root->transprm=0;
    m_root->parent=NULL;
    m_tranSeqNumber=0;

    GeneralEnumTrans enumTrans;
    m_initDistance=enumTrans.getDistance(firstPerm);//??
    std::cout<<m_initDistance<<"\n";
    if(m_initDistance==0)
        return;
    enumTrans.findAllTrans(firstPerm);
#ifdef _DEBUG_OUT
  std::cout<<enumTrans;
#endif
    m_root->child.reserve(enumTrans.m_transNumber);
    doTrans(enumTrans.m_allProperTrans,m_root);
    doTrans(enumTrans.m_allMergeTrans,m_root);
    doTrans(enumTrans.m_allCutTrans,m_root);

    GeneralPermutation** permpath=new GeneralPermutation* [m_initDistance+1];
    for(int i=0; i<=m_initDistance; ++i)
        permpath[i]=new GeneralPermutation(permut);
    TransTreeNode* fttnode=m_root;
    int curlevel=m_initDistance;
    while(fttnode!=NULL){ //time for space
		for ( ; curlevel>0; --curlevel){

		    //assert(!(fttnode->child.empty()));
            fttnode=fttnode->child[fttnode->child.size()-1];//level[i]
			*permpath[curlevel-1]=*permpath[curlevel];
            permpath[curlevel-1]->doTranslocation(fttnode->transprm);
#ifdef _DEBUG_OUT
  std::cout<<*permpath[curlevel-1];
#endif
            enumTrans.getDistance(*permpath[curlevel-1]);
            if(enumTrans.m_distance!=curlevel-1){
                std::cout<<curlevel-1<<"\n";
                std::cout<<*permpath[curlevel];
                std::cout<<*permpath[curlevel-1];
                throw(std::runtime_error("distance"));
            }

            //assert(enumTrans.m_distance==curlevel-1);
            if(enumTrans.m_distance==0){
                ++m_tranSeqNumber;
                break;
            }
            else {
                enumTrans.findAllTrans(*permpath[curlevel-1]);
#ifdef _DEBUG_OUT
  std::cout<<enumTrans;
#endif
                fttnode->child.reserve(enumTrans.m_transNumber);
                doTrans(enumTrans.m_allProperTrans,fttnode);
                doTrans(enumTrans.m_allMergeTrans,fttnode);
                doTrans(enumTrans.m_allCutTrans,fttnode);

            }
		}
        //vector<TransTreeNode*>(fttnode->child).swap(fttnode->child);
		if(wrt){
            os<<m_tranSeqNumber<<"\n";
            for (int i=m_initDistance ; i>=0; --i){
                os<<*permpath[i];
            }
            if(m_tranSeqNumber%100==0)
                os.flush();//??
        }
		TransTreeNode* pttnode=fttnode->parent;
        delete fttnode;/// ??!!
        curlevel=1;

        if (pttnode!=NULL){
            pttnode->child.pop_back();
            while(pttnode->child.empty()){
                fttnode=pttnode;
                pttnode=pttnode->parent;
                delete fttnode;
                if (pttnode!=NULL)
                    pttnode->child.pop_back();
                else
                    break;
                ++curlevel;
            }
        }
            fttnode=pttnode;

    }//while
    m_root=NULL;
    for(int i=0; i<=m_initDistance; ++i)
        delete permpath[i];
	delete [] permpath;
//#ifdef _DEBUG_OUT
  std::cout<<m_tranSeqNumber<<"\n";
//#endif
}
inline
void GeneralEnumTransTree::doTrans(vector<TransParam>& trsprms, TransTreeNode* fttnode)
{

    for(int i=0; i<trsprms.size(); ++i){

        TransTreeNode* ttnode=new TransTreeNode();
        ttnode->transprm=trsprms[i];
        ttnode->parent=fttnode;
        fttnode->child.push_back(ttnode);

    }
        trsprms.clear();

}

ostream& operator<<(ostream& os, const GeneralEnumTransTree& enumTransTree)
{   //<<"permutation distance: "<<"translocation sequence number : "
    os<<enumTransTree.m_initDistance<<";"<<enumTransTree.m_tranSeqNumber<<"\n";

	return os;
}

}//namespace
