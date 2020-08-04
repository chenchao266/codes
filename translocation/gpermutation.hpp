#ifndef GPERMUTATION_HPP
#define GPERMUTATION_HPP


#include <deque>
#include <iostream>
using std::ostream;
using std::deque;

#include "permutation.hpp"

using namespace Translocation;

namespace GeneralTranslocation
{



class GeneralPermutation: public Permutation
{
	int chromNumber;
public:

	GeneralPermutation(const int* fp, int genenum, const int * chln, int chrnum);//, int* lp
	//GeneralPermutation(const int* fp1, const int * chln1,const int* fp2, const int * chln2);
	~GeneralPermutation();
	GeneralPermutation(const GeneralPermutation& rperm);
	GeneralPermutation& operator=(const GeneralPermutation& rperm);
	const int getChromNumber()const{
		return chromNumber;
	}

	void doGeneralTranslocation(const TransParam& trprm);

	friend void print(ostream& os, const GeneralPermutation& perms);
	friend bool operator==(const GeneralPermutation& lperm, const GeneralPermutation& rperm);
	//friend void addCap( Permutation& A, Permutation& B);
	friend void addCap(GeneralPermutation& A, GeneralPermutation& B);
private:

};
	//void renumber(GeneralPermutation& A, GeneralPermutation& B);
} //end of namespace
#endif
