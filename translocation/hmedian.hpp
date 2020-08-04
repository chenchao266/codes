#ifndef HMEDIAN_HPP
#define HMEDIAN_HPP
#include <functional>
#include <iostream>
#include <utility>
namespace Translocation
{
	struct HTransParam;
	class Permutation;
class HMedian
{
	typedef  std::vector<std::pair<HTransParam, ExtTransParam> > vhetp;
	struct Vertex{
		Permutation * label;
		short d1,d2,d3;
		short score;
		vhetp other; 
	};
	friend std::ostream& operator<< (std::ostream& os, const HMedian::Vertex& vtx);
	//struct VertexCompare:
	//	public std::binary_function<Vertex, Vertex, bool> {
	//		bool operator()(const Vertex& lhs, const Vertex& rhs) const
	//		{
	//			return lhs.score < rhs.score ;
	//		}
	//};
	//struct TransParamCompare:
	//	public std::binary_function<HTransParam, HTransParam, bool> {
	//		bool operator()(const HTransParam& lhs, const HTransParam& rhs) const
	//		{
	//			return lhs < rhs ;
	//		}
	//};
private:
	Permutation* perm1;
	Permutation* perm2;
	Permutation* perm3;
	Permutation* medianperm;
    int d12,d23,d13;
	int medianscore;
	int md1,md2,md3;
public:
	HMedian(Permutation* permut1,Permutation* permut2,Permutation* permut3);
	~HMedian(void);
	void computeMedian(ostream& os, bool wrt);
	Permutation* getMedian()const{ return medianperm; }
	void printScore(ostream& os);
	friend ostream& operator<<(ostream& os,const HMedian& mdn);
};

}

#endif //MEDIAN_HPP