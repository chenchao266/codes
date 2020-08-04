#include <cassert>
#include "transparam.hpp"
#include "permutation.hpp"
namespace Translocation
{
int Common::chromNumber;
int Common::geneNumber;
//TransType Common::transType;
//inline
const int Permutation::getGene(int chi, int genei)const
{
	assert(chi<Common::chromNumber);
	return genome[chi].getGene(genei);
}
//inline
const int Permutation::getLength(int chi)const
{
	assert(chi<Common::chromNumber);
	return genome[chi].getLength();
}
void Permutation::setGene(int chi, int genei, int value)
{	
	assert(chi<Common::chromNumber);
	genome[chi].setGene(genei,value);
}
const bool Permutation::isTails(int chi,int gni)const
{
	assert(chi<Common::chromNumber);
	return genome[chi].isTails(gni);
}
const void Permutation::toIntArray(int * arr)const
{
	int t=0;
	for (int i=0; i<Common::chromNumber; ++i) {
		for (int j=0; j<genome[i].getLength(); ++j) {
			arr[t]=genome[i].getGene(j);//
			++t;
		}
	}
}

Permutation::Permutation()
{
}
Permutation::Permutation(const int* fp, const int * chln)//, int* lp
{

	genome=new Chromosome[Common::chromNumber];
	int k=0;

	for (int i=0; i<Common::chromNumber; ++i) {
		genome[i].assign(fp,k,chln[i]);
		k+=chln[i];

	}

}

Permutation::~Permutation()
{
	//for (int chi=0; chi<Common::chromNumber; ++chi) {
	//	genome[chi].~Chromosome();//??
	//}
	delete [] genome;

}

void Permutation::reverseChrom(int chi)
{
    genome[chi].reverse();
}
void Permutation::reverseChrom(int chi, int begini, int endi)
{
    genome[chi].reverse(begini, endi);
}
Permutation::Permutation(const Permutation& rperm)
{
    genome=new Chromosome[Common::chromNumber];
    *this=rperm;
}
Permutation& Permutation::operator=(const Permutation& rperm)
{
    if(this==&rperm)
        return *this;
    for(int i=0; i<Common::chromNumber; ++i){
        genome[i]=rperm.genome[i];
    }
    return *this;
}
void Permutation::backup(Chromosome& chromPermOld, int chi)
{
		chromPermOld=genome[chi];
}
void Permutation::regain(Chromosome& chromPermOld, int chi)
{
		genome[chi]=chromPermOld;
}

int Permutation::sum(int i)const
{
	return genome[i].sum();
}
void Permutation::doTranslocation(const TransParam& trprm)
{

    assert(trprm.chromIndex1<trprm.chromIndex2);

    const int chrom1=trprm.chromIndex1;
    const int chrom2=trprm.chromIndex2;
    const int gene1 =trprm.geneIndex1;
    const int gene2 =trprm.geneIndex2;

#ifdef _DEBUG_OUT
  std::cout<<trprm;
#endif
	doTrans(genome[chrom1], gene1, genome[chrom2], gene2,trprm.transStyle);
}

bool operator==(const Permutation& lperm, const Permutation& rperm)
{//slow??!!
	int equal=0;
	bool noeq=true;
	for (int i=0;i<Common::chromNumber;++i)
	{
		noeq=true;
		for (int j=0;j<Common::chromNumber;++j)
		{
			if (lperm.genome[i]==rperm.genome[j])
			{
				++equal;
				noeq=false;
				break;
			}
		}
		if (noeq)
		{	
			return false;
		}
	}
	if (equal==Common::chromNumber)
		return true;
	else
		return false;

}

ostream& operator<<(ostream& os, const Permutation& perms)
{
	//os<<"{";
	for (int chi=0; chi<Common::chromNumber; ++chi) {
		//os<<"chromosome "<<chi<<" : "<<"\n";
        os<<perms.genome[chi];
	}
	//os<<"}";
    os<<"\n";//
	return os;
}
void renumber(int* a, int* b, int n)
{
	int* c=new int[n+1];//0,1..n
	c[0]=0;
	for (int j=0; j<n; ++j){
		int g=b[j];
		if(g>0)
			c[g]=j+1;
		else
			c[-g]=-(j+1);

	}
	for (int j=0; j<n; ++j){
		int h=a[j];
		if(h>0)
			a[j]=c[h];
		else
			a[j]=-c[-h];

	}
	delete [] c;
}
void renumber( Permutation& A,  Permutation& B)
{
	int* c=new int[Common::geneNumber+1];//0,1..n

	c[0]=0;
	int h=-1;
	int k=0;

	for (int i=0; i<Common::chromNumber; ++i){
		for(int j=0; j<B.getLength(i); ++j){
			int g=B.getGene(i,j);
			if(g>0)
				c[g]=k+1;
			else
				c[-g]=-(k+1);
			++k;
		}

	}

	for (int i=0; i<Common::chromNumber; ++i){
		for(int j=0; j<A.getLength(i); ++j){
			int h=A.getGene(i,j);
			if(h>0)
				A.setGene(i,j,c[h]);
			else
				A.setGene(i,j,-c[-h]);
		}
	}


	delete [] c;
}

} //end of namespace
