
#include <cassert>
#include <stdexcept>
#include <algorithm>

#include "transparam.hpp"
#include "gpermutation.hpp"
#include "gchromtree.hpp"
#include "gcomponent.hpp"
#include "pathcycle.hpp"
#include "gbfenumtrans.hpp"

namespace GeneralTranslocation
{

GeneralBFEnumTrans::GeneralBFEnumTrans()
{
    m_cycleNumber=0;
	m_genePosit=new GenePosition();
    m_firstComps=new GeneralComponent();
    m_pathCycle=new PathCycle();
    m_backPerm.chrom1=new Chromosome();
	m_backPerm.chrom2=new Chromosome();//
	m_backPerm.chree1=new GeneralChromTree();
	m_backPerm.chree2=new GeneralChromTree();	//
}

GeneralBFEnumTrans::~GeneralBFEnumTrans()
{
    delete m_backPerm.chrom1;
    delete m_backPerm.chrom2;
    delete m_backPerm.chree1;
    delete m_backPerm.chree2;
	delete m_firstComps;
	delete m_pathCycle;
	delete m_genePosit;
}
int GeneralBFEnumTrans::getDistance(GeneralPermutation& permut, GeneralPermutation& rpermut)
{
	GeneralPermutation perm=permut;
	GeneralPermutation rperm=rpermut;
	addCap(perm, rperm);
    m_genePosit->initPosition(perm);
	m_genePosit->initBeginIndex(perm);
	vector<TransParam> vtrsprms;
	m_pathCycle->getPathCycles(perm, *m_genePosit,vtrsprms,true);//fill
	
    m_cycleNumber=m_pathCycle->getPathCyleNumber();

	m_firstComps->createTotalTree(perm,*m_genePosit,*m_pathCycle);
	lf=m_firstComps->generalAllLeafNumber();

	m_distance= Common::geneNumber - Common::chromNumber - m_cycleNumber + lf;
	return m_distance;
}

inline
void GeneralBFEnumTrans::doTransParam(GeneralPermutation& perm, GeneralPermutation& rperm, GenePosition& gnpst, TransParam& trprm,ostream& fout,bool wrt)
{
	if (trprm.transStyle==PrefixPrefix){//change caps
		if((trprm.geneIndex1==-1 && trprm.geneIndex2==-1)||
			(trprm.geneIndex1==perm.getLength(trprm.chromIndex1)-1 && trprm.geneIndex2==perm.getLength(trprm.chromIndex2)-1 ))
			return;
	}
	else{//change caps
		if((trprm.geneIndex1==-1 && trprm.geneIndex2==perm.getLength(trprm.chromIndex2)-1)||
			(trprm.geneIndex2==-1 && trprm.geneIndex1==perm.getLength(trprm.chromIndex1)-1))
			return;
	}

		//perm.backup(*m_backPerm.chrom1, trprm.chromIndex1);
		//perm.backup(*m_backPerm.chrom2, trprm.chromIndex2);
		perm.doGeneralTranslocation(trprm);
#ifdef _DEBUG_OUT
  std::cout<<perm;
#endif
		addCap(perm, rperm);///??
		gnpst.initPosition(perm);
		gnpst.initBeginIndex(perm);
        //gnpst=*m_genePosit;
        //gnpst.updatePosition(perm,trprm.chromIndex1);
        //gnpst.updatePosition(perm,trprm.chromIndex2);
        //gnpst.updateBeginIndex(perm,trprm.chromIndex1,trprm.chromIndex2);
        vector<TransParam> vtrsprms;
		m_pathCycle->getPathCycles(perm, gnpst,vtrsprms, false);
		int cyclech=m_pathCycle->getPathCyleNumber()-m_cycleNumber;

        int lfch=m_firstComps->updategf(perm, trprm.chromIndex1,trprm.chromIndex2,*m_backPerm.chree1,*m_backPerm.chree2)-lf;

		//perm.regain(*m_backPerm.chrom1, trprm.chromIndex1);
		//perm.regain(*m_backPerm.chrom2, trprm.chromIndex2);
       ///¦¤c - ¦¤Lf == 1!!
        if(cyclech-lfch==1){
            ++m_transNumber;
            if(wrt){
                fout<<trprm;
                fout<<perm;
            }
        }
}

int GeneralBFEnumTrans::findAllTrans(GeneralPermutation& permut, GeneralPermutation& rpermut, ostream& fout, bool wrt)
{
    m_transNumber=0;
    if(m_distance==0)
        return m_transNumber;
    //vector<TransParam> tps;
    TransParam trprm;
    //ofstream fout;
    if(wrt){
        //fout.open("bf.txt");
        fout<<permut;
    }
    GenePosition gnpst;
	GeneralPermutation perm=permut;
	GeneralPermutation rperm=rpermut;

	for(int chj=0;chj<perm.getChromNumber(); ++chj)
	{

		trprm.chromIndex2=-1;
		trprm.geneIndex2=-1;

		trprm.chromIndex1=chj;


		int chjln=permut.getLength(chj);
		for(int genej=-1; genej<chjln; ++genej){
			trprm.geneIndex1=genej;
			trprm.transStyle=PrefixPrefix;
			perm=permut;
			rperm=rpermut;
			doTransParam(perm,rperm, gnpst,trprm, fout, wrt);

			trprm.transStyle=PrefixSuffix;
			perm=permut;
			rperm=rpermut;
			doTransParam(perm,rperm, gnpst,trprm, fout, wrt);
			//tps.push_back(trprm);

		}//for genej

	}//for chj
	for(int chi=0; chi<perm.getChromNumber(); ++chi)
	{
		int chiln=permut.getLength(chi);
		for(int chj=chi+1;chj<perm.getChromNumber(); ++chj)
		{
			int chjln=permut.getLength(chj);
			trprm.chromIndex1=chi;
			trprm.chromIndex2=chj;
			for(int genei=-1; genei<chiln; ++genei){
				trprm.geneIndex1=genei;

				for(int genej=-1; genej<chjln; ++genej){
					trprm.geneIndex2=genej;
					trprm.transStyle=PrefixPrefix;
					perm=permut;
					rperm=rpermut;
					doTransParam(perm,rperm, gnpst,trprm, fout, wrt);

					trprm.transStyle=PrefixSuffix;
					perm=permut;
					rperm=rpermut;
					doTransParam(perm,rperm, gnpst,trprm, fout, wrt);
					//tps.push_back(trprm);

				}//for genej
			}//for genei
		}//for chj
	}//for chi
	if(wrt){
		fout<<m_distance<<";"<<m_transNumber<<"\n";
	}

	return m_transNumber;
}
ostream& operator<<(ostream& os, const GeneralBFEnumTrans& bfEnumTrans)
{

	os<<bfEnumTrans.m_distance<<";"<<bfEnumTrans.m_transNumber<<"\n";

	return os;
}
}//namespace
