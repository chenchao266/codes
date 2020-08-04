#include <cstdlib>
#include <stack>
#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "pathcycle.hpp"
#include "transparam.hpp"
using std::stack;

namespace GeneralTranslocation
{
	PathCycle::PathCycle()
	{
		idxPath.reserve(2*Common::geneNumber);
		for (int i=0; i<2*Common::geneNumber; ++i)
		{
			idxPath.push_back(std::make_pair((Path*)NULL,-1));//
		}

	}
	PathCycle::~PathCycle()
	{
		idxPath.clear();
		while (!m_AAPath.empty()) {
			Path* pth=m_AAPath.front();
			delete pth;
			m_AAPath.pop_front();//
		}
		while (!m_ABPath.empty()) {
			Path* pth=m_ABPath.front();
			delete pth;
			m_ABPath.pop_front();//
		}
		while (!m_BBPath.empty()) {
			Path* pth=m_BBPath.front();
			delete pth;
			m_BBPath.pop_front();//
		}
		while (!m_cycles.empty()) {
			Path* pth=m_cycles.front();
			delete pth;
			m_cycles.pop_front();//
		}
	}

	void PathCycle::getPathCycles(const GeneralPermutation& permut,const GenePosition& genePosit, vector<TransParam>& trsprms, bool fill)//
	{
		//    if(fill){
		//        m_cycle.clear();
		//    }
		m_AANumber=0;
		m_ABNumber=0;
		m_BBNumber=0;
		m_cycleNumber=0;
		m_AAPath.clear();
		m_ABPath.clear();
		m_BBPath.clear();
		m_cycles.clear();
		//int cycleNumber=0;
		idxPath.clear();
		for (int i=0; i<2*Common::geneNumber; ++i)
		{
			idxPath.push_back(std::make_pair((Path*)NULL,-1));//
		}
		const int n=Common::geneNumber;
		int* visited=new int[n];//0..n-1 , point;
		int i=0;
		for (i=0; i<n; ++i) {
			visited[i]=0;
		}


		int chbegin=0;
		int genebegin=0;
		int fpi,fpj,nextj;

		Position posit,positi;
		vector<Position> oneCycle;

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
			int m=0;
			positi.chromIndex=chbegin;
			positi.geneIndex=genebegin;
			int idxbegin=-1;
			//bool extgray=false;
			unsigned int idxi=0;//2x-1 2x
			unsigned int idxj=0;
			int totalindex=0;
#ifdef _DEBUG_OUT
			std::cout<<"?pathcycles?"<<chbegin<<"."<<genebegin<<"."<<adj<<":";
#endif



			while (true) {

				fpi=permut.getGene(positi.chromIndex,positi.geneIndex);//-1??

				idxi=2*(positi.geneIndex);//genePosit.getBeginIndex(positi.chromIndex)+
				if (fpi>0) {

					if (adj==1) {//right
						idxi++;
						genePosit.getNextGene(permut,fpi+1,posit,fpj,totalindex);
						++visited[totalindex];
						idxj=2*posit.geneIndex;
						if (fpj>0) {
							nextj=posit.geneIndex-1;
							
							adj=1;

						} else if (fpj<0) {
							nextj=posit.geneIndex+1;
							idxj++;
							adj=0;

						}
					} else if (adj==0) {//left
						
						genePosit.getNextGene(permut,fpi-1,posit,fpj,totalindex);
						++visited[totalindex];
						idxj=2*posit.geneIndex;

						if (fpj>0) {
							nextj=posit.geneIndex+1;
							idxj++;
							adj=0;

						} else if (fpj<0) {
							nextj=posit.geneIndex-1;
							
							adj=1;

						}
					}

				} else if (fpi<0) {
					if (adj==1) {//right
						idxi++;
						genePosit.getNextGene(permut,-fpi-1,posit,fpj,totalindex);
						++visited[totalindex];
						idxj=2*posit.geneIndex;
						if (fpj>0) {
							nextj=posit.geneIndex+1;
							idxj++;
							adj=0;

						} else if (fpj<0) {
							nextj=posit.geneIndex-1;
												
							adj=1;

						}
					} else if (adj==0) {//left
						
						genePosit.getNextGene(permut,-fpi+1,posit,fpj,totalindex);
						++visited[totalindex];
						idxj=2*posit.geneIndex;
						if (fpj>0) {
							nextj=posit.geneIndex-1;

							adj=1;

						} else if (fpj<0) {
							nextj=posit.geneIndex+1;
							idxj++;
							adj=0;

						}
					}

				}
#ifdef _DEBUG_OUT
				std::cout<<"~"<<posit.chromIndex<<"."<<nextj<<"."<<adj<<"|"<<idxi<<"-"<<idxj;
#endif
				++visited[genePosit.getBeginIndex(posit.chromIndex)+nextj];
				
				positi.geneIndex=idxi;
				oneCycle.push_back(positi);///
				if(idxbegin<0)
					idxbegin=idxi;
				posit.geneIndex=idxj;
				oneCycle.push_back(posit);///

				if ((chbegin==posit.chromIndex)&&(nextj==genebegin)&&(adj==1))
					break;
				if((posit.chromIndex==0)&&(nextj==0)&&(adj==1))
					break;
				++m;
				if( m>n ){
					std::cout<<permut;
					throw(std::runtime_error("pathcycles"));
				}


				positi.chromIndex=posit.chromIndex;
				positi.geneIndex=nextj;//??
			}// while(true)

#ifdef _DEBUG_OUT
			std::cout<<"\n";
#endif

			//direct=false;

			positi.chromIndex=chbegin;
			positi.geneIndex=idxbegin;//genebegin
			oneCycle.push_back(positi);///0

			//getProperTransParam(oneCycle,trsprms);
			splitCycles(permut,genePosit,oneCycle);
			oneCycle.clear();


		}//for

		delete [] visited;
		constructInterleave(genePosit);
	}
	void PathCycle::splitCycles(const GeneralPermutation& permut, const GenePosition& genePosit, vector<Position>& oneCycle)
	{
		Path* pathcycle;
		Tail tail1=non;
		Tail tail2=non;
		Tail firstTail=non;
		Tail nextTail=non;
		int tail1Idx=-1;
		int tail2Idx=-1;
		int firstTailIdx=-1;
		int nextTailIdx=-1;
		vector<Position> tempCycle;
		bool path=false;
		bool intraChrom=true;
		Position beginPosit=oneCycle[0];
		Position endPosit=oneCycle[0];
		for (int i=1; i<oneCycle.size(); ++i,++i){
			Position posit=oneCycle[i];
			Position positbefore=oneCycle[i-1];
			if(permut.isTails(posit.chromIndex,posit.geneIndex/2)){
				nextTail= A;
				nextTailIdx=i;
				if(permut.isTails(positbefore.chromIndex,positbefore.geneIndex/2)){
					tail2= A;
				}
				else {
					tail2= B;
				}
				tail2Idx=i-1;

			}//??
			else
				if(permut.isTails(positbefore.chromIndex,positbefore.geneIndex/2)){
					tail2= A;
					tail2Idx=i-1;
					nextTail=B;
					nextTailIdx=i;

				}
				else {
					//continue;//?
				}
				if(firstTail==non){
					firstTail=tail2;
					firstTailIdx=tail2Idx;
				}


				if(positbefore.chromIndex!=beginPosit.chromIndex){//??
					intraChrom=false;
				}
				if(positbefore.chromIndex<beginPosit.chromIndex ||
					(positbefore.chromIndex==beginPosit.chromIndex && positbefore.geneIndex<beginPosit.geneIndex)){
						beginPosit=positbefore;
				}
				if(positbefore.chromIndex>endPosit.chromIndex ||
					(positbefore.chromIndex==endPosit.chromIndex && positbefore.geneIndex>endPosit.geneIndex)){
						endPosit=positbefore;
				}

				if(nextTail==non){

					if(posit.chromIndex!=beginPosit.chromIndex){
						intraChrom=false;
					}
					if(posit.chromIndex<beginPosit.chromIndex ||
						(posit.chromIndex==beginPosit.chromIndex && posit.geneIndex<beginPosit.geneIndex)){
							beginPosit=posit;
					}
					if(posit.chromIndex>endPosit.chromIndex ||
						(posit.chromIndex==endPosit.chromIndex && posit.geneIndex>endPosit.geneIndex)){
							endPosit=posit;
					}
				}
				if(tail1Idx>=tail2Idx){
					continue;//??
				}
				if(tail1!=non)
				{

					pathcycle=new Path();
					pathcycle->intraChrom=intraChrom;
					pathcycle->beginPosit=beginPosit;
					pathcycle->endPosit=endPosit;
					tempCycle.clear();
					tempCycle.reserve(tail2Idx-tail1Idx+1);
					for(vector<Position>::const_iterator vppi=oneCycle.begin()+tail1Idx;vppi<oneCycle.begin()+tail2Idx+1;++vppi){
						tempCycle.push_back((*vppi));	
						int idx=genePosit.getBeginIndex((*vppi).chromIndex)*2+(*vppi).geneIndex;
						idxPath[idx].first=pathcycle;
					}
					pathcycle->path=tempCycle;


					if((tail1==A)&&(tail2==A)){
						path=true;
						pathcycle->type=AA;
						m_AAPath.push_back(pathcycle);
						++m_AANumber;

					}
					else if(((tail1==B)&&(tail2==A))||((tail2==B)&&(tail1==A))){
						path=true;
						pathcycle->type=AB;
						m_ABPath.push_back(pathcycle);
						++m_ABNumber;

					}
					else if((tail1==B)&&(tail2==B)){
						path=true;
						pathcycle->type=BB;
						m_BBPath.push_back(pathcycle);
						++m_BBNumber;

					}
				}
				tail1=nextTail;
				tail1Idx=nextTailIdx;

				tail2=non;
				tail2Idx=-1;
				nextTail=non;
				intraChrom=true;
				beginPosit=posit;
				endPosit=posit;



		}//for
		if(firstTailIdx>=0){

			//tail1=lastTail;
			tail2=firstTail;

			for(int i=0; i<firstTailIdx+1; ++i){
				Position posit=oneCycle[i];
				if(posit.chromIndex!=beginPosit.chromIndex){
					intraChrom=false;
				}
				if(posit.chromIndex<beginPosit.chromIndex ||
					(posit.chromIndex==beginPosit.chromIndex && posit.geneIndex<beginPosit.geneIndex)){
						beginPosit=posit;
				}
				if(posit.chromIndex>endPosit.chromIndex ||
					(posit.chromIndex==endPosit.chromIndex && posit.geneIndex>endPosit.geneIndex)){
						endPosit=posit;
				}
			}

			pathcycle=new Path();
			pathcycle->intraChrom=intraChrom;
			pathcycle->beginPosit=beginPosit;
			pathcycle->endPosit=endPosit;
			tempCycle.clear();
			tempCycle.reserve(oneCycle.size()-tail1Idx+firstTailIdx);
			for(vector<Position>::const_iterator vppi=oneCycle.begin()+tail1Idx;vppi<oneCycle.end();++vppi){
				tempCycle.push_back((*vppi));		
				int idx=genePosit.getBeginIndex((*vppi).chromIndex)*2+(*vppi).geneIndex;
				idxPath[idx].first=pathcycle;
			}
			for(vector<Position>::const_iterator vppi=oneCycle.begin()+1;vppi<oneCycle.begin()+firstTailIdx+1;++vppi){
				tempCycle.push_back((*vppi));		
				int idx=genePosit.getBeginIndex((*vppi).chromIndex)*2+(*vppi).geneIndex;
				idxPath[idx].first=pathcycle;
			}
			pathcycle->path=tempCycle;

			if((tail1==A)&&(tail2==A)){
				path=true;
				pathcycle->type=AA;
				m_AAPath.push_back(pathcycle);
				++m_AANumber;

			}
			else if(((tail1==B)&&(tail2==A))||((tail2==B)&&(tail1==A))){
				path=true;
				pathcycle->type=AB;
				m_ABPath.push_back(pathcycle);
				++m_ABNumber;

			}
			else if((tail1==B)&&(tail2==B)){
				path=true;
				pathcycle->type=BB;
				m_BBPath.push_back(pathcycle);
				++m_BBNumber;
			}
		}//if(firstTailIdx)

		if(path==false){
			pathcycle=new Path();
			pathcycle->intraChrom=intraChrom;
			pathcycle->beginPosit=beginPosit;
			pathcycle->endPosit=endPosit;
			tempCycle.clear();
			tempCycle.reserve(oneCycle.size());
			for(vector<Position>::const_iterator vppi=oneCycle.begin();vppi<oneCycle.end();++vppi){
				tempCycle.push_back((*vppi));		
				int idx=genePosit.getBeginIndex((*vppi).chromIndex)*2+(*vppi).geneIndex;
				idxPath[idx].first=pathcycle;
			}
			pathcycle->path=tempCycle;
			pathcycle->type=CYCLE;
			m_cycles.push_back(pathcycle);
			++m_cycleNumber;
		}

	}


	//??many components interleave to a component
	void PathCycle::constructInterleave(const GenePosition& genePosit)
	{
		const int n=Common::geneNumber;
		//Path** parent=new Path*[2*n];
		int i=0;
		stack<pair<Position, Position> > stkpp;
		for(i=0; i<2*n; ++i){//
			if(idxPath[i].first==NULL)
				continue;
			if(i==2*(genePosit.getBeginIndex(idxPath[i].first->beginPosit.chromIndex))+idxPath[i].first->beginPosit.geneIndex)//
				stkpp.push(std::make_pair(idxPath[i].first->beginPosit,idxPath[i].first->endPosit));
			pair<Position, Position> ext;
			ext.first=idxPath[i].first->beginPosit;
			ext.second=idxPath[i].first->endPosit;
			while(stkpp.top().first > idxPath[i].first->beginPosit){
				ext.first=std::min(ext.first, stkpp.top().first);
				ext.second=std::max(ext.second, stkpp.top().second);
				int j=2*(genePosit.getBeginIndex(stkpp.top().first.chromIndex))+stkpp.top().first.geneIndex;
				//parent[j]=idxPath[i];
#ifdef _DEBUG_OUT
				std::cout<<"interleave:"<<j<<"->"<<i<<"\n";
#endif
				if(idxPath[j].first->intraChrom==false || idxPath[i].first->intraChrom==false){
					idxPath[j].first->intraChrom=false;
					idxPath[i].first->intraChrom=false;
				}

				int k=i;
				while(idxPath[k].second!=-1){//??loop forever
					//assert(k!=idxPath[k].second);
					k=idxPath[k].second;
					if(idxPath[j].first->intraChrom==false){
						idxPath[k].first->intraChrom=false;
					}
				}
				idxPath[k].second=j;//i-->j


				stkpp.pop();
			}//while
			stkpp.top().first=std::min(ext.first, stkpp.top().first);
			stkpp.top().second=std::max(ext.second, stkpp.top().second);
			if(i==2*(genePosit.getBeginIndex(stkpp.top().second.chromIndex))+stkpp.top().second.geneIndex)
				stkpp.pop();

		}//for
		//delete [] parent;
	}

	
	int  PathCycle::findComps(const GeneralPermutation& permut, const GenePosition& genePosit, const int chn, vector<GeneralInterval>& comps, short int* compBegin, short int* compEnd)const
	{
		int n=permut.getLength(chn);
		if(n==2)//AA
			return 0;		
		int compNumber=0;
		int i=2*genePosit.getBeginIndex(chn);//
		Path* path=NULL;
		n*=2;
		int * visited=new int[n];
		for(int t=0;t<n;++t){
			visited[t]=0;
		}
		for(int j=0; j<n; ++j, ++j){
			int k=i+j;
			
			vector<Position>::const_iterator vpitor;
			if(idxPath[k].first!=NULL && visited[j]==0 && idxPath[k].first->intraChrom ){
				PathType abc=CYCLE;
				if(idxPath[k].first->type==AA) {//
					abc=AA;					
				}
				else if(idxPath[k].first->type==BB){
					abc=BB;
				}
				else if(idxPath[k].first->type==AB){
					abc=AB;
				}
				//else abc=CYCLE;
					
					int s=idxPath[k].first->beginPosit.geneIndex;
					int t=idxPath[k].first->endPosit.geneIndex;
					for(vpitor=idxPath[k].first->path.begin();vpitor<idxPath[k].first->path.end();++vpitor){
						visited[(*vpitor).geneIndex]=1;///
					}
					
					while(idxPath[k].second!=-1){
						k=idxPath[k].second;
						if(idxPath[k].first->type==AA) {//
							abc=AA;
						}
						else if(idxPath[k].first->type==BB){
							abc=BB;
						}
						else if(idxPath[k].first->type==AB){
							abc=AB;
						}
						//else abc=CYCLE;
						for(vpitor=idxPath[k].first->path.begin();vpitor<idxPath[k].first->path.end();++vpitor){
							visited[(*vpitor).geneIndex]=1;///
						}
						if(idxPath[k].first->beginPosit.geneIndex < s)
							s=idxPath[k].first->beginPosit.geneIndex;
						if(idxPath[k].first->endPosit.geneIndex > t)
							t=idxPath[k].first->endPosit.geneIndex;
					}

					GeneralInterval itvl;
					itvl.begin=s/2;
					itvl.end=t/2;
					itvl.type=abc;
#ifdef _DEBUG_OUT
	std::cout<<chn<<"?find comps?"<<itvl<<"\n";
#endif
					comps.push_back(itvl);//
					++compNumber;
					if(compBegin[itvl.begin]!=0){
						std::cout<<permut;
						throw(std::runtime_error("general components"));
					}
				//assert(compBegin[s]==0);
					compBegin[itvl.begin]=compNumber;
					compEnd[itvl.end]=compNumber;

			}
		}
		delete [] visited;
		return compNumber;
	}
    void PathCycle::optimizeCaps(GeneralPermutation& permut)
	{
//3  对于G中所有包含两条AB 路径的SMSP，添加灰边闭合其中一条AB 路径 
//4  if   G中有2 个SMSP，并且这两个SMSP 相互依赖 
//5  按引理6找一条有效灰边g 添加到G中
//6  if   G包含真偶隔离带，且G 中的SMSP数大于0 
//7  按引理7找一条有效灰边
//8  if   G包含弱偶隔离带 
//9  按引理8找一条有效灰边
//10  if   G包含半偶隔离带 
//11  按引理9找一条有效灰边
//12  while   G中有路径 
//13  if   G中的SMSP数不少于2
//14  令 sm 为 G 中的 SMSP 数，按引理 1 找sm/2条有效灰边
//15  else if   G中只有一个SMSP 
//16  if   G中的RMSP数为偶数
//17  令g为连接 SMSP中的AB路径的B-tail和一条AA路径的灰边
//18  else 
//19  令g为闭合 SMSP中的AB路径的灰边
//20  else if   G中有 BB路径 
//21  令p为G中的BB路径数，根据引理10找p条有效灰边
//22  else
//23  令g为闭合任意一条路径的灰边
//24  endwhile 
//25 
//根据G计算B 的最优加帽基因组 OPT B 
	}
	inline
		void PathCycle::getProperTransParam(vector<Position>& extcycle,vector<TransParam>& trsprms)
	{
		TransParam trprm;
		int chi1,chi2,gni1,gni2;

		//list<vector<Position> >::const_iterator extCycitor = m_cycle.begin();
		//for (; extCycitor != m_cycle.end(); ++extCycitor) {
		vector<Position>::const_iterator vitor1 = extcycle.begin();

		for (++vitor1; vitor1 <= extcycle.end()-4; ++vitor1){//-6
			chi1=(*vitor1).chromIndex;
			gni1=(*vitor1).geneIndex;
			if(gni1>(*(++vitor1)).geneIndex)
				gni1=(*vitor1).geneIndex;
			assert(chi1==(*vitor1).chromIndex);
#ifdef _DEBUG_OUT
			std::cout<<"?get proper chi1?"<<chi1<<";"<<"\n";
#endif
			vector<Position>::const_iterator vitor2=vitor1+1;//+3

			for (; vitor2 <= extcycle.end()-2; ++vitor2){//-2
#ifdef _DEBUG_OUT
				std::cout<<"?get proper t2?"<<(*vitor2).chromIndex<<";"<<"\n";
#endif

				if((*vitor2).chromIndex!=(*vitor1).chromIndex){//(*vitor1)
					chi2=(*vitor2).chromIndex;
					gni2=(*vitor2).geneIndex;
					if(gni2>(*(++vitor2)).geneIndex)
						gni2=(*vitor2).geneIndex;
					assert(chi2==(*vitor2).chromIndex);
#ifdef _DEBUG_OUT
					std::cout<<"?get proper 1?2?"<<chi1<<"?"<<chi2<<"\n";
#endif

					if(chi1<chi2){

						trprm.chromIndex1=chi1;
						trprm.chromIndex2=chi2;

						trprm.geneIndex1=gni1;
						trprm.geneIndex2=gni2;

					}else {//if(chi2<chi1)
						trprm.chromIndex1=chi2;
						trprm.chromIndex2=chi1;

						trprm.geneIndex1=gni2;
						trprm.geneIndex2=gni1;

					}
					assert(trprm.chromIndex2!=trprm.chromIndex1);

					trprm.transStyle=PrefixPrefix;
					trsprms.push_back(trprm);
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
	ostream& operator<<(ostream& os,const GeneralInterval& gintvl)
	{
		os<<gintvl.begin<<"~"<<gintvl.end<<":"<<gintvl.type<<";";
		return os;
	}
	ostream& operator<<(ostream& os,const PathCycle::Path& paths)
	{
		os<<"   ";
		os<<paths.intraChrom<<" : "<<paths.beginPosit<<"-"<<paths.endPosit<<" : ";
		vector<Position>::const_iterator vitor=paths.path.begin();
		for (; vitor<paths.path.end(); ++vitor)
			os<<(*vitor).chromIndex<<"."<<(*vitor).geneIndex<<"~";
		os<<"\n";
		return os;
	}
	ostream& operator<<(ostream& os,const PathCycle& cycles)
	{
		os<<"number of cycles = "<<cycles.m_cycleNumber<<"\n";
		list<PathCycle::Path* >::const_iterator extCycitor = cycles.m_cycles.begin();
		for (; extCycitor!=cycles.m_cycles.end(); ++extCycitor) {
			os<<*(*extCycitor);
		}

		os<<"number of AA path = "<<cycles.m_AANumber<<"\n";
		extCycitor = cycles.m_AAPath.begin();
		for (; extCycitor!=cycles.m_AAPath.end(); ++extCycitor) {
			os<<*(*extCycitor);
		}

		os<<"number of AB path "<<cycles.m_ABNumber<<"\n";
		extCycitor = cycles.m_ABPath.begin();
		for (; extCycitor!=cycles.m_ABPath.end(); ++extCycitor) {
			os<<*(*extCycitor);
		}

		os<<"number of BB path "<<cycles.m_BBNumber<<"\n";
		extCycitor = cycles.m_BBPath.begin();
		for (; extCycitor!=cycles.m_BBPath.end(); ++extCycitor) {
			os<<*(*extCycitor);
		}

		os<<"\n";
		return os;
	}
}//namespace

