#include <cstdlib>
#include <stack>
#include <cassert>
#include <stdexcept>
#include "gcomponent.hpp"
#include "transparam.hpp"
#include "gpermutation.hpp"

#include "pathcycle.hpp"
using std::stack;

using std::make_pair;
using namespace Translocation;
namespace GeneralTranslocation
{

GeneralComponent::GeneralComponent()
:m_totalLeafNumber(0),m_totalTreeNumber(0),m_chromHasLeaf(0),
m_chromHasSemiLeaf(0),m_totalSemiLeafNumber(0),m_totalSemiTreeNumber(0),m_totalRealSemiTreeNumber(0),m_totalSemiRealTreeNumber(0)
{
	m_compBegin=new short int[Common::geneNumber];
	m_compEnd=new short int[Common::geneNumber];
	m_chromTrees=new GeneralChromTree[Common::chromNumber];
}

GeneralComponent::~GeneralComponent()
{
	m_chromComps.clear();
	delete [] m_compBegin;
	delete [] m_compEnd;
	for (int chi=0; chi<Common::chromNumber; ++chi) {
		m_chromTrees[chi].deleteTP();//clearChromTree(chi);
	}
	delete [] m_chromTrees;
}


void GeneralComponent::createChromTree(const GeneralPermutation& permut,const GenePosition& genePosit, const PathCycle& paths, int chn, GeneralChromTree& chrtr)
{
	m_chromComps.clear();
	int chromCompNumber=0;
	int length=permut.getLength(chn);
	for (int j=0; j<=length; ++j) {//geneNumber
		m_compBegin[j]=0;
		m_compEnd[j]=0;
	}

    chrtr.deleteTP();
	//if(Common::transType==General)
	chromCompNumber=paths.findComps(permut,genePosit, chn, m_chromComps, m_compBegin, m_compEnd);
	if (chromCompNumber!=0) {
		chrtr.constructTP(m_chromComps, m_compBegin, m_compEnd, length);
		chrtr.splitTP(length);
		chrtr.pruneTP(length);
        //chrtr.clearTrees();
	}
}


void GeneralComponent::createTotalTree(const GeneralPermutation& permut, const GenePosition& genePosit, const PathCycle& paths)
{
	for (int chn=0; chn<Common::chromNumber; ++chn) {
		createChromTree(permut,genePosit,paths, chn, m_chromTrees[chn]);
	}
}



int GeneralComponent::generalAllLeafNumber()
{
	m_totalTreeNumber=0;
	m_totalLeafNumber=0;
	m_chromHasLeaf=0;

    m_totalSemiTreeNumber=0;
	m_totalSemiLeafNumber=0;
	m_chromHasSemiLeaf=0;
	m_totalRealSemiTreeNumber=0;
	m_totalSemiRealTreeNumber=0;
	for (int chn=0; chn<Common::chromNumber; ++chn) {
        if ((0!=m_chromTrees[chn].getTreeNumber()) ||
            (0!=m_chromTrees[chn].getSemiTreeNumber())){
            m_chromTrees[chn].countChromLeafNumber();
            if (0!=m_chromTrees[chn].getTreeNumber()){
                ++m_chromHasLeaf;
                m_totalTreeNumber+=m_chromTrees[chn].getTreeNumber();
                
            }
			m_totalLeafNumber+=m_chromTrees[chn].getChromLeafNumber();
            if(0!=m_chromTrees[chn].getSemiTreeNumber()){
                ++m_chromHasSemiLeaf;
                m_totalSemiTreeNumber+=m_chromTrees[chn].getSemiTreeNumber();
                m_totalSemiLeafNumber+=m_chromTrees[chn].getChromSemiLeafNumber();
				if( m_chromTrees[chn].getRealSemiTreeNumber()!=0 ||
					m_chromTrees[chn].getSemiRealTreeNumber()!=0 ){
					m_totalRealSemiTreeNumber+=m_chromTrees[chn].getRealSemiTreeNumber();
					m_totalSemiRealTreeNumber+=m_chromTrees[chn].getSemiRealTreeNumber();
					++m_chromHasLeaf;
				}
				
            }
        }

	}//for
	assert(m_totalRealSemiTreeNumber<=m_totalSemiTreeNumber);
	assert(m_totalSemiRealTreeNumber<=m_totalSemiTreeNumber);
		EITYPE ei=getEIType();
        return m_totalLeafNumber+countgf(ei,m_totalTreeNumber,m_totalLeafNumber,m_totalSemiTreeNumber,m_totalSemiLeafNumber,m_totalRealSemiTreeNumber,m_totalSemiRealTreeNumber);

}

EITYPE GeneralComponent::getEIType()const
{
		if(m_totalLeafNumber==0 || 1!=m_totalTreeNumber+m_totalRealSemiTreeNumber+m_totalSemiRealTreeNumber)
			return Non;	
		for (int chn=0; chn<Common::chromNumber; ++chn) {
			if(1==m_chromTrees[chn].getTreeNumber()+m_chromTrees[chn].getRealSemiTreeNumber()+m_chromTrees[chn].getSemiRealTreeNumber())
				return m_chromTrees[chn].getEIType();
		}
		return Non;
}
EITYPE GeneralComponent::getEIType(int chi1, int chi2, GeneralChromTree& chrtr1, GeneralChromTree& chrtr2)const
{
		if(m_totalLeafNumber==0 || 1!=m_totalTreeNumber+m_totalRealSemiTreeNumber+m_totalSemiRealTreeNumber)
			return Non;	
		for (int chn=0; chn<Common::chromNumber; ++chn) {
			if(chn==chi1 || chn==chi2)
				continue;
			if(1==m_chromTrees[chn].getTreeNumber()+m_chromTrees[chn].getRealSemiTreeNumber()+m_chromTrees[chn].getSemiRealTreeNumber())
				return m_chromTrees[chn].getEIType();
		}
		if(1==chrtr1.getTreeNumber()+chrtr1.getRealSemiTreeNumber()+chrtr1.getSemiRealTreeNumber())
				return chrtr1.getEIType();
		if(1==chrtr2.getTreeNumber()+chrtr2.getRealSemiTreeNumber()+chrtr2.getSemiRealTreeNumber())
				return chrtr2.getEIType();
		return Non;
}

inline
int GeneralComponent::countgf(EITYPE ei,int treenum,int leafnum,int semitreenum,int semileafnum,int realsemitreenum,int semirealtreenum)const
{//(sm+o)/2+f

	int o=0;
	if(leafnum%2==1)
		o=1;
	int d=semileafnum+o;
	if(d%2==1)
		d=d/2+1;
	else
		d=d/2;

	int f=0;

	
	if ((ei==Real && semileafnum==0) ||// real even-isolation
		(ei==Strong && semileafnum==2)){//strong even-isolation and smsp==2
		f=2;
	}
	else if( (ei==Strong && semileafnum>2)||
			(ei==Real && semileafnum>0) ||
			(ei==Weak && semileafnum==1) ||
			(ei==Semi && semileafnum%2==0) ||
			(ei==Semi && semileafnum==1 && semitreenum==1)||
			(o==0 && semileafnum==2 && semitreenum==1 && leafnum==0) ){//ds==1, o==0
		f=1;
	}
	else
		f=0;

	return d+f;
}


void GeneralComponent::updateChromLeaf(const GeneralPermutation& permut, int chn, GeneralChromTree& chrtr, int& leafch, int& treech, int& semileafch, int& semitreech, int& rstreech, int& srtreech)
{

	//createChromTree(permut, chn, chrtr);
	chrtr.countChromLeafNumber();

	leafch=chrtr.getChromLeafNumber()-m_chromTrees[chn].getChromLeafNumber();//leafchange
	treech=chrtr.getTreeNumber()-m_chromTrees[chn].getTreeNumber();//treechange
	semileafch=chrtr.getChromSemiLeafNumber()-m_chromTrees[chn].getChromSemiLeafNumber();//leafchange
	semitreech=chrtr.getSemiTreeNumber()-m_chromTrees[chn].getSemiTreeNumber();//treechange

	rstreech=chrtr.getRealSemiTreeNumber()-m_chromTrees[chn].getRealSemiTreeNumber();//leafchange
	srtreech=chrtr.getSemiRealTreeNumber()-m_chromTrees[chn].getSemiRealTreeNumber();//leafchange
}
int GeneralComponent::updategf(const GeneralPermutation& permut, int chi1, int chi2, GeneralChromTree& chrtr1, GeneralChromTree& chrtr2)
{
	int leafch1, treech1, semileafch1,  semitreech1,  rstreech1,  srtreech1;
	int leafch2, treech2, semileafch2,  semitreech2,  rstreech2,  srtreech2;
    //int oldf=countgf(ei,m_totalTreeNumber,m_totalLeafNumber,m_totalSemiTreeNumber,m_totalSemiLeafNumber,m_totalRealSemiTreeNumber,m_totalSemiRealTreeNumber);
    updateChromLeaf(permut, chi1, chrtr1, leafch1,  treech1,  semileafch1,  semitreech1,  rstreech1,  srtreech1);
	updateChromLeaf(permut, chi2, chrtr2, leafch2,  treech2,  semileafch2,  semitreech2,  rstreech2,  srtreech2);
	EITYPE ei=getEIType( chi1, chi2, chrtr1, chrtr2);
	return countgf(ei,m_totalLeafNumber+leafch1+leafch2, m_totalTreeNumber+treech1+treech2, 
		m_totalSemiTreeNumber+semitreech1+semitreech2, m_totalSemiLeafNumber+semileafch1+semileafch2,
		m_totalRealSemiTreeNumber+rstreech1+rstreech2, m_totalSemiRealTreeNumber+srtreech1+srtreech2);
}

void GeneralComponent::getCaseValue(int& newLeaf, int& newf)const
{

	newLeaf=0;
	newf=0;
//  if (m_totalLeafNumber==0) {
//  }
//  else
    if ((m_totalLeafNumber%2==0)&&(m_totalTreeNumber!=1)) {
        if ((m_chromHasLeaf==1)||(m_chromHasLeaf==2))//¦¤L==0
            newf=1;//count ¦¤f
    }
    else
    if (m_totalLeafNumber%2==1) {//f==1
        newLeaf=1;//0,1
        if ((m_chromHasLeaf==1)||(m_chromHasLeaf==2))//&& (¦¤L==1)
            newf=1;//count ¦¤f
    }
    else
    if ((m_totalLeafNumber%2==0)&&(m_totalTreeNumber==1)) {//f==2
        newLeaf=2;//0,1,2
    }

}


ostream& operator<<(ostream& os,const GeneralComponent& compon)
{
    os<<"components : "<<"\n";
    os<<"  total Leaf Number : "<<compon.m_totalLeafNumber<<"\n";
	os<<"  total Tree Number : "<<compon.m_totalTreeNumber<<"\n";
	os<<"  chromosome Has Leaf : "<<compon.m_chromHasLeaf<<"\n";

    os<<"  total Semi Leaf Number : "<<compon.m_totalSemiLeafNumber<<"\n";
	os<<"  total Semi Tree Number : "<<compon.m_totalSemiTreeNumber<<"\n";
	os<<"  total Real Semi Tree Number : "<<compon.m_totalRealSemiTreeNumber<<"\n";
	os<<"  total Semi Real Tree Number : "<<compon.m_totalSemiRealTreeNumber<<"\n";
	os<<"  chromosome Has Semi Leaf : "<<compon.m_chromHasSemiLeaf<<"\n";

	for (int chi=0; chi<Common::chromNumber; ++chi) {
		os<<"chromosome "<<chi<<" : "<<"\n";
        os<<compon.m_chromTrees[chi];
	}

	os<<"\n";
	return os;
}

}//end of namespace
