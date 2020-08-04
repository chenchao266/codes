#include <cassert>
#include "chromosome.hpp"
#include "transparam.hpp"
#include "gpermutation.hpp"



namespace GeneralTranslocation
{
GeneralPermutation::GeneralPermutation(const int* fp, int genenum, const int * chln, int chrnum)//, int* lp
{

	genome=new Chromosome[genenum];
	int k=0;
	for (int i=0; i<chrnum; ++i) {
		genome[i].assign(fp,k,chln[i]);
		k+=chln[i];
	}
	chromNumber=chrnum;
	//for (int i=Common::chromNumber; i<Common::geneNumber; ++i) {
	//	genome[i].length=0;
	//}
	//Common::chromNumber=Common::geneNumber;
	//Common::geneNumber=3*Common::geneNumber;

}
GeneralPermutation::~GeneralPermutation()
{
	//for (int chi=0; chi<Common::chromNumber; ++chi) {
	//	genome[chi].~Chromosome();//??
	//}
	//delete [] genome;

}
GeneralPermutation::GeneralPermutation(const GeneralPermutation& rperm)
{
    genome=new Chromosome[Common::chromNumber];
    *this=rperm;
}
GeneralPermutation& GeneralPermutation::operator=(const GeneralPermutation& rperm)
{
    if(this==&rperm)
        return *this;
    for(int i=0; i<Common::chromNumber; ++i){
        genome[i]=rperm.genome[i];
    }
	chromNumber=rperm.chromNumber;
    return *this;
}

void GeneralPermutation::doGeneralTranslocation(const TransParam& trprm)//has no cap
{

    const int chrom1=trprm.chromIndex1;
    const int chrom2=trprm.chromIndex2;
    const int gene1 =trprm.geneIndex1;
    const int gene2 =trprm.geneIndex2;

#ifdef _DEBUG_OUT
  std::cout<<trprm;
#endif
	if(trprm.chromIndex2==-1){//fission
		//assert(trprm.chromIndex1>=0 && trprm.geneIndex1>=0 && trprm.geneIndex1<genome[trprm.chromIndex1].getLength()-1);
		doTrans(genome[chrom1], gene1, genome[chromNumber], -1,trprm.transStyle);
		++chromNumber;
	}
	else{
		//assert(trprm.chromIndex1>=0);
		if((trprm.geneIndex1==-1 && trprm.geneIndex2==-1 && trprm.transStyle==PrefixSuffix) ||
			(trprm.geneIndex1==genome[trprm.chromIndex1].getLength()-1 && trprm.geneIndex2==genome[trprm.chromIndex2].getLength()-1 && trprm.transStyle==PrefixSuffix) ||
			(trprm.geneIndex1==-1 && trprm.geneIndex2==genome[trprm.chromIndex2].getLength()-1 && trprm.transStyle==PrefixPrefix) ||
			(trprm.geneIndex1==genome[trprm.chromIndex1].getLength()-1 && trprm.geneIndex2==-1 && trprm.transStyle==PrefixPrefix) 
			){//fussion
			//if (trprm.transStyle==PrefixPrefix) {
			//	assert(trprm.geneIndex2>=0);
			//}
			//else{//ps
			//	assert(trprm.geneIndex2<genome[trprm.chromIndex2].getLength()-1);
			//}
			doTrans(genome[chrom1], gene1, genome[chrom2], gene2,trprm.transStyle);
		
			--chromNumber;
		}
		else
			doTrans(genome[chrom1], gene1, genome[chrom2], gene2,trprm.transStyle);
	}

}

//void renumber(GeneralPermutation& A, GeneralPermutation& B)
//{
//	int* c=new int[Common::geneNumber+1];//0,1..n
//
//	c[0]=0;
//	int h=-1;
//	int k=0;
//   
//	for (int i=0; i<Common::chromNumber; ++i){
//		for(int j=0; j<B.getLength(i); ++j){
//			int g=B.getGene(i,j);
//			if(g>0)
//				c[g]=k+1;
//			else
//				c[-g]=-(k+1);
//			++k;
//		}
//		
//	}
//
//	for (int i=0; i<Common::chromNumber; ++i){
//		for(int j=0; j<A.getLength(i); ++j){
//			int h=A.getGene(i,j);
//			if(h>0)
//				A.setGene(i,j,c[h]);
//			else
//				A.setGene(i,j,-c[-h]);
//		}
//	}
//
// 
//    delete [] c;
//	}
//void addCap( Permutation& A, Permutation& B)
//{
//	int n=Common::geneNumber;
//	int m=Common::chromNumber;//A. B. ??
//	int cap=n;
//	Chromosome* newgenome=new Chromosome[n];
//	for (int i=0; i<m; ++i){
//		newgenome[i].length=A.genome[i].length+2;
//		newgenome[i].genes.push_back(++cap);
//		newgenome[i].genes.insert(newgenome[i].genes.end(), A.genome[i].genes.begin(), A.genome[i].genes.end());
//		newgenome[i].genes.push_back(++cap);
//	}
//	for(int i=m; i<n; ++i){
//		newgenome[i].length=2;
//		newgenome[i].genes.push_back(++cap);
//		newgenome[i].genes.push_back(++cap);
//	}
//	delete [] A.genome;
//	A.genome=newgenome;
//	
//	newgenome=new Chromosome[n];
//	for (int i=0; i<m; ++i){
//		newgenome[i].length=B.genome[i].length+2;
//		newgenome[i].genes.push_back(++cap);
//		newgenome[i].genes.insert(newgenome[i].genes.end(), B.genome[i].genes.begin(), B.genome[i].genes.end());
//		newgenome[i].genes.push_back(++cap);
//	}
//	for(int i=m; i<n; ++i){
//		newgenome[i].length=2;
//		newgenome[i].genes.push_back(++cap);
//		newgenome[i].genes.push_back(++cap);
//	}
//	delete [] B.genome;
//	B.genome=newgenome;
//
//	Common::geneNumber=3*n;
//	Common::chromNumber=n;
//
//	renumber(A,B);
//
//}
void addCap(GeneralPermutation& A, GeneralPermutation& B)
{
	int cap=Common::chromNumber;
	for (int i=0; i<Common::chromNumber; ++i){
		A.genome[i].addcap(cap+1, cap+2);
		cap+=2;
	}
	cap=Common::chromNumber;
	for (int i=0; i<Common::chromNumber; ++i){
		B.genome[i].addcap(cap+1, cap+2);
		cap+=2;
	}
#ifdef _DEBUG_OUT
  std::cout<<A;
  std::cout<<B;
#endif
	renumber(A,B);

}
bool operator==(const GeneralPermutation& lperm, const GeneralPermutation& rperm)
{
	if (lperm.chromNumber!=rperm.chromNumber)
		return false;
	
	int equal=0;
	for (int i=0;i<Common::chromNumber;++i)
	{
		if(lperm.genome[i].isGeneralEmpty())
			continue;
		for (int j=0;i<Common::chromNumber;++j)
		{
			if(rperm.genome[j].isGeneralEmpty())
				continue;
			if (isEqual(lperm.genome[i],rperm.genome[j]))
			{
				++equal;
				break;
			}
		}
	}
	if (equal==lperm.chromNumber)
		return true;
	else
		return false;

}

void print(ostream& os, const GeneralPermutation& perms)
{
	//os<<"{";
	for (int chi=0; chi<perms.chromNumber; ++chi) {
		//os<<"chromosome "<<chi<<" : "<<"\n";
        print(os,perms.genome[chi]);
	}
	//os<<"}";
    os<<"\n";//
}


} //end of namespace
