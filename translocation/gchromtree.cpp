#include <cassert>
#include <stack>
#include <stdexcept>
#include "gchromtree.hpp"
//#include "permutation.hpp"
#include "transparam.hpp"
#include "gpermutation.hpp"
#include "geneposition.hpp"

using std::stack;
using std::make_pair;

namespace GeneralTranslocation
{

	GeneralChromTree::GeneralChromTree()
		:treeNumber(0),chromLeafNumber(0),semiTreeNumber(0),chromSemiLeafNumber(0),realSemiTreeNumber(0),semiRealTreeNumber(0)

	{
	}
	GeneralChromTree::~GeneralChromTree()
	{
		deleteTP();
	}

	void GeneralChromTree::ChromTreeNode::deleteTree()
	{
		TreeNode* q=root;
		TreeNode* p=q;

		while (q!=NULL) {
			while (!q->child.empty())
				q=q->child.front();  //begin()
			p=q->parent;
			//q->child.clear();
			//std::cout<<"?delete tree?"<<q->ginterval.begin<<"~"<<q->ginterval.end<<"??"<<"\n";
			delete q;

			if (p!=NULL){
				p->child.pop_front();
			}
			q=p;
		}
		root=NULL;
	}

	void GeneralChromTree::deleteTP()
	{
		chromLeafNumber=0;
		treeNumber=0;
		chromSemiLeafNumber=0;
		semiTreeNumber=0;
		while (!trees.empty()) {
			ChromTreeNode* ctnode=trees.front();
			//ctnode->leafNumber=0;
			//ctnode->semiLeafNumber=0;

			ctnode->leafs.clear();
			ctnode->semiLeafs.clear();///
			if(ctnode->root!=NULL)
				ctnode->deleteTree();//

			delete ctnode;
			trees.pop_front();//
		}

	}
	void GeneralChromTree::clearTrees()
	{
		list<ChromTreeNode*>::iterator ritor=trees.begin();
		for ( ;ritor!=trees.end(); ++ritor) {
			(*ritor)->deleteTree();
		}
	}

	void GeneralChromTree::constructTP(const vector<GeneralInterval>& comps, const short int* start, const short int* end, const int length)
	{

		treeNumber=0;
		semiTreeNumber=0;


		TreeNode* q=NULL;
		TreeNode* p=NULL;
		TreeNode* qn;
		TreeNode* pn;

		int compIndex=0;
		//int ei=0;
		ChromTreeNode* chtrnode;
		for (int i=0; i<length; ++i) {
			//there is a component C starting at position i
			if (start[i]!=0) {

				//there is no component ending at position i
				if (end[i]==0) {
					//create a new square node q as a child of p
					qn=new TreeNode();

					qn->style=Square;
					qn->ginterval.begin=0;//
					qn->ginterval.end=0;//
					//qn->ginterval.type=??
					qn->parent=p;
					if (p==NULL) {
						chtrnode=new ChromTreeNode();
						chtrnode->root=qn;
						chtrnode->leafNumber=0;
						//chtrnode->semiNode=0;///
						chtrnode->semiLeafNumber=0;///
						trees.push_back(chtrnode);
						++treeNumber;
					} else {
						p->child.push_back(qn); //round node child number>=1!!
					}
					q=qn;
				}
				//create a new round node p (representing C) as a child of q
				compIndex=start[i]-1;//
				pn=new TreeNode();
				pn->parent=q;
				pn->style=Round;
				pn->ginterval=comps[compIndex];
				//if (comps[compIndex].type==AB){ /// ?????
				//	++(chtrnode->semiNode);
				//	--treeNumber;
				//	++semiTreeNumber;
				//}
				q->child.push_back(pn);
				++q->ginterval.begin;//square node child number

				p=pn;
#ifdef _DEBUG_OUT
				std::cout<<"?construct tp?"<<compIndex<<" : "<<p->ginterval.begin<<"~"<<p->ginterval.end<<"??"<<"\n";
#endif
				//there is a component ending at position i
			} else if (end[i]!=0) {
				p=q->parent;
				if (p==NULL) {
					q=NULL;
					continue;
				} else {
					q=p->parent;
				}
			}
		}//for
#ifdef _DEBUG_OUT
				std::cout<<"?construct tp?"<<treeNumber<<" , "<<semiTreeNumber<<"\n";
#endif
	}

	void GeneralChromTree::splitTP(const int length)//??????????????
	{
		vector<list<TreeNode *>::iterator> itvt;
		//vector<int> itvt;
		list<ChromTreeNode*>::iterator ritor=trees.begin();
		for ( ;ritor!=trees.end(); ) {//++ritor
			bool bstart=false;
			TreeNode* q=(*ritor)->root;
			TreeNode* pq=q;
			list<TreeNode *>::iterator itor=q->child.begin();
			//itvt.push_back(itor);
			int level=0;
			GeneralInterval itvl;
			while (pq!=NULL) {

				while (!q->child.empty()) {
					++level;
					if (itvt.size()<level) {
						itor=q->child.begin();//first time
						itvt.push_back(itor);///0
					} else {
						itor=itvt[level-1];//??
						if (itor==q->child.end()){
							itvt.pop_back();
							--level;
							q=q->parent;//??
							
							if (q==NULL)
								break;//back to trees root
							else{
								++itvt[level-1];//next node
								--level;
								continue;
							}
						}
					}
					q=*itor;
				}//while  q

				if (q==NULL)
					break; //trees root -> next trees
#ifdef _DEBUG_OUT
				std::cout<<"?split tp?level?"<<level<<"\n";
#endif
				assert(q->style==Round);
				pq=q->parent;
				if (q->ginterval.type==AA || q->ginterval.type==BB){
					
					//delete current node and all parent , split tree
					list<ChromTreeNode*> treelist;///??
					while(q!=NULL){//
						
						assert(q->style==Round);
						delete q;
						itor=itvt[level-1];
						list<TreeNode *>::iterator tmpitor=pq->child.begin();
						if(itor!=pq->child.begin()){
							TreeNode* leftnode=new TreeNode();
							leftnode->style=Square;
							leftnode->parent=NULL;
							leftnode->child.assign(pq->child.begin(),itor);
							leftnode->ginterval.begin=leftnode->child.size();
							
							for(;tmpitor!=itor;++tmpitor){//?
								(*tmpitor)->parent=leftnode;
							}
							ChromTreeNode* leftchtrnode=new ChromTreeNode();
							leftchtrnode->root=leftnode;
							leftchtrnode->leafNumber=0;///??
							//leftchtrnode->semiNode=0;///??
							leftchtrnode->semiLeafNumber=0;///??
							treelist.push_front(leftchtrnode);
							++treeNumber;//??
						}
						++itor;
						if(itor!=pq->child.end()){
							TreeNode* rightnode=new TreeNode();
							rightnode->style=Square;
							rightnode->parent=NULL;
							
							rightnode->child.assign(itor,pq->child.end());
							rightnode->ginterval.begin=rightnode->child.size();
							tmpitor=itor;
							for(;tmpitor!=pq->child.end();++tmpitor){
								(*tmpitor)->parent=rightnode;
							}

							ChromTreeNode* rightchtrnode=new ChromTreeNode();
							rightchtrnode->root=rightnode;
							rightchtrnode->leafNumber=0;///??
							//rightchtrnode->semiNode=0;///??
							rightchtrnode->semiLeafNumber=0;///??
							treelist.push_back(rightchtrnode);
							++treeNumber;//??
						}
						pq->child.clear();
						q=pq->parent;
						delete pq;
						//itvt[level]=NULL;
						--level;
						itvt.pop_back();

						if(q!=NULL)
						{
							
							assert(q->style==Round);
							itor=itvt[level-1];//??
							
							list<TreeNode *>::iterator tmpitor=q->child.begin();
							for(;tmpitor!=itor;++tmpitor){//?
								(*tmpitor)->parent=NULL;
								ChromTreeNode* leftchtrnode=new ChromTreeNode();
								leftchtrnode->root=(*tmpitor);
								leftchtrnode->leafNumber=0;///??
								//leftchtrnode->semiNode=0;///??
								leftchtrnode->semiLeafNumber=0;///??
								treelist.push_front(leftchtrnode);
								++treeNumber;//??
							}
							++tmpitor;
							for(;tmpitor!=q->child.end();++tmpitor){//?
								(*tmpitor)->parent=NULL;
								ChromTreeNode* rightchtrnode=new ChromTreeNode();
								rightchtrnode->root=(*tmpitor);
								rightchtrnode->leafNumber=0;///??
								//rightchtrnode->semiNode=0;///??
								rightchtrnode->semiLeafNumber=0;///??
								treelist.push_back(rightchtrnode);
								++treeNumber;//??
							}
							q->child.clear();
							--level;
							itvt.pop_back();
							pq=q->parent;
						}

						
					}//while
					
					//if((*ritor)->semiNode>0)
					//	--semiTreeNumber;///
					//else
					--treeNumber;

					list<ChromTreeNode*>::iterator tmpritor=ritor;
					if(ritor!=trees.begin()){
						--tmpritor;//deleted?
					}
					else
						bstart=true;
					delete  *ritor ;	
					ritor=trees.erase(ritor); //??
					trees.insert(ritor,treelist.begin(),treelist.end());
					if(bstart==false)
						ritor=tmpritor;
					else
						ritor=trees.begin();
					break;
				}//if
				else
				{
					++itvt[level-1];//next node
				}
				q=pq;
				--level;
			}//while 
			if(ritor!=trees.end() && bstart==false)
				++ritor;//list iterator not incrementable
		}//for
#ifdef _DEBUG_OUT
				std::cout<<"?split tp?"<<treeNumber<<" , "<<semiTreeNumber<<"\n";
#endif
	}
	void GeneralChromTree::pruneTP(const int length)
	{
		vector<list<TreeNode *>::iterator> itvt;
		//vector<int> itvt;
		list<ChromTreeNode*>::iterator ritor=trees.begin();
		for ( ;ritor!=trees.end(); ) {//++ritor
			(*ritor)->leafNumber=0;
			(*ritor)->semiLeafNumber=0;///
			(*ritor)->semiRealNode=0;
			bool del=false;
			TreeNode* q=(*ritor)->root;
			TreeNode* pq=q;
			list<TreeNode *>::iterator itor=q->child.begin();
			//itvt.push_back(itor);
			int level=0;
			int semiNode=0;
			GeneralInterval itvl;
			while (pq!=NULL) {

				while (!q->child.empty()) {
					++level;
					if (itvt.size()<level) {
						itor=q->child.begin();//first time
						itvt.push_back(itor);///0
					} else {
						itor=itvt[level-1];//??
						if (itor==q->child.end()){
							if (q->style==Round && q->ginterval.type==AB){/// ?????
								++semiNode;//semiRealNode;
							}
							itvt.pop_back();
							--level;
							q=q->parent;//??
							
							if (q==NULL)
								break;//back to trees root
							else{
								++itvt[level-1];//next node
								--level;
								continue;
							}
						}
					}
					q=*itor;
				}//while  q

				if (q==NULL)
					break; //trees root -> next trees
#ifdef _DEBUG_OUT
				std::cout<<"?prune tp?level="<<level<<"?semi node="<<semiNode<<"\n";
#endif
				assert(q->style==Round);
				pq=q->parent;
				if (q->ginterval.type==AB){/// ?????
					++semiNode;
				}
				if (q->ginterval.end - q->ginterval.begin == 1) { //trivial component
					if (q->ginterval.type==AB){/// ?????
						--semiNode;
					}
					delete q;

					itor=pq->child.erase(itor);//
					itvt[level-1]=itor;//next node //!!#define _HAS_ITERATOR_DEBUGGING 0
					--pq->ginterval.begin;
					//assert(pq->style==Square);
					if (pq->child.empty()) {
						q=pq->parent;
						delete pq;

						--level;
						itvt.pop_back();
						if (q==NULL) {  //m_root->ginterval.end==m_root->ginterval.begin
							--treeNumber;
							assert(semiNode==0);///???
							delete  *ritor ;
							ritor=trees.erase(ritor); 
							del=true;
							break;//next tree;
						}

						itor=q->child.erase(itvt[level-1]);///itor
						itvt[level-1]=itor;//next node
						pq=q;
						itvl.begin=q->ginterval.begin;
						itvl.end=q->ginterval.end;
						if(q->child.empty()){
							if( (*ritor)->leafNumber +  (*ritor)->semiLeafNumber > Common::chromNumber )
							{
								std::cout<<itvl.begin<<"-"<<itvl.end;
								throw(std::runtime_error("gprunetp"));
							}
							if (q->ginterval.type==AB){/// ?????
								++((*ritor)->semiLeafNumber);
								(*ritor)->semiLeafs.push_back(itvl);

							}
							else{
								++((*ritor)->leafNumber);///
								(*ritor)->leafs.push_back(itvl);
							}
#ifdef _DEBUG_OUT
							std::cout<<"?prune tp?leafs2?"<<q->ginterval.begin<<"~"<<q->ginterval.end<<"\n";
#endif
							pq=q->parent;
							--level;
							itvt.pop_back();
							++itvt[level-1];//next node
						}//if q
					}//if pq

				} else {
					itvl=q->ginterval;
					if( (*ritor)->leafNumber +  (*ritor)->semiLeafNumber > Common::chromNumber )//
					{
						std::cout<<itvl.begin<<"-"<<itvl.end;
						throw(std::runtime_error("gprunetp"));
					}
					if (q->ginterval.type==AB){/// ?????
						++((*ritor)->semiLeafNumber);
						(*ritor)->semiLeafs.push_back(itvl);

					}
					else{
						++((*ritor)->leafNumber);///
						(*ritor)->leafs.push_back(itvl);
					}
#ifdef _DEBUG_OUT
					std::cout<<"?prune tp?leafs1?"<<q->ginterval.begin<<"~"<<q->ginterval.end<<"\n";
#endif
					++itvt[level-1];//next node
				}
				q=pq;
				--level;
			}//while pq

			if(semiNode>0){
				--treeNumber;
				++semiTreeNumber;///
				if((*ritor)->semiLeafNumber<semiNode){
					(*ritor)->semiRealNode=semiNode-(*ritor)->semiLeafNumber;//has ssp 
				}
				assert((*ritor)->semiLeafNumber<=2);
				assert((*ritor)->semiRealNode<=2);
			}
#ifdef _DEBUG_OUT
				std::cout<<"?prune tp?"<<treeNumber<<" , "<<semiTreeNumber<<"\n";
#endif
			if(ritor!=trees.end()){
				(*ritor)->gintvl.begin=(*ritor)->root->child.front()->ginterval.begin;
				(*ritor)->gintvl.end=(*ritor)->root->child.back()->ginterval.end;
				(*ritor)->gintvl.type=(*ritor)->root->child.front()->ginterval.type;//??
#ifdef _DEBUG_OUT
				std::cout<<"?prune tp?left~right?"<<(*ritor)->gintvl.begin<<"~"<<(*ritor)->gintvl.end<<"\n";
#endif
			}
			if(del==false)
				++ritor;
		}//for  ritor
		itvt.clear();

	}
	void GeneralChromTree::countChromLeafNumber()
	{

		chromLeafNumber=0;
		chromSemiLeafNumber=0;
		realSemiTreeNumber=0;
		semiRealTreeNumber=0;
		if ((0!=treeNumber)||(0!=semiTreeNumber)) {

			list<ChromTreeNode*>::const_iterator ritor=trees.begin();
			for ( ;ritor!=trees.end(); ++ritor) {
				if(0!=treeNumber){
					chromLeafNumber+=(*ritor)->leafNumber;
				}
				if (0!=semiTreeNumber){
					chromSemiLeafNumber+=(*ritor)->semiLeafNumber;///
					if((*ritor)->leafNumber!=0){
						chromLeafNumber+=(*ritor)->leafNumber;
						if((*ritor)->semiRealNode==0)
							++realSemiTreeNumber;
						else
							++semiRealTreeNumber;
					}
				}
			}
		}
		//assert(treeNumber<=chromLeafNumber);
	assert(realSemiTreeNumber<=semiTreeNumber);
	assert(semiRealTreeNumber<=semiTreeNumber);
	}

	EITYPE GeneralChromTree::getEIType()const
	{	
		if(chromLeafNumber==0)
			return Non;	
		if (1==treeNumber && realSemiTreeNumber==0 && semiRealTreeNumber==0) {
			if(chromLeafNumber%2==0)
				return Real;
		}
		else
		if(1==realSemiTreeNumber && semiRealTreeNumber==0 && treeNumber==0 ) {
			if(chromLeafNumber%2==0 && chromSemiLeafNumber==2 && 1==semiTreeNumber )
				return Strong;
			else 
			if(chromLeafNumber%2==0 && chromSemiLeafNumber==1)
				return Real;
			else
			if(chromLeafNumber%2==1 && chromSemiLeafNumber==1)
				return Weak;
		}
		else
		if(1==semiRealTreeNumber && realSemiTreeNumber==0 && treeNumber==0) {
			if(chromLeafNumber%2==0)// && chromSemiLeafNumber<=1
				return Semi;
			else
			if(chromLeafNumber%2==1 && chromSemiLeafNumber==1)
				return Weak;
			
		}
		return Non;
	}
	ostream& operator<<(ostream& os,const GeneralChromTree& chtree)
	{

		os<<"  chromosome leaf number = "<<chtree.chromLeafNumber<<"\n";
		os<<"  tree number = "<<chtree.treeNumber<<"\n";
		os<<"  chromosome semi leaf number = "<<chtree.chromSemiLeafNumber<<"\n";
		os<<"  semi tree number = "<<chtree.semiTreeNumber<<"\n";
	    os<<"  real semi tree number = "<<chtree.realSemiTreeNumber<<"\n";
		os<<"  semi real tree number = "<<chtree.semiRealTreeNumber<<"\n";
		list<GeneralChromTree::ChromTreeNode*>::const_iterator ritor=chtree.trees.begin();
		for ( ;ritor!=chtree.trees.end(); ++ritor) {

			os<<"    leaf number = "<<(*ritor)->leafNumber<<"\n";
			os<<"    leaf : ";
			vector<GeneralInterval>::const_iterator citor=(*ritor)->leafs.begin();
			for ( ;citor<(*ritor)->leafs.end(); ++citor) {
				os<<(*citor).begin<<"~"<<(*citor).end<<",";
			}
			os<<"\n";
			os<<"    semi leaf number = "<<(*ritor)->semiLeafNumber<<"\n";
			os<<"    semi leaf : ";
			citor=(*ritor)->semiLeafs.begin();
			for ( ;citor<(*ritor)->semiLeafs.end(); ++citor) {
				os<<(*citor).begin<<"~"<<(*citor).end<<",";
			}
			os<<"\n";
		}

		return os;
	}

}//end of namespace
