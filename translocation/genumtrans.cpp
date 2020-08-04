#include <cassert>
#include <stdexcept>
#include <algorithm>
#include "genumtrans.hpp"
#include "gchromtree.hpp"
#include "gcomponent.hpp"
#include "pathcycle.hpp"



namespace GeneralTranslocation
{

GeneralEnumTrans::GeneralEnumTrans():m_distance(0),m_transNumber(0)
{
	m_genePosit=new GenePosition();
    m_firstComps=new GeneralComponent();
    m_pathCycle=new PathCycle();
    m_backPerm.chrom1=new Chromosome();
	m_backPerm.chrom2=new Chromosome();//
	m_backPerm.chree1=new GeneralChromTree();
	m_backPerm.chree2=new GeneralChromTree();	//
}

GeneralEnumTrans::~GeneralEnumTrans()
{
    delete m_backPerm.chrom1;
    delete m_backPerm.chrom2;
    delete m_backPerm.chree1;
    delete m_backPerm.chree2;
	delete m_firstComps;
	delete m_genePosit;
	delete m_pathCycle;

}

int GeneralEnumTrans::getDistance(GeneralPermutation& firstPerm)//,bool fill
{
    //if(fill){
        m_allCutTrans.clear();
        m_allMergeTrans.clear();
        m_allProperTrans.clear();
    //}
    m_genePosit->initPosition(firstPerm);
	m_genePosit->initBeginIndex(firstPerm);
	m_pathCycle->getPathCycles(firstPerm, *m_genePosit,m_allProperTrans,true);//fill
	
    int c=m_pathCycle->getPathCyleNumber();
#ifdef _DEBUG_OUT
  std::cout<<"?c?"<<*m_pathCycle<<c<<"\n";
#endif
	m_firstComps->createTotalTree(firstPerm,*m_genePosit,*m_pathCycle);
	int lf=m_firstComps->generalAllLeafNumber();
#ifdef _DEBUG_OUT
  std::cout<<"?lf?"<<lf<<"\n";
  std::cout<<(*m_firstComps);
#endif
	m_distance= Common::geneNumber - Common::chromNumber - c + lf;
	return m_distance;

}

inline
void GeneralEnumTrans::doTransParam(GeneralPermutation& perm, vector<TransParam>& trsprms, TransMode trmd)
{

	vector<TransParam>::iterator tpitor=trsprms.begin();
	for (; tpitor!=trsprms.end(); ++tpitor) {
		int chi1=(*tpitor).chromIndex1;
		int chi2=(*tpitor).chromIndex2;
		perm.backup(*m_backPerm.chrom1, chi1);
		perm.backup(*m_backPerm.chrom2, chi2);

		perm.doGeneralTranslocation(*tpitor);

		perm.regain(*m_backPerm.chrom1, chi1);
		perm.regain(*m_backPerm.chrom2, chi2);
	}
}

inline
void GeneralEnumTrans::doProperTrans(GeneralPermutation& perm, int newLeaf, int newf, vector<TransParam>& trsprms)
{
//    GenePosition gnpst;
    int transize=trsprms.size();
    //vector<TransParam>::iterator tpitor=trsprms.begin();
	for (int i=0; i<transize; ++i) {
	    bool proper=true;
		int chi1=trsprms[i].chromIndex1;
		int chi2=trsprms[i].chromIndex2;
		perm.backup(*m_backPerm.chrom1, chi1);
		perm.backup(*m_backPerm.chrom2, chi2);

		perm.doGeneralTranslocation(trsprms[i]);
#ifdef _DEBUG_OUT
 std::cout<<"?=proper trans=?"<<"\n";
 std::cout<<perm;
#endif


		perm.regain(*m_backPerm.chrom1, chi1);
		perm.regain(*m_backPerm.chrom2, chi2);
	}//for
    //std::cout<<"tsize"<<transize<<"\n";
    trsprms.resize(transize);

}


int GeneralEnumTrans::findAllTrans(GeneralPermutation& firstPerm)
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

void GeneralEnumTrans::generatePerms(ostream& os, const GeneralPermutation& firstPerm)const
{
    GeneralPermutation permut=firstPerm;
    vector<TransParam>::const_iterator tpitor=m_allProperTrans.begin();
	for (; tpitor!=m_allProperTrans.end(); ++tpitor) {

		permut=firstPerm;

		permut.doGeneralTranslocation(*tpitor);
		os<<"proper:";
		os<<(*tpitor);
		os<<permut;
	}
    tpitor=m_allMergeTrans.begin();
	for (; tpitor!=m_allMergeTrans.end(); ++tpitor) {

		permut=firstPerm;

		permut.doGeneralTranslocation(*tpitor);
		os<<"merge:";
		os<<(*tpitor);
		os<<permut;
	}
    tpitor=m_allCutTrans.begin();
	for (; tpitor!=m_allCutTrans.end(); ++tpitor) {

		permut=firstPerm;
		permut.doGeneralTranslocation(*tpitor);
		os<<"cut:";
		os<<(*tpitor);
		os<<permut;
	}

}
void GeneralEnumTrans::getOneTransSequence(const GeneralPermutation& firstPermut,vector<TransParam>& sequen)
{
    GeneralPermutation permut=firstPermut;
    getDistance(permut);
    if(m_distance==0)
        return;
    findAllTrans(permut);

    while ( m_distance!=0){
        TransParam tp;
        if(!(m_allProperTrans.empty()))
            tp=m_allProperTrans.front();
        else if(!(m_allMergeTrans.empty()))
            tp=m_allMergeTrans.front();
        else if(!(m_allCutTrans.empty()))
            tp=m_allCutTrans.front();
        permut.doGeneralTranslocation(tp);
        sequen.push_back(tp);
        getDistance(permut);
        if(m_distance==0)
            break;
        findAllTrans(permut);
    }
}
void GeneralEnumTrans::generateOneTransSequence(ostream& os, const GeneralPermutation& firstPerm, vector<TransParam>& sequen)const
{
    GeneralPermutation permut=firstPerm;
    os<<permut;
    vector<TransParam>::const_iterator tpitor=sequen.begin();
	for (; tpitor!=sequen.end(); ++tpitor) {
		permut.doGeneralTranslocation(*tpitor);
		os<<(*tpitor);
		os<<permut;
	}

}

ostream& operator<<(ostream& os, const GeneralEnumTrans& enumTrans)
{

	os<<enumTrans.m_distance<<";"<<enumTrans.m_transNumber<<"\n";

	return os;
}
}//namespace

