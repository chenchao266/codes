#ifndef CHROMOSOME_HPP
#define CHROMOSOME_HPP
#include <cassert>
#include <cstdlib>
#include <deque>
#include <iostream>
using std::ostream;
using std::deque;
#include "transparam.hpp"
namespace Translocation
{

//typedef int Gene;

class Chromosome {
    deque<short int> genes;
    int length;
    //friend class Permutation;
	//friend class GeneralPermutation;
public:
    Chromosome();

    ~Chromosome();
    const int getGene(int i)const {
		assert(0<=i && i<length);
        return genes[i];
    }
    const int getLength()const {
        return length;
    }
	void setGene(int i, int value)  {
		assert(0<=i && i<length);
         genes[i]=value;
    }
    void setLength(int value)  {
        length=value;
    }
    const bool isTails(int gni)const{
      return gni==0||gni==length-1;
    }
	const bool isGeneralEmpty()const{
		return 2==length;
	}
    Chromosome(const Chromosome& rchrom);
    Chromosome& operator=(const Chromosome& rchrom);
	int sum()const;
	void assign(const int* fp, int k, int ln);
	void addcap(int cap1, int cap2);
    void reverse();
    void reverse(int begini, int endi);
	friend void print(ostream& os, const Chromosome& chrom);
    friend ostream& operator<<(ostream& os, const Chromosome& chrom);
	friend bool operator==(const Chromosome& lchrom, const Chromosome& rchrom);
	friend bool isEqual(const Chromosome& lchrom, const Chromosome& rchrom);
	friend void doTrans(Chromosome& chrom1,int gene1, Chromosome& chrom2, int gene2, TransStyle trstyl);
};


} //end of namespace
#endif

