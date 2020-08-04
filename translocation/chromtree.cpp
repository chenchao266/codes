#include <cassert>
#include <stack>
#include <stdexcept>
#include "chromtree.hpp"
#include "geneposition.hpp"
#include "transparam.hpp"
#include "permutation.hpp"
using std::stack;
using std::make_pair;
namespace Translocation
{

	ChromTree::ChromTree()
		:treeNumber(0),chromLeafNumber(0)
	{
	}
	ChromTree::~ChromTree()
	{
		deleteTP();
	}

	void ChromTree::ChromTreeNode::deleteTree()
	{
		TreeNode* q=root;
		TreeNode* p=q;

		while (q!=NULL) {
			while (!q->child.empty())
				q=q->child.front();  //begin()
			p=q->parent;
			//q->child.clear();
			//std::cout<<"?delete tree?"<<q->intvl.begin<<"~"<<q->intvl.end<<"??"<<"\n";
			delete q;

			if (p!=NULL){
				p->child.pop_front();
			}
			q=p;
		}
		root=NULL;
	}

	void ChromTree::deleteTP()
	{
		chromLeafNumber=0;
		treeNumber=0;


		while (!trees.empty()) {
			ChromTreeNode* ctnode=trees.front();
			ctnode->leafs.clear();

			if(ctnode->root!=NULL)
				ctnode->deleteTree();//

			delete ctnode;
			trees.pop_front();//
		}

	}
	void ChromTree::clearTrees()
	{
		list<ChromTreeNode*>::iterator ritor=trees.begin();
		for ( ;ritor!=trees.end(); ++ritor) {
			(*ritor)->deleteTree();
		}
	}

	void ChromTree::constructTP(const vector<Interval>& comps, const short int* start, const short int* end, const int length)
	{

		treeNumber=0;
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
					qn->intvl.begin=0;//
					qn->intvl.end=0;//

					qn->parent=p;
					if (p==NULL) {
						chtrnode=new ChromTreeNode();//
						chtrnode->root=qn;
						chtrnode->leafNumber=0;
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
				pn->intvl.begin=comps[compIndex].begin;
				pn->intvl.end=comps[compIndex].end;

				q->child.push_back(pn);
				++q->intvl.begin;//square node child number

				p=pn;
#ifdef _DEBUG_OUT
				std::cout<<"?construct tp?"<<compIndex<<" : "<<p->intvl.begin<<"~"<<p->intvl.end<<"??"<<"\n";
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
		}
	}

	void ChromTree::pruneTP(const int length)
	{
		vector<list<TreeNode *>::iterator> itvt;

		list<ChromTreeNode*>::iterator ritor=trees.begin();
		for ( ;ritor!=trees.end(); ) {//++ritor
			(*ritor)->leafNumber=0;
			(*ritor)->leafs.clear();
			bool del=false;
			TreeNode* q=(*ritor)->root;
			TreeNode* pq=q;
			list<TreeNode *>::iterator itor=q->child.begin();

			int level=0;
			Interval itvl;
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
				std::cout<<"?prune tp?level?"<<level<<"\n";
#endif
				assert(q->style==Round);
				pq=q->parent;
				if (q->intvl.end - q->intvl.begin == 1) { //trivial component
					delete q;

					itor=pq->child.erase(itor);//
					itvt[level-1]=itor;//next node 
					--pq->intvl.begin;
					//assert(pq->style==Square);
					if (pq->child.empty()) {
						q=pq->parent;
						delete pq;

						--level;
						itvt.pop_back();
						if (q==NULL) {  //

							--treeNumber;
							delete  *ritor ;
							ritor=trees.erase(ritor); 
							del=true;
							break;//next tree;
						}

						itor=q->child.erase(itvt[level-1]);///itor
						itvt[level-1]=itor;//next node
						pq=q;
						itvl=q->intvl;

						if(q->child.empty()){//q become new leaf.
							//if( itvl.begin==4 && itvl.end==6)
							//{
							//	std::cout<<itvl.begin<<"-"<<itvl.end;								
							//}
							++((*ritor)->leafNumber);

							if((*ritor)->leafNumber >Common::geneNumber )//((*ritor)->leafs.size() > 0) && ( (*ritor)->leafs.back()==itvl)
							{
								std::cout<<itvl.begin<<"-"<<itvl.end;
								throw(std::runtime_error("prunetp"));
							}
							(*ritor)->leafs.push_back(itvl);

#ifdef _DEBUG_OUT
							std::cout<<"?prune tp?leafs2?"<<q->intvl.begin<<"~"<<q->intvl.end<<"\n";
#endif
							pq=q->parent;
							--level;
							itvt.pop_back();
							++itvt[level-1];//next node
						}//if q
					}//if pq

				} 
				else {//leaf
					itvl=q->intvl;
					//if( itvl.begin==4 && itvl.end==6)
					//{
					//	std::cout<<itvl.begin<<"-"<<itvl.end;
					//}
					++((*ritor)->leafNumber);
					if((*ritor)->leafNumber >Common::geneNumber )// ((*ritor)->leafs.size() > 0) && ((*ritor)->leafs.back()==itvl)
					{
						std::cout<<itvl.begin<<"-"<<itvl.end;
						throw(std::runtime_error("prunetp"));
					}
					(*ritor)->leafs.push_back(itvl);//??

#ifdef _DEBUG_OUT
					std::cout<<"?prune tp?leafs1?"<<itvl.begin<<"~"<<itvl.end<<"\n";
#endif
					++itvt[level-1];//next node
				}
				
				q=pq;
				--level;
			}//while pq
			assert(itvt.size()==0);
			if(ritor!=trees.end()){
				(*ritor)->intvl.begin=(*ritor)->root->child.front()->intvl.begin;// 
				(*ritor)->intvl.end=(*ritor)->root->child.back()->intvl.end;// 
#ifdef _DEBUG_OUT
				std::cout<<"?prune tp?left~right?"<<(*ritor)->intvl.begin<<"~"<<(*ritor)->intvl.end<<"\n";
#endif
			}
			if(del==false)
				++ritor;
		}//for  ritor
		itvt.clear();
#ifdef _DEBUG_OUT
		//std::cout<<"?prune tp?tree number?"<<treeNumber<<"\n";
#endif
	}
	void ChromTree::countChromLeafNumber()
	{
#ifdef _DEBUG_OUT
		//std::cout<<"?tree number?"<<treeNumber<<"\n";
#endif
		chromLeafNumber=0;

		if ((0!=treeNumber)) {//||(0!=semiTreeNumber)

			list<ChromTreeNode*>::const_iterator ritor=trees.begin();
			for ( ;ritor!=trees.end(); ++ritor) {
				chromLeafNumber+=(*ritor)->leafNumber;
			}
		}
		//assert(treeNumber<=chromLeafNumber);

	}

	ostream& operator<<(ostream& os,const ChromTree& chtree)
	{

		os<<"  chromosome leaf number = "<<chtree.chromLeafNumber<<"\n";
		os<<"  tree number = "<<chtree.treeNumber<<"\n";

		list<ChromTree::ChromTreeNode*>::const_iterator ritor=chtree.trees.begin();
		for ( ;ritor!=chtree.trees.end(); ++ritor) {

			os<<"    leaf number = "<<(*ritor)->leafNumber<<"\n";
			os<<"    leaf : ";
			vector<Interval>::const_iterator citor=(*ritor)->leafs.begin();
			for ( ;citor<(*ritor)->leafs.end(); ++citor) {
				os<<(*citor).begin<<"~"<<(*citor).end<<",";
			}
			os<<"\n";

		}

		return os;
	}

}//end of namespace
