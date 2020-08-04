#include <cassert>
#include <stdexcept>
#include <algorithm>
#include "henumtrans.hpp"
#include "transparam.hpp"
#include "chromosome.hpp"
#include "permutation.hpp"
#include "geneposition.hpp"
#include "chromtree.hpp"
#include "component.hpp"
#include "hcycle.hpp"



namespace Translocation
{

HEnumTrans::HEnumTrans():m_distance(0),m_transNumber(0)
{
	m_genePosit=new GenePosition();
    m_firstComps=new Component();
    //m_extCycle=new HCycle();
    m_backPerm.chrom1=new Chromosome();
	m_backPerm.chrom2=new Chromosome();//
	m_backPerm.chree1=new ChromTree();
	m_backPerm.chree2=new ChromTree();	//
}

HEnumTrans::~HEnumTrans()
{
    delete m_backPerm.chrom1;
    delete m_backPerm.chrom2;
    delete m_backPerm.chree1;
    delete m_backPerm.chree2;
	delete m_firstComps;
	delete m_genePosit;
	//delete m_extCycle;

}

int HEnumTrans::getDistance(Permutation& firstPerm)//,bool fill
{
    //if(fill){
        m_allCutTrans.clear();
        m_allMergeTrans.clear();
        m_allProperTrans.clear();
    //}
    m_genePosit->initPosition(firstPerm);
	m_genePosit->initBeginIndex(firstPerm);
	int c=HCycle::getCycles(firstPerm, *m_genePosit,m_allProperTrans,true);//¦¤c=1

//#ifdef _DEBUG_OUT
//  std::cout<<"?c?"<<*m_extCycle<<"\n";
//#endif
	m_firstComps->createTotalTree(firstPerm);
	int lf=m_firstComps->totalLeafNumber();
#ifdef _DEBUG_OUT
  std::cout<<"?lf?"<<lf<<"\n";
  std::cout<<(*m_firstComps);
#endif
	m_distance= Common::geneNumber - Common::chromNumber - c + lf;
	return m_distance;

}

//inline
//void HEnumTrans::doTransParam(Permutation& perm, vector<TransParam>& trsprms, TransMode trmd)
//{
//
//	vector<TransParam>::iterator tpitor=trsprms.begin();
//	for (; tpitor!=trsprms.end(); ++tpitor) {
//		int chi1=(*tpitor).chromIndex1;
//		int chi2=(*tpitor).chromIndex2;
//		perm.backup(*m_backPerm.chrom1, chi1);
//		perm.backup(*m_backPerm.chrom2, chi2);
//
//		perm.doTranslocation(*tpitor);
//
//        int leafch1=0;
//        int leafch2=0;
//        int treech1=0;
//        int treech2=0;
//        m_firstComps->updateChromLeaf(perm, chi1,*m_backPerm.chree1,leafch1,treech1);
//        m_firstComps->updateChromLeaf(perm, chi2,*m_backPerm.chree2,leafch2,treech2);
//        int leafch=leafch1+leafch2;
//        int fch=m_firstComps->updatef(leafch,treech1+treech2);//???-fold
//
//        if(trmd==Merge){
//            if((leafch!=-2)||(fch!=0)){
//                std::cout<<perm;
//                throw(std::runtime_error("merge leaf change"));
//            }
//        }
//        else if(trmd==Cut){
//            if((leafch!=-1)||(fch!=-1)){
//                std::cout<<perm;
//                throw(std::runtime_error("cut leaf change"));
//            }
//        }
//
//		perm.regain(*m_backPerm.chrom1, chi1);
//		perm.regain(*m_backPerm.chrom2, chi2);
//	}
//}

inline
void HEnumTrans::doProperTrans(Permutation& perm, int newLeaf, int newf, vector<HTransParam>& trsprms)
{
//    GenePosition gnpst;
    int transize=trsprms.size();
    //vector<HTransParam>::iterator tpitor=trsprms.begin();
	for (int i=0; i<transize; ++i) {
	    bool proper=true;
		int chi1=trsprms[i].chromIndex1;
		int chi2=trsprms[i].chromIndex2;
		perm.backup(*m_backPerm.chrom1, chi1);
		perm.backup(*m_backPerm.chrom2, chi2);

		perm.doTranslocation(trsprms[i]);
#ifdef _DEBUG_OUT
 std::cout<<"?=proper trans=?"<<"\n";
 std::cout<<perm;
#endif

            int leafch1=0;
            int leafch2=0;
            int treech1=0;
            int treech2=0;
            m_firstComps->updateChromLeaf(perm, chi1,*m_backPerm.chree1,leafch1,treech1);
            m_firstComps->updateChromLeaf(perm, chi2,*m_backPerm.chree2,leafch2,treech2);
#ifdef _DEBUG_OUT
  std::cout<<"?proper trans leaf change?"<<leafch1<<","<<leafch2<<"\n";
  std::cout<<(*m_firstComps);
#endif
            int leafch=leafch1+leafch2;
			trsprms[i].leafchange=leafch;// leaf change
			assert(leafch>=0);
            if (newLeaf==0) {
                if (leafch!=0)
                    proper=false;
            }
            else
            if (newLeaf==1) {
                if ((leafch!=0)&&
                    (leafch!=1))//0||1
                    proper=false;
            }
            else
            if (newLeaf==2) {
                if ((leafch!=0)&&
                    (leafch!=1)&&
                    (leafch!=2))//0||1||2
                    proper=false;
            }

            if ((proper)&&(newf==1)&&(newLeaf==leafch)) {
                //fch==0,-1,-2

                int fch=m_firstComps->updatef(leafch,treech1+treech2);
#ifdef _DEBUG_OUT
  std::cout<<"?proper trans f change?"<<fch<<"\n";
#endif

                if (leafch+fch!=0)
                    proper=false;

            }
        //}//if cyclech
		if (proper==false){
		    //int oldsize=trsprms.size();
		    --transize;

		    if(i!=transize)
                trsprms[i]=trsprms[transize];

			--i;
		}

		perm.regain(*m_backPerm.chrom1, chi1);
		perm.regain(*m_backPerm.chrom2, chi2);
	}//for
    //std::cout<<"tsize"<<transize<<"\n";
    trsprms.resize(transize);

}
void HEnumTrans::getAllTrans(vector<HTransParam>& alltrps)const
{	
	alltrps.insert(alltrps.end(),m_allProperTrans.begin(),m_allProperTrans.end());

	vector<TransParam>::const_iterator tpitor=m_allMergeTrans.begin();
	HTransParam htpm;
	for (; tpitor!=m_allMergeTrans.end(); ++tpitor) {
		htpm=(*tpitor);
		htpm.leafchange=-2;
		alltrps.push_back(htpm);

	}
	tpitor=m_allCutTrans.begin();

	for (; tpitor!=m_allCutTrans.end(); ++tpitor) {
		htpm=(*tpitor);
		htpm.leafchange=-1;
		alltrps.push_back(htpm);
	}
}

int HEnumTrans::findAllTrans(Permutation& firstPerm)
{

	if(m_distance==0)
        return 0;
    int newLeaf=0;
	int newf=0;
    m_transNumber=0;


	m_firstComps->getCaseValue(newLeaf, newf);

    //m_extCycle->getProperTransParam(m_allProperTrans);

	if (!m_allProperTrans.empty()) {
		doProperTrans(firstPerm, newLeaf, newf,m_allProperTrans);
	}

    m_transNumber+=m_allProperTrans.size();
	m_transNumber+=getAllMergeTransParam(firstPerm, *m_firstComps, m_allMergeTrans);
	//doTransParam(firstPerm, m_allMergeTrans, Merge);
	m_transNumber+=getAllCutTransParam(firstPerm, *m_firstComps, m_allCutTrans);
	assert(m_transNumber==m_allCutTrans.size()+m_allMergeTrans.size()+m_allProperTrans.size());
    //doTransParam(firstPerm, m_allCutTrans,Cut);
#ifdef _DEBUG_OUT
 std::cout<<"?=find trans=transnum?"<<m_transNumber<<"\n";
#endif
    if(m_transNumber==0){
        std::cout<<firstPerm;
        throw(std::runtime_error("transnumber"));
    }
    return m_transNumber ;
}

void HEnumTrans::generatePerms(ostream& os, const Permutation& firstPerm)const
{
    Permutation permut=firstPerm;
    vector<HTransParam>::const_iterator htpitor=m_allProperTrans.begin();
	for (; htpitor!=m_allProperTrans.end(); ++htpitor) {

		permut=firstPerm;

		permut.doTranslocation(*htpitor);
		os<<"proper:";
		os<<(*htpitor);
		os<<permut;
	}
    vector<TransParam>::const_iterator tpitor=m_allMergeTrans.begin();
	for (; tpitor!=m_allMergeTrans.end(); ++tpitor) {

		permut=firstPerm;
		
		permut.doTranslocation(*tpitor);
		os<<"merge:";
		os<<(*tpitor);
		os<<permut;
	}
    tpitor=m_allCutTrans.begin();
	for (; tpitor!=m_allCutTrans.end(); ++tpitor) {

		permut=firstPerm;
		permut.doTranslocation(*tpitor);
		os<<"cut:";
		os<<(*tpitor);
		os<<permut;
	}

}

ostream& operator<<(ostream& os, const HEnumTrans& enumTrans)
{

	os<<enumTrans.m_distance<<";"<<enumTrans.m_transNumber<<"\n";

	return os;
}
}//namespace
