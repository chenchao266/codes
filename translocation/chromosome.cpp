#include <cassert>
#include "chromosome.hpp"



namespace Translocation
{
void Chromosome::assign(const int* fp, int k, int ln)
{
		length=ln;
		for (int j=0; j<ln; ++j) {
			genes.push_back(fp[k++]);//
		}
}
Chromosome::Chromosome():length(0)
{
}
Chromosome::~Chromosome()
{
    //genes.clear();//??
}
void Chromosome::addcap(int cap1, int cap2)
{
	length+=2;	
	genes.push_front(cap1);
	genes.push_back(cap2);
}
void Chromosome::reverse()
{
    reverse(0,length-1);
}
void Chromosome::reverse(int begini, int endi)
{
    for(; begini<endi; ++begini,--endi){
        int agene=-genes[begini];
        genes[begini]=-genes[endi];
        genes[endi]=agene;
    }
    if(begini==endi)
        genes[begini]=-genes[begini];
}
int Chromosome::sum()const
{
	deque<short int>::const_iterator cit= genes.begin();
	int s=0;
	for ( ; cit!= genes.end() ; ++cit ) {
		s+=(*cit);
	}
	return s;
}

Chromosome::Chromosome(const Chromosome& rchrom)
{
		length=rchrom.length;
        genes=rchrom.genes;
}

Chromosome& Chromosome::operator=(const Chromosome& rchrom)
{
    if(this==&rchrom)
        return *this;
	length=rchrom.length;

	genes.clear();///
    genes=rchrom.genes;
    return *this;
}
void doTrans(Chromosome& chrom1,int gene1, Chromosome& chrom2, int gene2, TransStyle trstyl)
{

	int gi1=gene1;
    int gi2=gene2;
	int chromlength1=chrom1.length;
	int chromlength2=chrom2.length;
	assert(gene1>=0 && gene1<chromlength1);
	assert(gene2>=0 && gene2<chromlength2);

	int agene;
	int swaplength;

	int leftlength1=chromlength1-gi1-1;
	int leftlength2=chromlength2-gi2-1;

	if (trstyl==PrefixPrefix) {
	    //(X1,X2)(Y1,Y2)=>(X1,Y2)(Y1,X2)
		swaplength=leftlength1<=leftlength2?leftlength1:leftlength2;
		for (int i=0; i<swaplength; ++i) {
			++gi1;
			++gi2;
			agene=chrom1.genes[gi1];
			chrom1.genes[gi1]=chrom2.genes[gi2];
			chrom2.genes[gi2]=agene;
		}
  //std::cout<<"? do pp trans?"<<gi1<<" , "<<gi2<<std::"\n";
		if (leftlength1<leftlength2) {
			for (++gi2; gi2<chromlength2; ++gi2)
				chrom1.genes.push_back(chrom2.genes[gi2]);
			for (int j=0; j<leftlength2-leftlength1; ++j)
				chrom2.genes.pop_back();

		} else if (leftlength1>leftlength2) {
			for ( ++gi1; gi1<chromlength1; ++gi1)
				chrom2.genes.push_back(chrom1.genes[gi1]);
			for (int j=0; j<leftlength1-leftlength2; ++j)
				chrom1.genes.pop_back();

		}

		chrom1.length=gene1+chromlength2-gene2;
		chrom2.length=gene2+chromlength1-gene1;

	} //PrefixPrefix
	else
	if (trstyl==PrefixSuffix) {
	    //(X1,X2)(Y1,Y2)=>(X1,-Y1)(-X2,Y2)
		swaplength=leftlength1<=(gene2+1)?leftlength1:(gene2+1);
		for (int i=0; i<swaplength; ++i) {
			++gi1;

			agene=-chrom1.genes[gi1];
			chrom1.genes[gi1]=-chrom2.genes[gi2];
			chrom2.genes[gi2]=agene;
			--gi2;
		}

  //std::cout<<"? do ps trans?"<<gi1<<" , "<<gi2<<std::"\n";
		if (leftlength1<(gene2+1)) {
			for ( ; 0<=gi2; --gi2)
				chrom1.genes.push_back(-chrom2.genes[gi2]);
			for (int j=0; j<gene2-leftlength1+1; ++j)
				chrom2.genes.pop_front();

		} else if (leftlength1>(gene2+1)) {
			for (++gi1 ; gi1<chromlength1; ++gi1)
				chrom2.genes.push_front(-chrom1.genes[gi1]);
			for  (int j=0; j<leftlength1-gene2-1; ++j)//??
				chrom1.genes.pop_back();

		}

		chrom1.length=gene1+gene2+2;
		chrom2.length=chromlength1-gene1+chromlength2-gene2-2;

		
	}//PrefixSufix
	assert(chrom1.length>1 && chrom2.length>1);
}
bool operator==(const Chromosome& lchrom, const Chromosome& rchrom)
{
	if (lchrom.length==rchrom.length){
		deque<short int>::const_iterator lcgitor=lchrom.genes.begin();
		
		if (lchrom.genes[0]==rchrom.genes[0] && lchrom.genes[lchrom.length-1]==rchrom.genes[rchrom.length-1] )  {//same orientation
			deque<short int>::const_iterator rcgitor=rchrom.genes.begin();
			for ( ; lcgitor!=lchrom.genes.end() ; ++lcgitor,++rcgitor) {
				if (*lcgitor!=*rcgitor){			
					return false;
				}
			}
			return true;
		}
		else 
		{	
			if (lchrom.genes[0]==-rchrom.genes[rchrom.length-1] && lchrom.genes[lchrom.length-1]==-rchrom.genes[0])//reverse orientation
			{	
				deque<short int>::const_iterator rcgitor=rchrom.genes.end()-1;	
				for ( ; lcgitor!=lchrom.genes.end()-1 ; ++lcgitor,--rcgitor) {//??
					if (*lcgitor!=-(*rcgitor)){			
						return false;
						}
				}
			}
			return true;
		}
		
	}
	return false;

}
bool isEqual(const Chromosome& lchrom, const Chromosome& rchrom)
{
	if (lchrom.length==rchrom.length){
		deque<short int>::const_iterator lcgitor=lchrom.genes.begin()+1;
		deque<short int>::const_iterator rcgitor=rchrom.genes.begin()+1;
		
		if (*lcgitor==*rcgitor){//same orientation
			for ( ; lcgitor!=lchrom.genes.end()-1 ; ++lcgitor,++rcgitor) {
				if (*lcgitor!=*rcgitor){			
					return false;
				}
			}
			return true;
		}
		else 
		{	rcgitor=rchrom.genes.end()-2;
			if (*lcgitor==-(*rcgitor))//reverse orientation
			{		
				for ( ; lcgitor!=lchrom.genes.end()-1 ; ++lcgitor,--rcgitor) {
					if (*lcgitor!=-(*rcgitor)){			
						return false;
					}
				}
			}
			return true;
		}

	}
	return false;
}
void print(ostream& os, const Chromosome& chrom)
{
    os<<"(";
	if(chrom.length>2){
		deque<short int>::const_iterator cgitor=chrom.genes.begin();
		for ( ++cgitor; cgitor!=chrom.genes.end()-1; ++cgitor) {
			os<<(*cgitor)<<",";//??!!
		}
	}
    os<<")";
}
ostream& operator<<(ostream& os, const Chromosome& chrom)
{
    //os<<"   length : "<<chrom.length<<"\n";
    //os<<"   genes : ";


    os<<"(";
	if(chrom.length!=0){
		deque<short int>::const_iterator cgitor=chrom.genes.begin();
		for ( ; cgitor!=chrom.genes.end()-1; ++cgitor) {
			os<<(*cgitor)<<",";//??!!
		}
		os<<(*cgitor);
	}
    os<<")";

	return os;
}


} //end of namespace

