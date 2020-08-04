#ifndef MEDIAN_HPP
#define MEDIAN_HPP
#include <functional>
#include <iostream>
namespace Translocation
{
	struct TransParam;
	class Permutation;
class Median
{
	struct Vertex{
		Permutation * label;
		short d1,d2,d3;
		short best;
		short worst;	
	};
	friend std::ostream& operator<< (std::ostream& os, const Median::Vertex& vtx);
	struct VertexCompare:
		public std::binary_function<Vertex, Vertex, bool> {
			bool operator()(const Vertex& lhs, const Vertex& rhs) const
			{
				return lhs.best < rhs.best ;
			}
	};
	struct TransParamCompare:
		public std::binary_function<TransParam, TransParam, bool> {
			bool operator()(const TransParam& lhs, const TransParam& rhs) const
			{
				return lhs < rhs ;
			}
	};
private:
	Permutation* perm1;
	Permutation* perm2;
	Permutation* perm3;
	Permutation* medianperm;
    int d12,d23,d13;
	int medianscore;
	int md1,md2,md3;
public:
	Median(Permutation* permut1,Permutation* permut2,Permutation* permut3);
	~Median(void);
	void computeMedian(ostream& os, bool wrt);
	void computeMedianNM(ostream& os, bool wrt);
	Permutation* getMedian()const{ return medianperm; }
	friend ostream& operator<<(ostream& os,const Median& mdn);
};

}

#endif //MEDIAN_HPP