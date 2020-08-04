#include <cstdlib>
#include <stack>
#include <cassert>
#include <stdexcept>
#include "component.hpp"
#include "transparam.hpp"
#include "permutation.hpp"
#include "chromtree.hpp"
#include "geneposition.hpp"

using std::stack;

using std::make_pair;
namespace Translocation
{

Component::Component()
:m_totalLeafNumber(0),m_totalTreeNumber(0),m_chromHasLeaf(0)

{
	m_compBegin=new short int[Common::geneNumber];
	m_compEnd=new short int[Common::geneNumber];
	m_chromTrees=new ChromTree[Common::chromNumber];
}

Component::~Component()
{
	m_chromComps.clear();
	delete [] m_compBegin;
	delete [] m_compEnd;
	for (int chi=0; chi<Common::chromNumber; ++chi) {
		m_chromTrees[chi].deleteTP();//clearChromTree(chi);
	}
	delete [] m_chromTrees;
}

int Component::findComps(const Permutation& permut, int chn, vector<Interval>& comps, short int* compBegin, short int* compEnd)
{

	int length=permut.getLength(chn);

	int compNumber=0;
	int maxelem=Common::geneNumber+1;//??
	stack<short int> m1;
	stack<short int> m2;
	stack<short int> s1;
	stack<short int> s2;
	m1.push(maxelem);//??
	m2.push(0);//??
	s1.push(0);
	s2.push(0);
	short int* gr=new short int[length+1];
	short int* sm=new short int[length+1];
	gr[0]=maxelem;//??
	sm[0]=0;//??
	Interval itvl;
    int s;
    int fpi;
	int fpiabs;
	int fpjabs;
    int fps;
	int fpsabs;
	int t=0;
	for (int i=1; i<length; ++i) {//i=1..length-1
		//compute the gr[i]
		fpi=permut.getGene(chn,i);
		fpiabs=abs(fpi);
        fpjabs=abs(permut.getGene(chn,i-1));

		if (fpjabs>fpiabs) {
			m1.push(fpjabs);
		} else {
			while ((!m1.empty())&&(m1.top()<fpiabs))
				m1.pop();
		}
		gr[i]=m1.top();//
		if(gr[i]==maxelem)
            t=i;
#ifdef _DEBUG_OUT
 //std::cout<<chn<<"?find comps=i:gr[i]:t?"<<i<<":"<<gr[i]<<":"<<t<<"\n";
#endif
		//find direct components
		if (!s1.empty()) {
			s=s1.top();
			fps=permut.getGene(chn,s);
			fpsabs=abs(fps);

			while ((fpsabs>fpiabs)||(gr[s]<fpiabs)) {//??!!
				s1.pop();
				if (!s1.empty()) {
					s=s1.top();
					fps=permut.getGene(chn,s);
					fpsabs=abs(fps);
				} else
					break;
			}

			if ((fpi>0)&&(gr[i]==gr[s])&&(i-s==fpi-fps)) {//fpiabs-fpsabs
#ifdef _DEBUG_OUT
 std::cout<<chn<<"?find comps=direct?"<<s<<"~"<<i<<"\n";
#endif
 itvl.begin=s;
 itvl.end=i;
				comps.push_back(itvl);//
				++compNumber;
				if(compBegin[s]!=0){
				    std::cout<<permut;
                    throw(std::runtime_error("components"));

				}
				//assert(compBegin[s]==0);
				compBegin[s]=compNumber;
				compEnd[i]=compNumber;

            }
		}
		//compute the sm[i]
		if (fpjabs<fpiabs) {
			m2.push(fpjabs);
		} else {
			while ((!m2.empty())&&(m2.top()>fpiabs))
				m2.pop();
		}
		sm[i]=m2.top();//

		//find reversed components
		if (!s2.empty()) {
			s=s2.top();
			fps=permut.getGene(chn,s);
			fpsabs=abs(fps);
			while ( (s>t)&&
                    //(gr[s]!=gr[0])&&
                    ((fpsabs<fpiabs)||(sm[s]>fpiabs)) ) {
				s2.pop();
#ifdef _DEBUG_OUT
 std::cout<<chn<<"?find comps=s:gr[s]?"<<s<<":"<<gr[s]<<"\n";
#endif
				if (!s2.empty()) {
					s=s2.top();
					fps=permut.getGene(chn,s);
					fpsabs=abs(fps);
				} else
					break;
			}

			if ((s>=t)&&(fpi<0)&&
                (sm[i]==sm[s])&&
                (sm[s]==sm[s+1])&&
                //(gr[i]<=gr[s+1])&&//??
                (i-s==fpi-fps)) {//fpsabs-fpiabs
#ifdef _DEBUG_OUT
  std::cout<<chn<<"?find comps=reversed?"<<s<<"~"<<i<<"\n";
#endif
  itvl.begin=s;
  itvl.end=i;
				comps.push_back(itvl);//
				++compNumber;
				if(compBegin[s]!=0){
				    std::cout<<permut;
                    throw(std::runtime_error("components"));

				}
				//assert(compBegin[s]==0);
				compBegin[s]=compNumber;
				compEnd[i]=compNumber;

			}
		}
		//update stack
		if (fpi>0) {
			s1.push(i);
		} else {
			s2.push(i);
		}

	}
	delete [] gr;
	delete [] sm;
	return compNumber;
}

void Component::createChromTree(const Permutation& permut, int chn, ChromTree& chrtr)
{
	m_chromComps.clear();
	int chromCompNumber=0;
	int length=permut.getLength(chn);
	for (int j=0; j<=length; ++j) {//geneNumber
		m_compBegin[j]=0;
		m_compEnd[j]=0;
	}

    chrtr.deleteTP();
	chromCompNumber=findComps(permut, chn, m_chromComps, m_compBegin, m_compEnd);
	if (chromCompNumber!=0) {
		chrtr.constructTP(m_chromComps, m_compBegin, m_compEnd, length);
		chrtr.pruneTP(length);
        chrtr.clearTrees();///??
	}
}
void Component::createTotalTree(const Permutation& permut)
{
	for (int chn=0; chn<Common::chromNumber; ++chn) {
		createChromTree(permut, chn, m_chromTrees[chn]);
	}
}

int Component::totalLeafNumber()
{
	m_totalTreeNumber=0;
	m_totalLeafNumber=0;
	m_chromHasLeaf=0;

	for (int chn=0; chn<Common::chromNumber; ++chn) {
        if (0!=m_chromTrees[chn].getTreeNumber()){
            m_chromTrees[chn].countChromLeafNumber();
                ++m_chromHasLeaf;
                m_totalTreeNumber+=m_chromTrees[chn].getTreeNumber();
                m_totalLeafNumber+=m_chromTrees[chn].getChromLeafNumber();
        }

	}//for
    	return m_totalLeafNumber+countf(m_totalLeafNumber,m_totalTreeNumber);
}

inline
int Component::countf(int leafnum, int treenum)const
{
	int f=0;
	if (leafnum==0)
		f=0;
	else if ((treenum==1)&&(leafnum%2==0))
		f=2;
	else if (leafnum%2==1)
		f=1;
	else if (leafnum%2==0)
		f=0;
	return f;
}


void Component::updateChromLeaf(const Permutation& permut, int chn, ChromTree& chrtr, int& leafch, int& treech)
{

	createChromTree(permut, chn, chrtr);
	chrtr.countChromLeafNumber();

	leafch=chrtr.getChromLeafNumber()-m_chromTrees[chn].getChromLeafNumber();//leafchange
	treech=chrtr.getTreeNumber()-m_chromTrees[chn].getTreeNumber();//treechange

}
int Component::updatef(int leafch, int treech)
{
    int oldf=countf(m_totalLeafNumber,m_totalTreeNumber);
    return countf(m_totalLeafNumber+leafch,m_totalTreeNumber+treech)-oldf;
}

void Component::getCaseValue(int& newLeaf, int& newf)const
{

	newLeaf=0;
	newf=0;
//  if (m_totalLeafNumber==0) {
//  }
//  else
    if ((m_totalLeafNumber%2==0)&&(m_totalTreeNumber!=1)) {
        if ((m_chromHasLeaf==1)||(m_chromHasLeaf==2))// ¦¤L==0
            newf=1;//count ¦¤f
    }
    else
    if (m_totalLeafNumber%2==1) {//f==1
        newLeaf=1;//0,1
        if ((m_chromHasLeaf==1)||(m_chromHasLeaf==2))//&& (¦¤L==1)
            newf=1;//count ¦¤f
    }
    else
    if ((m_totalLeafNumber%2==0)&&(m_totalTreeNumber==1)) {//f==2
        newLeaf=2;//0,1,2
    }

}


ostream& operator<<(ostream& os,const Component& compon)
{
    os<<"components : "<<"\n";
    os<<"  total Leaf Number : "<<compon.m_totalLeafNumber<<"\n";
	os<<"  total Tree Number : "<<compon.m_totalTreeNumber<<"\n";
	os<<"  chromosome Has Leaf : "<<compon.m_chromHasLeaf<<"\n";

	for (int chi=0; chi<Common::chromNumber; ++chi) {
		os<<"chromosome "<<chi<<" : "<<"\n";
        os<<compon.m_chromTrees[chi];
	}

	os<<"\n";
	return os;
}

}//end of namespace
