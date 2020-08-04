#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include "transparam.hpp"
#include "hcycle.hpp"
#include "permutation.hpp"
#include "geneposition.hpp"



namespace Translocation
{


int HCycle::getCycles(const Permutation& permut,const GenePosition& genePosit, vector<HTransParam>& trsprms, bool fill)//
{
//    if(fill){
//        m_cycle.clear();
//    }
	int cycleNumber=0;

	const int n=Common::geneNumber;
	int* visited=new int[n];//0..n-1 , point;
	int i=0;
	for (i=0; i<n; ++i) {
		visited[i]=0;
	}


	int chbegin=0;
	int genebegin=0;
	int chi,genei,fpi;
	int chj,genej,fpj,nextj;


	Position posit;
	vector<pair<Position,bool> > extCycle;

	for (i=0; i<n; ++i) { //0..n
		if (2==visited[i])
			continue;
		if (i>=n-1)//??
			break;

		genebegin=i-genePosit.getBeginIndex(chbegin);
		if (1==visited[i]){
			if (genebegin==permut.getLength(chbegin)-1) {
				++chbegin;
				continue;
			}
			else
			if (genebegin==0) {
				continue;
			}
		}
		++visited[i];//=1
		int adj=1;
#ifdef _DEBUG_OUT
  std::cout<<"?cycles?"<<chbegin<<"."<<genebegin<<"."<<adj<<":";
#endif

        //int gene1,gene2;
        int m=0;
		chi=chbegin;
		genei=genebegin;
		bool extgray=false;
		bool direct=true;
		//int cycleExtGrayNum=0;
		while (true) {

			fpi=permut.getGene(chi,genei);
        if(fill){
			posit.chromIndex=chi;
			posit.geneIndex=genei;
			extCycle.push_back(std::make_pair(posit,direct));///
        }
			if (fpi>0) {
				if (adj==1) {//right
					chj=genePosit.getPosition(fpi+1).chromIndex;//k->k+1
					genej=genePosit.getPosition(fpi+1).geneIndex;
					fpj=permut.getGene(chj,genej);

					++visited[genePosit.getBeginIndex(chj)+genej];
                    //gene1=genei;
					if (fpj>0) {
						nextj=genej-1;
						direct=false;
						adj=1;
						//gene2=genej-1;

						//trprm.transStyle=PrefixPrefix;
					} else if (fpj<0) {
						nextj=genej+1;
						direct=true;
						adj=0;
						//gene2=genej;
						//trprm.transStyle=PrefixSuffix;
					}
				} else if (adj==0) {//left
					chj=genePosit.getPosition(fpi-1).chromIndex;//k->k-1
					genej=genePosit.getPosition(fpi-1).geneIndex;
					fpj=permut.getGene(chj,genej);
					//gene1=genei-1;

					++visited[genePosit.getBeginIndex(chj)+genej];
					if (fpj>0) {
						nextj=genej+1;
						direct=true;
						adj=0;
						//gene2=genej;
						//trprm.transStyle=PrefixPrefix;
					} else if (fpj<0) {
						nextj=genej-1;
						direct=false;
						adj=1;
						//gene2=genej-1;
						//trprm.transStyle=PrefixSuffix;
					}
				}

			} else if (fpi<0) {
				if (adj==1) {//right
					chj=genePosit.getPosition(-fpi-1).chromIndex;//k->k-1
					genej=genePosit.getPosition(-fpi-1).geneIndex;
					fpj=permut.getGene(chj,genej);
                    //gene1=genei;
					++visited[genePosit.getBeginIndex(chj)+genej];

					if (fpj>0) {
						nextj=genej+1;
						direct=true;
						adj=0;
						//gene2=genej;
                        //trprm.transStyle=PrefixSuffix;
					} else if (fpj<0) {
						nextj=genej-1;
						direct=false;
						adj=1;
						//gene2=genej-1;
						//trprm.transStyle=PrefixPrefix;
					}
				} else if (adj==0) {//left
					chj=genePosit.getPosition(-fpi+1).chromIndex;//k->k+1
					genej=genePosit.getPosition(-fpi+1).geneIndex;
					fpj=permut.getGene(chj,genej);
                    //gene1=genei-1;
					++visited[genePosit.getBeginIndex(chj)+genej];

					if (fpj>0) {
						nextj=genej-1;
						direct=false;
						adj=1;
						//gene2=genej-1;
                        //trprm.transStyle=PrefixSuffix;
					} else if (fpj<0) {
						nextj=genej+1;
						direct=true;
						adj=0;
						//gene2=genej;
						//trprm.transStyle=PrefixPrefix;
					}
				}

			}
#ifdef _DEBUG_OUT
 std::cout<<"~"<<chj<<"."<<nextj<<"."<<adj;
#endif
            ++visited[genePosit.getBeginIndex(chj)+nextj];

        if(fill){
			posit.chromIndex=chj;
			posit.geneIndex=genej;

			extCycle.push_back(std::make_pair(posit,direct));///

            if(chi!=chj){
#ifdef _DEBUG_OUT
  std::cout<<"?ext gray?"<<fpi<<"~"<<fpj<<"\n";
#endif
                extgray=true;

            }//if !=
        }
            if ((chbegin==chj)&&(nextj==genebegin)&&(adj==1))
				break;
            if((chj==0)&&(nextj==0)&&(adj==1))
                break;
        ++m;
        if( m>n ){
            std::cout<<permut;
            throw(std::runtime_error("cycles"));
        }


            chi=chj;
            genei=nextj;//??
		}// while(true)
        ++cycleNumber;
#ifdef _DEBUG_OUT
  std::cout<<"?cycle?"<<cycleNumber<<";"<<"\n";
#endif
        if(fill){
            direct=false;

            posit.chromIndex=chbegin;
            posit.geneIndex=genebegin;
            extCycle.push_back(std::make_pair(posit,direct));///0
            if(extgray){
                getProperTransParam(extCycle,trsprms,cycleNumber);
            }
            extCycle.clear();
        }
	}//for
	delete [] visited;
    return cycleNumber;
}


inline
void HCycle::getProperTransParam(vector<pair<Position,bool> >& extcycle,vector<HTransParam>& trsprms, int cycleNumber)
{
    HTransParam trprm;
    int chi1,chi2,gni1,gni2;
	int gri1,gri2;
	//list<vector<pair<Position,bool> > >::const_iterator extCycitor = m_cycle.begin();
	//for (; extCycitor != m_cycle.end(); ++extCycitor) {
		vector<pair<Position,bool> >::const_iterator vitor1 = extcycle.begin();
		
		for (++vitor1,gri1=0; vitor1 <= extcycle.end()-4; ++vitor1,++gri1){//-6
			chi1=(*vitor1).first.chromIndex;
            gni1=(*vitor1).first.geneIndex;
			if(gni1>(*(++vitor1)).first.geneIndex)
                gni1=(*vitor1).first.geneIndex;
			//++gri1;
            assert(chi1==(*vitor1).first.chromIndex);

            vector<pair<Position,bool> >::const_iterator vitor2=vitor1+1;//+3
			gri2=gri1+1;
            for (; vitor2 <= extcycle.end()-2; ++vitor2,++gri2){//-2


                if((*vitor2).first.chromIndex!=(*vitor1).first.chromIndex){//(*vitor1)
                    chi2=(*vitor2).first.chromIndex;
                    gni2=(*vitor2).first.geneIndex;
                    if(gni2>(*(++vitor2)).first.geneIndex)
                        gni2=(*vitor2).first.geneIndex;
					//++gri2;
                    assert(chi2==(*vitor2).first.chromIndex);

                    if(chi1<chi2){

                        trprm.chromIndex1=chi1;
                        trprm.chromIndex2=chi2;

                        trprm.geneIndex1=gni1;
                        trprm.geneIndex2=gni2;
						trprm.cycnum1=cycleNumber;
						trprm.cycnum2=cycleNumber;
						trprm.grayIndex1= vitor1-extcycle.begin();//gri1;
						trprm.grayIndex2= vitor2-extcycle.begin();//gri2;
                    }else {//if(chi2<chi1)
                        trprm.chromIndex1=chi2;
                        trprm.chromIndex2=chi1;

                        trprm.geneIndex1=gni2;
                        trprm.geneIndex2=gni1;
						trprm.cycnum1=cycleNumber;
						trprm.cycnum2=cycleNumber;
						trprm.grayIndex1=vitor2-extcycle.begin();//gri2;
						trprm.grayIndex2=vitor1-extcycle.begin();//gri1;
                    }
                    assert(trprm.chromIndex2!=trprm.chromIndex1);
                    if((*vitor2).second==(*vitor1).second)
                        trprm.transStyle=PrefixPrefix;
                    
                    else
                        trprm.transStyle=PrefixSuffix;
                    trsprms.push_back(trprm);
#ifdef _DEBUG_OUT
  std::cout<<"?get proper?"<<trprm<<";"<<"\n";
#endif
                }//if !=
            }//for vitor2

		}//for vitor1
	//}//for extCycitor
}

}//namespace
