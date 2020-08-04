#include <cassert>
#include "gchromtree.hpp"
#include "gcomponent.hpp"
#include "transparam.hpp"
#include "gpermutation.hpp"
#include "chromrange.hpp"
#include "pathcycle.hpp"

namespace GeneralTranslocation
{
using namespace Translocation;
int getNonLeaf(const GeneralPermutation& permut, const ChromRange& chromrg1, const GeneralComponent& comps, vector<TransParam>& trsprms)
{
    int transNumber=0;
    ChromRange chromrg2;
    chromrg2.startIndex=0;
    chromrg2.endIndex=0;

    for (int chj=0; chj<Common::chromNumber; ++chj) {
        if(chj==chromrg1.chromIndex)
            continue;
        int length=permut.getLength(chj);
        chromrg2.chromIndex=chj;
        if(comps.m_chromTrees[chj].chromLeafNumber==0){
            chromrg2.startIndex=0;
            chromrg2.endIndex=length-1;
            transNumber+=getBlackPair(chromrg1, chromrg2, trsprms);
#ifdef _DEBUG_OUT
  //std::cout<<"?cut=?"<<chromrg2<<"\n";
#endif

        } else {
            list<GeneralChromTree::ChromTreeNode*>::const_iterator cctitor2=comps.m_chromTrees[chj].trees.begin();
            for ( ;cctitor2!=comps.m_chromTrees[chj].trees.end(); ++cctitor2) {
                //vector<GeneralInterval >::const_iterator cpitor2=(*cctitor2)->leafs.begin();
                // ;cpitor2!=(*cctitor2)->leafs.end(); ++cpitor2
                int leafnum=(*cctitor2)->leafNumber;
                chromrg2.endIndex=(*cctitor2)->leafs[0].begin;
                if(chromrg2.endIndex>0){
                    chromrg2.startIndex=0;
                    transNumber+=getBlackPair(chromrg1, chromrg2, trsprms);
#ifdef _DEBUG_OUT
  //std::cout<<"?cut=? ?"<<chromrg2<<"\n";
#endif
                }
                for (int j=0; j<leafnum-1; ++j) {
                    chromrg2.startIndex=(*cctitor2)->leafs[j].end;//(*cpitor2)
                    chromrg2.endIndex=(*cctitor2)->leafs[j+1].begin;
                    if(chromrg2.startIndex<chromrg2.endIndex){
                        transNumber+=getBlackPair(chromrg1, chromrg2, trsprms);
                    }else
                        continue;
#ifdef _DEBUG_OUT
  //std::cout<<"?cut=? ?"<<chromrg2<<"\n";
#endif
                }//j
                chromrg2.startIndex=(*cctitor2)->leafs[leafnum-1].end;
                if(chromrg2.startIndex<length-1){
                    chromrg2.endIndex=length-1;
                    transNumber+=getBlackPair(chromrg1, chromrg2, trsprms);
#ifdef _DEBUG_OUT
  //std::cout<<"?cut=? ?"<<chromrg2<<"\n";
#endif
                }
            }//cctitor2
        }//else
    }//chj
    return transNumber;
}

int getAllCutTransParam(const GeneralPermutation& permut, const GeneralComponent& comps, vector<TransParam>& trsprms)
{

	int transNumber=0;//, ,-
	ChromRange chromrg1;
	list<GeneralChromTree::ChromTreeNode*>::const_iterator cctitor1,cctitor1e;
	vector<GeneralInterval >::const_iterator cpitor1;
	if (comps.m_totalLeafNumber==0)
		return 0;
	if ((comps.m_totalLeafNumber%2==0)&&(comps.m_totalTreeNumber!=1))
		return 0;
	if (comps.m_totalLeafNumber%2==1) {
		if (comps.m_totalTreeNumber==2) {//
			//if(T==2&&L(T1)==1&&L(T2)==even)

			//any one leaf in T2 do cut leaf;
			int chromone=-1;//not use
			int chromeven=-1;
			for (int chi=0; chi<Common::chromNumber; ++chi) {
                if (comps.m_chromTrees[chi].chromLeafNumber==0)continue;
                else if(comps.m_chromTrees[chi].chromLeafNumber==1)chromone=chi;
                else if(comps.m_chromTrees[chi].chromLeafNumber>1)chromeven=chi;

			}//
			if((chromone>=0)&&(chromeven>=0)){//NL==2
                chromrg1.chromIndex=chromeven;
                //assert(comps.m_chromTrees[chromeven].treeNumber==1);
                cctitor1=comps.m_chromTrees[chromeven].trees.begin();

                    cpitor1=(*cctitor1)->leafs.begin();
                    for ( ;cpitor1!=(*cctitor1)->leafs.end(); ++cpitor1) {

                        chromrg1.startIndex=(*cpitor1).begin;
                        chromrg1.endIndex=(*cpitor1).end;
                        transNumber+=getNonLeaf(permut, chromrg1, comps, trsprms);
#ifdef _DEBUG_OUT
  //std::cout<<"?cut=1e? ?"<<chromrg1<<"\n";
#endif
                    }//cpitor1;

                return transNumber;
			}
			else
			if((chromone==-1)&&(chromeven>=0)){//NL==1
                chromrg1.chromIndex=chromeven;
                cctitor1=comps.m_chromTrees[chromeven].trees.begin();
                for ( ;cctitor1!=comps.m_chromTrees[chromeven].trees.end(); ++cctitor1) {
                    if((*cctitor1)->leafNumber==1)
                        chromone=chromeven;
                    else
                        cctitor1e=cctitor1;
                }
                if(chromone>=0){

                    cpitor1=(*cctitor1e)->leafs.begin();
                    for ( ;cpitor1!=(*cctitor1e)->leafs.end(); ++cpitor1) {

                        chromrg1.startIndex=(*cpitor1).begin;
                        chromrg1.endIndex=(*cpitor1).end;
                        transNumber+=getNonLeaf(permut, chromrg1, comps, trsprms);
#ifdef _DEBUG_OUT
  //std::cout<<"?cut=1e? ?"<<chromrg1<<"\n";
#endif
                    }//cpitor1;

                    return transNumber;
                }

			}
		} //if(L(T1)=1&&L(T2)=even)
		else
		if ((comps.m_totalTreeNumber==1)&&(comps.m_totalLeafNumber>1)) {
		    //if(T==1&&L(T1)==odd)
		    //any one leaf internal T1 do cut leaf;

			int chromodd=-1;
			for (int chi=0; chi<Common::chromNumber; ++chi) {
                if (comps.m_chromTrees[chi].chromLeafNumber==0)continue;
                else if(comps.m_chromTrees[chi].chromLeafNumber>1)chromodd=chi;

			}
			if(chromodd>=0){
                cctitor1=comps.m_chromTrees[chromodd].trees.begin();
                chromrg1.chromIndex=chromodd;
                for ( ;cctitor1!=comps.m_chromTrees[chromodd].trees.end(); ++cctitor1) {
                    cpitor1=(*cctitor1)->leafs.begin();
                    ++cpitor1;
                    for (int lj=0 ;lj<comps.m_chromTrees[chromodd].chromLeafNumber-2; ++lj) {
                    //for ( ;cpitor1!=(*cctitor1)->leafs.end(); ++cpitor1) {
                        chromrg1.startIndex=(*cpitor1).begin;
                        chromrg1.endIndex=(*cpitor1).end;

                        transNumber+=getNonLeaf(permut, chromrg1, comps, trsprms);
                        ++cpitor1;
#ifdef _DEBUG_OUT
  //std::cout<<"?cut=o? ?"<<chromrg1<<"\n";
#endif
                    }//cpitor1;
                }//cctitor1

                return transNumber;
			}//if any one leaf internal T1 do cut leaf;

		}//if(T==1&&L(T1)==odd)
	}
	for (int chi=0; chi<Common::chromNumber; ++chi) {
		if (comps.m_chromTrees[chi].chromLeafNumber==0)
			continue;
		cctitor1=comps.m_chromTrees[chi].trees.begin();
		chromrg1.chromIndex=chi;
		for ( ;cctitor1!=comps.m_chromTrees[chi].trees.end(); ++cctitor1) {
			cpitor1=(*cctitor1)->leafs.begin();
			for ( ;cpitor1!=(*cctitor1)->leafs.end(); ++cpitor1) {
                chromrg1.startIndex=(*cpitor1).begin;
                chromrg1.endIndex=(*cpitor1).end;

				transNumber+=getNonLeaf(permut, chromrg1, comps, trsprms);
#ifdef _DEBUG_OUT
  //std::cout<<"?cut=? ?"<<chromrg1<<"\n";
#endif
			}//cpitor1;
		}//cctitor1
	}//chi
	//===============================//
	if ((comps.m_totalLeafNumber%2==0)&&(comps.m_totalTreeNumber==1)){
        /// find black edge between leafs in even-isolation.

        int chi=0;
        for (; chi<Common::chromNumber; ++chi) {
            if (comps.m_chromTrees[chi].chromLeafNumber==0)
                continue;
            else
                break;
        }//chi
        cctitor1=comps.m_chromTrees[chi].trees.begin();
        chromrg1.chromIndex=chi;
		chromrg1.startIndex=(*cctitor1)->gintvl.begin;

        cpitor1=(*cctitor1)->leafs.begin();
        chromrg1.endIndex=(*cpitor1).begin;
#ifdef _DEBUG_OUT
 //std::cout<<"?cut=?left?"<<chromrg1<<"\n";
#endif
        if(chromrg1.startIndex!=chromrg1.endIndex)
            transNumber+=getNonLeaf(permut, chromrg1, comps, trsprms);

        chromrg1.startIndex=(*cpitor1).end;
        while(true) {
            ++cpitor1;
            if(cpitor1==(*cctitor1)->leafs.end())
                break;
            chromrg1.endIndex=(*cpitor1).begin;
            if(chromrg1.startIndex!=chromrg1.endIndex)
                transNumber+=getNonLeaf(permut, chromrg1, comps, trsprms);
#ifdef _DEBUG_OUT
//std::cout<<"?cut=?adjacency?"<<chromrg1<<"\n";
#endif
            chromrg1.startIndex=(*cpitor1).end;
        }//while;
        --cpitor1;
        chromrg1.startIndex=(*cpitor1).end;
		chromrg1.endIndex=(*cctitor1)->gintvl.end;
#ifdef _DEBUG_OUT
 //std::cout<<"?cut=?right?"<<chromrg1<<"\n";
#endif
        if(chromrg1.startIndex!=chromrg1.endIndex)
            transNumber+=getNonLeaf(permut, chromrg1, comps, trsprms);

    }//if
    return transNumber;
}
}//end of namespace
