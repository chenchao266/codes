#ifndef PERMUTATION_HPP
#define PERMUTATION_HPP


#include <deque>
#include <iostream>
using std::ostream;
using std::deque;
#include "chromosome.hpp"
namespace Translocation
{

//enum TransType {Reciprocal,General};
struct Common {
	static int geneNumber;
	static int chromNumber;
	//static TransType transType;
};

struct TransParam;
class Chromosome;
class Permutation
{
protected:
	Chromosome* genome;
public:
	Permutation();
	Permutation(const int* fp, const int * chln);//, int* lp

	//Permutation(const int* fp1, const int * chln1,const int* fp2, const int * chln2);
	~Permutation();
	Permutation(const Permutation& rperm);
	Permutation& operator=(const Permutation& rperm);
    const int getGene(int chi, int genei)const;
	void setGene(int chi, int genei, int value);
    const int getLength(int chi)const;
    const bool isTails(int chi,int gni)const;
	const void toIntArray(int * arr)const;
    void reverseChrom(int chi);
    void reverseChrom(int chi, int begini, int endi);
	void doTranslocation(const TransParam& trpr);
	int sum(int i)const;
	void backup(Chromosome& chromPermOld, int chi);
	void regain(Chromosome& chromPermOld, int chi);
	friend ostream& operator<<(ostream& os, const Permutation& perms);
	friend bool operator==(const Permutation& lperm, const Permutation& rperm);

private:

};
void renumber(int* a, int* b, int n);
void renumber( Permutation& a,  Permutation& b );
} //end of namespace
#endif
