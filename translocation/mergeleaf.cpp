#include "chromtree.hpp"
#include "component.hpp"
#include "transparam.hpp"
#include "permutation.hpp"
#include "chromrange.hpp"
#include "geneposition.hpp"
namespace Translocation
{


int getAllMergeTransParam(const Permutation& permut, const Component& comps, vector<TransParam>& trsprms)
{

	if ((comps.m_totalTreeNumber<2)||(comps.m_chromHasLeaf==1))
		return 0;
	int transNumber=0;
	ChromRange chromrg1,chromrg2;
	list<ChromTree::ChromTreeNode*>::const_iterator cctitor1,cctitor2,cctitor11e;
	vector<Interval>::const_iterator cpitor1,cpitor2;
	//int i,j;
	int chi, chj;
	if (comps.m_totalLeafNumber%2==0) {
		if ((comps.m_totalTreeNumber==2)&&(comps.m_totalLeafNumber>2)) {//&&(comps.m_chromHasLeaf==2)
			//if(L(T1)=1)&&(L(T2)=odd)
			// one leaf in T1 and any one leaf internal T2 do merge leaf

			int chromone=-1;
			int chromodd=-1;
			for (chi=0; chi<Common::chromNumber; ++chi) {
                if (comps.m_chromTrees[chi].chromLeafNumber==0)continue;
                else if(comps.m_chromTrees[chi].chromLeafNumber==1)chromone=chi;
                else if(comps.m_chromTrees[chi].chromLeafNumber>1)chromodd=chi;

			}
			if((chromone>=0)&&(chromodd>=0)){
                cctitor1=comps.m_chromTrees[chromone].trees.begin();
                cpitor1=(*cctitor1)->leafs.begin();
                chromrg1.chromIndex=chromone;
                chromrg1.startIndex=(*cpitor1).begin;
                chromrg1.endIndex=(*cpitor1).end;
#ifdef _DEBUG_OUT
  //std::cout<<"?merge=1o? ?"<<chromrg1<<"\n";
#endif
                cctitor2=comps.m_chromTrees[chromodd].trees.begin();
                cpitor2=(*cctitor2)->leafs.begin();
                ++cpitor2;
                //cpitor2!=--(*cctitor2)->leafs.end()
                chromrg2.chromIndex=chromodd;
                for (int lj=0 ;lj<comps.m_chromTrees[chromodd].chromLeafNumber-2; ++lj) {

                    chromrg2.startIndex=(*cpitor2).begin;
                    chromrg2.endIndex=(*cpitor2).end;

                    transNumber+=getBlackPair(chromrg1, chromrg2, trsprms);
                        ++cpitor2;
                }//lj
                return transNumber;
			}//if (L(T1)=1)&&(L(T2)=odd)
		}
		else
		if ((comps.m_totalTreeNumber==3)&&(comps.m_totalLeafNumber>3)) {//&&(comps.m_chromHasLeaf==3)
		    int chromone1=-1;
		    int chromone2=-1;
		    int chromeven=-1;
			//if(L(T1)=1&&L(T2)=1&&L(T3)=even)
			//one leaf in T1 and any one leaf in T3 do merge leaf
			//one leaf in T2 and any one leaf in T3 do merge leaf
			for (chi=0; chi<Common::chromNumber; ++chi) {
                if (comps.m_chromTrees[chi].chromLeafNumber==0)
                    continue;
                else if(comps.m_chromTrees[chi].chromLeafNumber==1){
                    if(chromone1<0)chromone1=chi;
                    else if(chromone2<0)chromone2=chi;
                }
                else if(comps.m_chromTrees[chi].chromLeafNumber>1)
                    chromeven=chi;

			}
			if((chromone1>=0)&&(chromone2>=0)&&(chromeven>=0)){//NL==3

                cctitor1=comps.m_chromTrees[chromone1].trees.begin();
                cpitor1=(*cctitor1)->leafs.begin();
                chromrg1.chromIndex=chromone1;
                chromrg1.startIndex=(*cpitor1).begin;
                chromrg1.endIndex=(*cpitor1).end;

#ifdef _DEBUG_OUT
   std::cout<<"?merge=11e? ?"<<chromrg1<<"\n";
#endif
                cctitor2=comps.m_chromTrees[chromeven].trees.begin();
                cpitor2=(*cctitor2)->leafs.begin();
                chromrg2.chromIndex=chromeven;
                for ( ;cpitor2!=(*cctitor2)->leafs.end(); ++cpitor2)  {

                    chromrg2.startIndex=(*cpitor2).begin;
                    chromrg2.endIndex=(*cpitor2).end;

                    transNumber+=getBlackPair(chromrg1, chromrg2, trsprms);

                }//cpitor2

                //===============================//
                cctitor1=comps.m_chromTrees[chromone2].trees.begin();
                cpitor1=(*cctitor1)->leafs.begin();
                chromrg1.chromIndex=chromone2;
                chromrg1.startIndex=(*cpitor1).begin;
                chromrg1.endIndex=(*cpitor1).end;

#ifdef _DEBUG_OUT
  std::cout<<"?merge=11e? ?"<<chromrg1<<"\n";
#endif
                cctitor2=comps.m_chromTrees[chromeven].trees.begin();
                chromrg2.chromIndex=chromeven;
                cpitor2=(*cctitor2)->leafs.begin();
                for ( ;cpitor2!=(*cctitor2)->leafs.end(); ++cpitor2)  {

                    chromrg2.startIndex=(*cpitor2).begin;
                    chromrg2.endIndex=(*cpitor2).end;

                    transNumber+=getBlackPair(chromrg1, chromrg2, trsprms);

                }//cpitor2
                return transNumber;
			}//if NL==3
			else
			if((chromone1>=0)&&(chromone2==-1)&&(chromeven>=0)){//NL==2

                cctitor2=comps.m_chromTrees[chromeven].trees.begin();
                for ( ;cctitor2!=comps.m_chromTrees[chromeven].trees.end(); ++cctitor2) {
                    if((*cctitor2)->leafNumber==1)
                        chromone2=chromeven;
                    else
                        cctitor11e=cctitor2;
                }
                if(chromone2>=0){
                    cctitor1=comps.m_chromTrees[chromone1].trees.begin();
                    cpitor1=(*cctitor1)->leafs.begin();
                    chromrg1.chromIndex=chromone1;
                    chromrg1.startIndex=(*cpitor1).begin;
                    chromrg1.endIndex=(*cpitor1).end;

#ifdef _DEBUG_OUT
    std::cout<<"?merge=11e? ?"<<chromrg1<<"\n";
#endif

                    chromrg2.chromIndex=chromeven;
                    cpitor2=(*cctitor11e)->leafs.begin();
                    for ( ;cpitor2!=(*cctitor11e)->leafs.end(); ++cpitor2)  {

                        chromrg2.startIndex=(*cpitor2).begin;
                        chromrg2.endIndex=(*cpitor2).end;

                        transNumber+=getBlackPair(chromrg1, chromrg2, trsprms);

                    }//cpitor2

                    return transNumber;
                }
			}//else if NL==2

		}//else if (L(T1)=1&&L(T2)=1&&L(T3)=even)
	}//if ((comps.m_totalLeafNumber%2==0))

	for (chi=0; chi<Common::chromNumber; ++chi) {
		if (comps.m_chromTrees[chi].chromLeafNumber==0)
			continue;
        chromrg1.chromIndex=chi;
		cctitor1=comps.m_chromTrees[chi].trees.begin();
		for ( ;cctitor1!=comps.m_chromTrees[chi].trees.end(); ++cctitor1) {
			cpitor1=(*cctitor1)->leafs.begin();
			for ( ;cpitor1!=(*cctitor1)->leafs.end(); ++cpitor1) {
                chromrg1.startIndex=(*cpitor1).begin;
                chromrg1.endIndex=(*cpitor1).end;

#ifdef _DEBUG_OUT
  std::cout<<"?merge=? ?"<<chromrg1<<"\n";
#endif
				for (chj=chi+1; chj<Common::chromNumber; ++chj) {
					if (comps.m_chromTrees[chj].chromLeafNumber==0)
						continue;
					chromrg2.chromIndex=chj;
					cctitor2=comps.m_chromTrees[chj].trees.begin();
					for ( ;cctitor2!=comps.m_chromTrees[chj].trees.end(); ++cctitor2) {
						cpitor2=(*cctitor2)->leafs.begin();
						for ( ;cpitor2!=(*cctitor2)->leafs.end(); ++cpitor2) {

                            chromrg2.startIndex=(*cpitor2).begin;
                            chromrg2.endIndex=(*cpitor2).end;

                            transNumber+=getBlackPair(chromrg1, chromrg2, trsprms);
						}//cpitor2
					}//cctitor2
				}//chj
			}//cpitor;
		}//cctitor
	}//chi
    return transNumber;
}


}//end of namespace
