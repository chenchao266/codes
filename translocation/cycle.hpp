#ifndef CYCLE_HPP
#define CYCLE_HPP

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
struct TransParam;
class GenePosition;
class Permutation;

class Cycle
{

public:
	static int getCycles(const Permutation& permut, const GenePosition& genePosit,vector<TransParam>& trsprms, bool fill=true);//

private:
	static void getProperTransParam(vector<pair<Position,bool> >& extCycle, vector<TransParam>& trsprms);
	Cycle();
	~Cycle();
	Cycle(const Cycle&);
	Cycle& operator=(const Cycle&);
};
}
#endif // CYCLE_HPP
