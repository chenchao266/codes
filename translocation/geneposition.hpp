#ifndef GENEPOSITION_HPP
#define GENEPOSITION_HPP

#include <iostream>
using std::ostream;

namespace Translocation
{
class Permutation;
struct Interval{
	short int begin;
	short int end;
};
bool operator == (const Interval& litvl, const Interval& ritvl);
struct Position{
    short int chromIndex;
    short int geneIndex;
	//Position(){
	//	chromIndex=-1;
	//	geneIndex=-1;
	//}
	//Position(const Position& rPosit)
	//{
	//	chromIndex=rPosit.chromIndex;
	//	geneIndex=rPosit.geneIndex;
	//}
	//Position& operator=(const Position& rPosit)
	//{
	//	if(this==&rPosit)
	//		return *this;
	//	chromIndex=rPosit.chromIndex;
	//	geneIndex=rPosit.geneIndex;
	//	return *this;
	//}
	
};
ostream& operator<<(ostream& os, const Position& posit);
bool operator < (const Position& lpos, const Position& rpos);
bool operator > (const Position& lpos, const Position& rpos);

class GenePosition
{
    short int* beginIndex;
    Position* posit;//
public:
    GenePosition();
    ~GenePosition();
    Position& getPosition(int geneabs)const{
        return posit[geneabs];
    }
    int getBeginIndex(int chi)const{
        return beginIndex[chi];
    }
//    bool isTail(const Permutation& permut,const Position& pst)const{
//        return permut.isTails(pst.chromIndex,pst.geneIndex);
//    }
    void getNextGene(const Permutation& permut,int absgene,Position& postn,int& gene,int & totalindex)const;
    void initPosition(const Permutation& permut);
    void updatePosition(const Permutation& permut, int chn);
    void initBeginIndex(const Permutation& permut);
    void updateBeginIndex(const Permutation& permut, int chrom1, int chrom2);//(chrom1,chrom2]
    //backup();
    //regain();
	GenePosition& operator=(const GenePosition& rgenePosit);
    friend ostream& operator<<(ostream& os, const GenePosition& genePos);

private:
    GenePosition(const GenePosition& rgenePosit);

};

}//namespace
#endif // POSITION_HPP
