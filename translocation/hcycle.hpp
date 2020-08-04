#ifndef HCYCLE_HPP
#define HCYCLE_HPP

#include <list>
#include <vector>
#include <utility>
#include <iostream>
using std::ostream;
using std::pair;
using std::vector;
using std::list;

namespace Translocation
{
struct Position;
struct HTransParam;
class GenePosition;
class Permutation;

class HCycle
{

public:
	static int getCycles(const Permutation& permut, const GenePosition& genePosit,vector<HTransParam>& trsprms, bool fill=true);//

private:
	static void getProperTransParam(vector<pair<Position,bool> >& extCycle, vector<HTransParam>& trsprms,int cycIndex);
	HCycle();
	~HCycle();
	HCycle(const HCycle&);
	HCycle& operator=(const HCycle&);
};
}
#endif // CYCLE_HPP
