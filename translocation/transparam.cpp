#include "transparam.hpp"
namespace Translocation
{
ostream& operator<<(ostream& os, const TransParam& trprm)
{
  os<<trprm.chromIndex1<<"."<<trprm.geneIndex1<<","
    <<trprm.chromIndex2<<"."<<trprm.geneIndex2<<","
    <<trprm.transStyle<<";;\t";
  return os;
}
ostream& operator<<(ostream& os, const ExtTransParam& etrprm)
{
	os<<etrprm.cycnum1<<"."<<etrprm.grayIndex1<<","
		<<etrprm.cycnum2<<"."<<etrprm.grayIndex2<<","<<etrprm.leafchange<<";;\t";
	return os;
}
ostream& operator<<(ostream& os, const HTransParam& htrprm)
{
	os<<htrprm.chromIndex1<<"."<<htrprm.geneIndex1<<","
		<<htrprm.chromIndex2<<"."<<htrprm.geneIndex2<<","
		<<htrprm.transStyle<<";\t";
	os<<htrprm.cycnum1<<"."<<htrprm.grayIndex1<<","
		<<htrprm.cycnum2<<"."<<htrprm.grayIndex2<<","<<htrprm.leafchange<<";;\t";
	return os;
}
bool operator==(const TransParam& tp1, const TransParam& tp2)
{
    return((tp1.chromIndex1 ==tp2.chromIndex1)&&
            (tp1.geneIndex1 ==tp2.geneIndex1)&&
            (tp1.chromIndex2==tp2.chromIndex2)&&
            (tp1.geneIndex2 ==tp2.geneIndex2)&&
            (tp1.transStyle ==tp2.transStyle));

}

bool operator<(const TransParam& tp1, const TransParam& tp2)
{
    if(tp1.chromIndex1<tp2.chromIndex1)
        return true;
    else if (tp1.chromIndex1==tp2.chromIndex1){
        if(tp1.geneIndex1<tp2.geneIndex1)
            return true;
        else if(tp1.geneIndex1==tp2.geneIndex1){
            if(tp1.chromIndex2<tp2.chromIndex2)
                return true;
            else if(tp1.chromIndex2==tp2.chromIndex2){
                if(tp1.geneIndex2<tp2.geneIndex2)
                    return true;
                else if(tp1.geneIndex2==tp2.geneIndex2){
                    if(tp1.transStyle<tp2.transStyle)
                        return true;

                }
            }
        }
    }
    return false;
}

HTransParam& HTransParam::operator=(const TransParam& tp1)
{
	this->chromIndex1=tp1.chromIndex1;
	this->chromIndex2=tp1.chromIndex2;
	this->geneIndex1=tp1.geneIndex1;
	this->geneIndex2=tp1.geneIndex2;
	this->transStyle=tp1.transStyle;
	this->cycnum1=-1;
	this->cycnum2=-2;//no same
	this->grayIndex1=-1;
	this->grayIndex2=-2;
	this->leafchange=0;
	return *this;
}
HTransParam::HTransParam(const TransParam& tp1)
{
	*this=tp1;

}
HTransParam::HTransParam()
{

}
HTransParam::HTransParam(const TransParam& tp1,const ExtTransParam& etp)
{
	chromIndex1=tp1.chromIndex1;
	chromIndex2=tp1.chromIndex2;
	geneIndex1=tp1.geneIndex1;
	geneIndex2=tp1.geneIndex2;
	transStyle=tp1.transStyle;
	cycnum1=etp.cycnum1;
	cycnum2=etp.cycnum2;
	grayIndex1=etp.grayIndex1;
	grayIndex2=etp.grayIndex2;
	leafchange=etp.leafchange;
}

}