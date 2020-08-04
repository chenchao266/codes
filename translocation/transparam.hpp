#ifndef TRANSPARAM_HPP
#define TRANSPARAM_HPP


#include <iostream>
using std::ostream;


namespace Translocation
{

enum TransStyle {   PrefixPrefix, PrefixSuffix   };
struct TransParam {//invariant: chromIndex1<chromIndex2
	short int chromIndex1;
	short int geneIndex1;
	short int chromIndex2;
	short int geneIndex2;
	TransStyle transStyle;
};
ostream& operator<<(ostream& os, const TransParam& trprm);
bool operator==(const TransParam& tp1, const TransParam& tp2);
bool operator<(const TransParam& tp1, const TransParam& tp2);

struct ExtTransParam{
	short int cycnum1;
	short int cycnum2;
	short int grayIndex1;
	short int grayIndex2;
	short int leafchange;
};
ostream& operator<<(ostream& os, const ExtTransParam& trprm);

struct HTransParam: public TransParam, public ExtTransParam{
	HTransParam();
	//HTransParam(int i);
	HTransParam(const TransParam& tp1);
	HTransParam& operator=(const TransParam& tp);
	HTransParam(const TransParam& tp,const ExtTransParam& etp);
};

ostream& operator<<(ostream& os, const HTransParam& trprm);

} //end of namespace
#endif
