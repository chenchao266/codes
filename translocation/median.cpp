 #include <boost/unordered_map.hpp>
#include <cassert>
#include <stdexcept>
#include <queue>
#include <set>
#include <algorithm>
#include "enumtrans.hpp"
#include "bfenumtrans.hpp"
#include "transparam.hpp"
#include "permutation.hpp"

#include "median.hpp"
namespace Translocation
{

Median::Median(Permutation* permut1,Permutation* permut2,Permutation* permut3)
		:d12(0),d13(0),d23(0),medianscore(0),md1(0),md2(0),md3(0)
{
	perm1=permut1;
	perm2=permut2;
	perm3=permut3;
	medianperm=new Permutation(*perm1);
}

Median::~Median(void)
{
	if (medianperm!=NULL)
	{
		delete medianperm;
		medianperm=NULL;
	}
}
std::ostream& operator<< (std::ostream& os, const Median::Vertex& vtx)
{
	if (vtx.label==NULL){
		return os;
	}
	os<<*vtx.label;
	os<<vtx.best<<"~"<<vtx.worst<<";"<<vtx.d1<<","<<vtx.d2<<","<<vtx.d3<<"\n";
	return os;

}
static std::size_t sg= Common::geneNumber*Common::geneNumber;
struct PermutationHash
	: std::unary_function<Permutation, std::size_t>
{
 

	std::size_t operator()(Permutation const& perm) const
	{
		std::size_t seed =sg;

		for(int i=0; i<Common::chromNumber; ++i)
		{
			 seed+=perm.getLength(i)*perm.sum(i);//??
		}

		return seed;
	}

};
void Median::computeMedian(ostream& os, bool wrt)//???
{

	Permutation permut12=*perm1;
	Permutation permut13=*perm1;
	Permutation permut23=*perm2;
	renumber(permut12,*perm2);
	renumber(permut13,*perm3);
	renumber(permut23,*perm3);
	//BFEnumTrans bfet;
	EnumTrans et12;

	EnumTrans et13;
	EnumTrans et23;
	d12=et12.getDistance(permut12);
	d13=et13.getDistance(permut13);
	d23=et23.getDistance(permut23);
	int mmax= d12+d13 ;
	if (d12<=d23)
	{ 
		if (d13>d23)
		{//1<->2
			Permutation * temp=perm1;
			perm1=perm2;
			perm2=temp;
			mmax= d12+d23;
			//d13<->d23
			int tmp=d13;
			d13=d23;
			d23=tmp;
		}
	
	} 
	else //d12>d23
	{
		if (d13<=d12)
		{//1<->3
			Permutation * temp=perm1;
			perm1=perm3;
			perm3=temp;
			mmax= d13+d23;
			//d12<->d23
			int tmp=d12;
			d12=d23;
			d23=tmp;
		} 
		else //d13>d12
		{//1<->2
			Permutation * temp=perm1;
			perm1=perm2;
			perm2=temp;
			mmax= d12+d23;
			//d13<->d23
			int tmp=d13;
			d13=d23;
			d23=tmp;
		}
	}
	int msum=d12+d13+d23;
	if (msum%2==1)
	{
		++msum;
	}
	int mmin=msum/2;

	std::cout<<"perm1:"<<*perm1;
	std::cout<<"perm2:"<<*perm2;
	std::cout<<"perm3:"<<*perm3;
	std::cout<<"distance:"<<d12<<","<<d13<<","<<d23<<"\n";
	*medianperm=*perm1;
	bool pmed=false;	
	if(mmin==mmax){
		medianscore=mmin;
		md1=0;
		md2=d12;
		md3=d13;
		pmed=true; 
		return;
	}
	std::priority_queue<Vertex,vector<Vertex>,VertexCompare > pque;
	boost::unordered_map <Permutation, int, PermutationHash>  marks;
	boost::unordered_map <Permutation, int, PermutationHash>::iterator mit;
	Vertex v0;
	int qnold=0;
	int hlold=0;

	std::vector<TransParam> s2;
	std::vector<TransParam> s3;
	//std::vector<TransParam> n2;
	std::vector<TransParam> n3;
	std::vector<TransParam> setr;
	std::vector<TransParam>::iterator vtit;
	std::back_insert_iterator<std::vector<TransParam> > bir(setr);
	for (int pass=1; pass<=2; ++pass)  {
		v0.label=new Permutation(*perm1);
		v0.d1=0;
		v0.d2=d12;
		v0.d3=d13;
		v0.best=mmin;
		v0.worst=mmax;//已改变
		pque.push(v0);
		while (!pque.empty())
		{
			setr.clear();
			Vertex v=pque.top();
			pque.pop();
			if (v.best >= mmax){//??
				
				delete v.label;
				v.label=NULL;
				break;//while
			}
			Permutation permut4=*v.label;
			if (pass == 1) {			
				//(S2;N2)  find all sorting reversals(vlabel, π2);
				
				renumber(permut4,*perm2);
				et12.getDistance(permut4);
				//n2.clear();
				//et12.getNeutralTrans(n2);
				//sort(n2.begin(),n2.end());
				et12.findAllTrans(permut4);
				s2.clear();
				et12.getAllTrans(s2);
				
				sort(s2.begin(),s2.end());//,TransParamCompare
				
				//(S3;N3)  find all sorting reversals(vlabel, π3);
				permut4=*v.label;
				renumber(permut4,*perm3);
				et13.getDistance(permut4);
				//n3.clear();
				//et13.getNeutralTrans(n3);
				//sort(n3.begin(),n3.end());
				et13.findAllTrans(permut4);
				s3.clear();
				et13.getAllTrans(s3);
				
				sort(s3.begin(),s3.end());//,TransParamCompare
				//setr.reserve(s2.size());
				set_intersection(s2.begin(),s2.end(),s3.begin(),s3.end(),bir);
				//R S2  S3;
			}
			else //pass==2
			{	
				//(S4, N4)  find all sorting reversals(vlabel, π4);
				//R  {  is a possible reversal on vlabel and not in (S4 U N4) };
				//permut4=*v.label;
				renumber(permut4,*perm1);
				s2.clear();
				BFEnumTrans::getAllTrans(permut4,s2);
				sort(s2.begin(),s2.end());//,TransParamCompare

				et13.getDistance(permut4);
				n3.clear();
				et13.getNeutralTrans(n3);
				et13.findAllTrans(permut4);
				s3.clear();
				et13.getAllTrans(s3);
				
				s3.insert(s3.end(),n3.begin(),n3.end());//S4 U N4
				sort(s3.begin(),s3.end());//,TransParamCompare
				//setr.reserve(s2.size());
				set_difference(s2.begin(),s2.end(),s3.begin(),s3.end(),bir);

				
			}
#ifdef _DEBUG_OUT
				std::cout<<pass<<"?r?"<<setr.size()<<"\n";	//fout	
#endif			
			//assert(setr.size()>0);//??
			for ( vtit=setr.begin();vtit!=setr.end();++vtit)
			{
				Vertex w;
				w.label=new Permutation(*v.label);//
				w.label->doTranslocation(*vtit);
				//if w is marked then continue else mark w;??
				mit=marks.find(*w.label);//?/
				if (mit==marks.end())
				{
					marks.insert(std::make_pair(*w.label,1));//copy??!!
					//mit=.first;
				}
				else{
					delete w.label;
					if ((*mit).second==1){
						
						continue;
					}
					else
						(*mit).second=1;
				}
				w.d1=v.d1+1;//??
				

				if (pass == 1) {
					//if (binary_search(s2.begin(), s2.end(), *vtit)){// s2					
						w.d2=v.d2-1;						
					//}
					//else if (binary_search(n2.begin(), n2.end(), *vtit)){// n2
					//	w.d2=v.d2;
					//}
					//else{
					//	w.d2=v.d2+1;
					//}

					//if (binary_search(s3.begin(), s3.end(), *vtit)){// s3					
						w.d3=v.d3-1;
					//}
					//else if (binary_search(n3.begin(), n3.end(), *vtit)){// n3
					//	w.d3=v.d3;
					//}
					//else{
					//	w.d3=v.d3+1;
					//}
				}
				else //pass==2
				{
					permut4=*w.label;
					renumber(permut4,*perm2);
					w.d2=et12.getDistance(permut4);

					permut4=*w.label;
					renumber(permut4,*perm3);
					w.d3=et13.getDistance(permut4);

				}
				msum=w.d2+w.d3+d23;
				if (msum%2==1){
					++msum;
				}
				w.best=w.d1+ msum/2;//==mmin
				w.worst=w.d1+ w.d2+w.d3 ;
				if (w.worst == mmin) {
					*medianperm = *w.label;
					medianscore=mmin;//??0
					md1=w.d1;
					md2=w.d2;
					md3=w.d3;
					pmed=true; 
					
					delete w.label;
					w.label=NULL;

					break;//for
				}
				if (w.worst < mmax){
					*medianperm = *w.label;
					medianscore=w.d1+w.d2+w.d3;//??0
					md1=w.d1;
					md2=w.d2;
					md3=w.d3;
					mmax=w.worst;
					//pque.push(w);///??
					if (wrt)
					{
						os<<pass<<"?w?"<<w<<"\n";	//fout	
					}
					
				}  
				if (w.best < mmax) { //too many
					pque.push(w);
					 
				}
				else{
					delete w.label;
					w.label=NULL;
				}
#ifdef _DEBUG_OUT
				std::cout<<"?w?"<<w<<"\n";				
#endif
			}//for
			int qn=pque.size();
			
			if (wrt && qn>qnold)
			{
				os<<pass<<"?pq?"<<qn<<"\n";	//895
				qnold=qn;
			}
			int hl=marks.size();
			hl>>=10;

			if (wrt && hl>hlold)
			{

				os<<pass<<"?hl?"<<hl<<"k\n";	//
				hlold=hl;
				//os.flush();
			}		
			
			delete v.label;//??
			v.label=NULL;
			if (pmed)
				break;
				
				
			
		}//while
		if (pass == 1) {
			if (pmed == true)  
				break;
			else if (mmax == mmin+1)  
				break;
			else {
				for (mit=marks.begin(); mit!=marks.end(); ++mit)
				{
					(*mit).second=0;
				}
				//clear all marks
			}
		}

	}//for
	while (!pque.empty())
	{
			
		Vertex v=pque.top();
		delete v.label;
		pque.pop();
	}
	marks.clear();
#ifdef _DEBUG_OUT
	std::cout<<"?Median?"<<medianscore<<":\n"<<*medianperm<<"\n";				
#endif
	//assert(pque.empty());
}



void Median::computeMedianNM(ostream& os, bool wrt)
{

	Permutation permut12=*perm1;
	Permutation permut13=*perm1;
	Permutation permut23=*perm2;
	renumber(permut12,*perm2);
	renumber(permut13,*perm3);
	renumber(permut23,*perm3);
	//BFEnumTrans bfet;
	EnumTrans et12;

	EnumTrans et13;
	EnumTrans et23;
	d12=et12.getDistance(permut12);
	d13=et13.getDistance(permut13);
	d23=et23.getDistance(permut23);
	int mmax= d12+d13 ;
	if (d12<=d23)
	{ 
		if (d13>d23)
		{//1<->2
			Permutation * temp=perm1;
			perm1=perm2;
			perm2=temp;
			mmax= d12+d23;
			//d13<->d23
			int tmp=d13;
			d13=d23;
			d23=tmp;
		}

	} 
	else //d12>d23
	{
		if (d13<=d12)
		{//1<->3
			Permutation * temp=perm1;
			perm1=perm3;
			perm3=temp;
			mmax= d13+d23;
			//d12<->d23
			int tmp=d12;
			d12=d23;
			d23=tmp;
		} 
		else //d13>d12
		{//1<->2
			Permutation * temp=perm1;
			perm1=perm2;
			perm2=temp;
			mmax= d12+d23;
			//d13<->d23
			int tmp=d13;
			d13=d23;
			d23=tmp;
		}
	}
	int msum=d12+d13+d23;
	if (msum%2==1)
	{
		++msum;
	}
	int mmin=msum/2;

	std::cout<<"perm1:"<<*perm1;
	std::cout<<"perm2:"<<*perm2;
	std::cout<<"perm3:"<<*perm3;
	std::cout<<"distance:"<<d12<<","<<d13<<","<<d23<<"\n";
	*medianperm=*perm1;
	bool pmed=false;	
	if(mmin==mmax){
		medianscore=mmin;
		md1=0;
		md2=d12;
		md3=d13;
		pmed=true; 
		return;
	}
	std::priority_queue<Vertex,vector<Vertex>,VertexCompare > pque;

	Vertex v0;
	int qnold=0;
	int hlold=0;

	std::vector<TransParam> s2;
	std::vector<TransParam> s3;

	std::vector<TransParam> n3;
	std::vector<TransParam> setr;
	std::vector<TransParam>::iterator vtit;
	std::back_insert_iterator<std::vector<TransParam> > bir(setr);
	for (int pass=1; pass<=2; ++pass)  {
		v0.label=new Permutation(*perm1);
		v0.d1=0;
		v0.d2=d12;
		v0.d3=d13;
		v0.best=mmin;
		v0.worst=mmax;//已改变
		pque.push(v0);
		while (!pque.empty())
		{
			setr.clear();
			Vertex v=pque.top();
			pque.pop();
			if (v.best >= mmax){//??

				delete v.label;
				v.label=NULL;
				break;//while
			}
			Permutation permut4=*v.label;
			if (pass == 1) {			
				//(S2;N2)  find all sorting reversals(vlabel, π2);

				renumber(permut4,*perm2);
				et12.getDistance(permut4);

				et12.findAllTrans(permut4);
				s2.clear();
				et12.getAllTrans(s2);

				sort(s2.begin(),s2.end());//,TransParamCompare

				//(S3;N3)  find all sorting reversals(vlabel, π3);
				permut4=*v.label;
				renumber(permut4,*perm3);
				et13.getDistance(permut4);

				et13.findAllTrans(permut4);
				s3.clear();
				et13.getAllTrans(s3);

				sort(s3.begin(),s3.end());//,TransParamCompare
				set_intersection(s2.begin(),s2.end(),s3.begin(),s3.end(),bir);
				//R S2  S3;
			}
			else //pass==2
			{	
				//(S4, N4)  find all sorting reversals(vlabel, π4);
				//R  {  is a possible reversal on vlabel and not in (S4 U N4) };

				renumber(permut4,*perm1);
				s2.clear();
				BFEnumTrans::getAllTrans(permut4,s2);
				sort(s2.begin(),s2.end());//,TransParamCompare

				et13.getDistance(permut4);
				n3.clear();
				et13.getNeutralTrans(n3);
				et13.findAllTrans(permut4);
				s3.clear();
				et13.getAllTrans(s3);

				s3.insert(s3.end(),n3.begin(),n3.end());//S4 U N4
				sort(s3.begin(),s3.end());//,TransParamCompare
				set_difference(s2.begin(),s2.end(),s3.begin(),s3.end(),bir);


			}
#ifdef _DEBUG_OUT
			std::cout<<pass<<"?r?"<<setr.size()<<"\n";	//fout	
#endif			
			//assert(setr.size()>0);//??
			for ( vtit=setr.begin();vtit!=setr.end();++vtit)
			{
				Vertex w;
				w.label=new Permutation(*v.label);//
				w.label->doTranslocation(*vtit);

				w.d1=v.d1+1;//??


				if (pass == 1) {
			
					w.d2=v.d2-1;						
		
					w.d3=v.d3-1;

				}
				else //pass==2
				{
					permut4=*w.label;
					renumber(permut4,*perm2);
					w.d2=et12.getDistance(permut4);

					permut4=*w.label;
					renumber(permut4,*perm3);
					w.d3=et13.getDistance(permut4);

				}
				msum=w.d2+w.d3+d23;
				if (msum%2==1){
					++msum;
				}
				w.best=w.d1 + msum/2;//==mmin
				w.worst=w.d1 + w.d2 + w.d3 ;
				++hlold;

				if (wrt && ((hlold&0x00000FFF)==0))
				{
					os<<pass<<"?hl?"<<hlold<<"\n";	//
					
					std::cout<<pass<<"?hl?"<<hlold<<std::endl;	//
 
				}		
				

				if (w.worst == mmin) {
					*medianperm = *w.label;
					medianscore=mmin;//??0
					md1=w.d1;
					md2=w.d2;
					md3=w.d3;
					pmed=true; 

					delete w.label;
					w.label=NULL;

					break;//for
				}
				if (w.worst < mmax){
					*medianperm = *w.label;
					medianscore=w.d1+w.d2+w.d3;//??0
					md1=w.d1;
					md2=w.d2;
					md3=w.d3;
					mmax=w.worst;
					//pque.push(w);///??
					if (wrt)
					{
						os<<pass<<"?w?"<<w<<"\n";	//fout	
						//std::cout<<pass<<"?w?"<<w<<"\n";	//
					}
					
				}  
				if (w.best < mmax) { //too many
					pque.push(w);

				}
				else{
					delete w.label;
					w.label=NULL;
				}
#ifdef _DEBUG_OUT
				std::cout<<"?w?"<<w<<"\n";				
#endif
			}//for


			delete v.label;//??
			v.label=NULL;
			if (pmed)
				break;



		}//while
		if (pass == 1) {
			if (pmed == true)  
				break;
			else if (mmax == mmin+1)  
				break;
			else {

			}
		}

	}//for
	while (!pque.empty())
	{

		Vertex v=pque.top();
		delete v.label;
		pque.pop();
	}

#ifdef _DEBUG_OUT
	std::cout<<"?Median?"<<medianscore<<":\n"<<*medianperm<<"\n";				
#endif
	//assert(pque.empty());
}



ostream& operator<<(ostream& os,const Median& mdn)
{
	os<<"perm1:"<<*mdn.perm1;
	os<<"perm2:"<<*mdn.perm2;
	os<<"perm3:"<<*mdn.perm3;
	os<<"distance:"<<mdn.d12<<","<<mdn.d13<<","<<mdn.d23<<"\n";
	os<<"median:"<<*mdn.medianperm;
	os<<"median score:"<<mdn.medianscore<<"="<<mdn.md1<<"+"<<mdn.md2<<"+"<<mdn.md3<<"\n";
	return os;
}
}