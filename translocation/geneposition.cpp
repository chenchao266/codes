#include <cstdlib>

#include "geneposition.hpp"
#include "permutation.hpp"


namespace Translocation
{
bool operator == (const Interval& litvl, const Interval& ritvl)
{
	return (litvl.begin==ritvl.begin && litvl.end==ritvl.end);
}
bool operator > (const Position& lpos, const Position& rpos)
{
	if((lpos.chromIndex>rpos.chromIndex)||(lpos.chromIndex==rpos.chromIndex && lpos.geneIndex>rpos.geneIndex))
		return true;
	else
		return false;
}
bool operator < (const Position& lpos, const Position& rpos){
	if((lpos.chromIndex<rpos.chromIndex)||(lpos.chromIndex==rpos.chromIndex && lpos.geneIndex<rpos.geneIndex))
		return true;
	else
		return false;
}
ostream& operator<<(ostream& os, const Position& posit)
{
  os<<posit.chromIndex<<"."<<posit.geneIndex;
  return os;
}
GenePosition::GenePosition()
{
    posit=new Position[Common::geneNumber+1];//0,1..n
    posit[0].chromIndex=-1;
	posit[0].geneIndex=-1;
	beginIndex=new short int[Common::chromNumber];//0..N
	beginIndex[0]=0;
}

GenePosition::~GenePosition()
{
    delete [] posit;
    delete [] beginIndex;
}
void GenePosition::initPosition(const Permutation& permut)
{
	for (int chi=0; chi<Common::chromNumber; ++chi) {
		updatePosition(permut, chi);
	}

}
void GenePosition::initBeginIndex(const Permutation& permut)
{
    updateBeginIndex(permut,0,Common::chromNumber-1);
}

void GenePosition::updateBeginIndex(const Permutation& permut, int chrom1, int chrom2)
{//(chrom1,chrom2]
    for(int chi=chrom1+1; chi<=chrom2; ++chi){
        beginIndex[chi]=beginIndex[chi-1]+permut.getLength(chi-1);//
    }
}


GenePosition& GenePosition::operator=(const GenePosition& rgenePosit)
{
    if(this!=&rgenePosit){
        for (int i=0; i<=Common::geneNumber; ++i) {
            posit[i].chromIndex=rgenePosit.posit[i].chromIndex;
            posit[i].geneIndex=rgenePosit.posit[i].geneIndex;
        }
        for (int i=0; i<Common::chromNumber; ++i) {
            beginIndex[i]=rgenePosit.beginIndex[i];
        }
	}
	return *this;
}

GenePosition::GenePosition(const GenePosition& rgenePosit)
{
    posit=new Position[Common::geneNumber+1];//0,1..n
    posit[0].chromIndex=-1;
	posit[0].geneIndex=-1;
	beginIndex=new short int[Common::chromNumber];
	beginIndex[0]=0;
	*this=rgenePosit;
}

void GenePosition::updatePosition(const Permutation& permut, int chn)
{
	for(int i=0; i<permut.getLength(chn); ++i){
	    int geneabs=abs(permut.getGene(chn,i));
		posit[geneabs].chromIndex=chn;
		posit[geneabs].geneIndex=i;
	}
}
void GenePosition::getNextGene(const Permutation& permut,int absgene,Position& postn,int& gene,int & totalindex)const
{
    postn=posit[absgene];
    gene=permut.getGene(postn.chromIndex,postn.geneIndex);
    totalindex=beginIndex[postn.chromIndex]+postn.geneIndex;

}

ostream& operator<<(ostream& os, const GenePosition& genePos)
{
	os<<"gene position : "<<"\n";
	for (int i=1; i<=Common::geneNumber; ++i) {
		os<<i<<":";
		os<<genePos.posit[i].chromIndex<<".";
		os<<genePos.posit[i].geneIndex<<"; ";
	}
    os<<"\n";
	os<<"chromosome begin index : "<<"\n";
	for (int i=0; i<Common::chromNumber; ++i) {
		os<<"  chromosome "<<i<<" begin ";
		os<<genePos.beginIndex[i]<<", ";
	}
    os<<"\n";
	return os;
}

}//namespace
