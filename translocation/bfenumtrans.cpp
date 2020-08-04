
#include <cassert>
#include <stdexcept>
#include <algorithm>

#include "transparam.hpp"
#include "chromosome.hpp"
#include "permutation.hpp"
#include "geneposition.hpp"
#include "chromtree.hpp"
#include "component.hpp"
#include "cycle.hpp"
#include "bfenumtrans.hpp"

namespace Translocation
{

BFEnumTrans::BFEnumTrans()
{
    m_cycleNumber=0;
	m_genePosit=new GenePosition();
    m_firstComps=new Component();
    //m_extCycle=new Cycle();
    m_backPerm.chrom1=new Chromosome();
	m_backPerm.chrom2=new Chromosome();//
	m_backPerm.chree1=new ChromTree();
	m_backPerm.chree2=new ChromTree();	//
}

BFEnumTrans::~BFEnumTrans()
{
    delete m_backPerm.chrom1;
    delete m_backPerm.chrom2;
    delete m_backPerm.chree1;
    delete m_backPerm.chree2;
	delete m_firstComps;
	delete m_genePosit;
}
int BFEnumTrans::getDistance(Permutation& permut)
{

    m_genePosit->initPosition(permut);
	m_genePosit->initBeginIndex(permut);
	vector<TransParam> vtrsprms;
	m_cycleNumber=Cycle::getCycles(permut, *m_genePosit,vtrsprms,false);

	m_firstComps->createTotalTree(permut);
	int lf=m_firstComps->totalLeafNumber();

	m_distance= Common::geneNumber - Common::chromNumber - m_cycleNumber + lf;
	return m_distance;
}

inline
void BFEnumTrans::doTransParam(Permutation& perm,GenePosition& gnpst, TransParam& trprm,ostream& fout,bool wrt)
{
		perm.backup(*m_backPerm.chrom1, trprm.chromIndex1);
		perm.backup(*m_backPerm.chrom2, trprm.chromIndex2);
		perm.doTranslocation(trprm);

        gnpst=*m_genePosit;///??
        gnpst.updatePosition(perm,trprm.chromIndex1);
        gnpst.updatePosition(perm,trprm.chromIndex2);
        gnpst.updateBeginIndex(perm,trprm.chromIndex1,trprm.chromIndex2);
        vector<TransParam> vtrsprms;
        int cyclech=Cycle::getCycles(perm, gnpst,vtrsprms, false)-m_cycleNumber;

        int leafch1=0;
        int leafch2=0;
        int treech1=0;
        int treech2=0;
        m_firstComps->updateChromLeaf(perm, trprm.chromIndex1,*m_backPerm.chree1,leafch1,treech1);
        m_firstComps->updateChromLeaf(perm, trprm.chromIndex2,*m_backPerm.chree2,leafch2,treech2);
        int leafch=leafch1+leafch2;
        int fch=m_firstComps->updatef(leafch,treech1+treech2);

		perm.regain(*m_backPerm.chrom1, trprm.chromIndex1);
		perm.regain(*m_backPerm.chrom2, trprm.chromIndex2);
       ///¦¤c - ¦¤L - ¦¤f == 1!!
        if(cyclech-leafch-fch==1){
            ++m_transNumber;
            if(wrt){
                fout<<trprm;
                fout<<perm;
            }
        }
}
void BFEnumTrans::getAllTrans(const Permutation& permut, vector<TransParam>& trps)
{
 
	TransParam trprm;

	for(int chi=0; chi<Common::chromNumber; ++chi)
	{
		int chiln=permut.getLength(chi)-1;
		for(int chj=chi+1;chj<Common::chromNumber; ++chj)
		{
			int chjln=permut.getLength(chj)-1;
			trprm.chromIndex1=chi;
			trprm.chromIndex2=chj;
			for(int genei=0; genei<chiln; ++genei){
				trprm.geneIndex1=genei;

				for(int genej=0; genej<chjln; ++genej){
					trprm.geneIndex2=genej;
					trprm.transStyle=PrefixPrefix;

 
						trps.push_back(trprm);
					trprm.transStyle=PrefixSuffix;
 
			 
						trps.push_back(trprm);

				}//for genej
			}//for genei
		}//for chj
	}//for chi
 
 
}
int BFEnumTrans::findAllTrans(Permutation& permut,ostream& fout, bool wrt)
{
    m_transNumber=0;
    if(m_distance==0)
        return m_transNumber;
	
    TransParam trprm;
    //ofstream fout;
    if(wrt){
        //fout.open("bf.txt");
        fout<<permut;
    }
    GenePosition gnpst;

    for(int chi=0; chi<Common::chromNumber; ++chi)
    {
        int chiln=permut.getLength(chi)-1;
        for(int chj=chi+1;chj<Common::chromNumber; ++chj)
        {
            int chjln=permut.getLength(chj)-1;
            trprm.chromIndex1=chi;
            trprm.chromIndex2=chj;
            for(int genei=0; genei<chiln; ++genei){
                trprm.geneIndex1=genei;

                for(int genej=0; genej<chjln; ++genej){
                    trprm.geneIndex2=genej;
                    trprm.transStyle=PrefixPrefix;

                    doTransParam(permut,gnpst,trprm, fout, wrt);
 
                    trprm.transStyle=PrefixSuffix;

                    doTransParam(permut,gnpst,trprm, fout, wrt);
 

                }//for genej
            }//for genei
        }//for chj
    }//for chi
    if(wrt){
        fout<<m_distance<<";"<<m_transNumber<<"\n";
    }

    return m_transNumber;
}
ostream& operator<<(ostream& os, const BFEnumTrans& bfEnumTrans)
{

	os<<bfEnumTrans.m_distance<<";"<<bfEnumTrans.m_transNumber<<"\n";

	return os;
}
}//namespace
