#include <boost/unordered_map.hpp>
#include <cassert>
#include <stdexcept>
#include <stack>
#include <set>
#include <algorithm>
#include "henumtrans.hpp"
#include "transparam.hpp"
#include "permutation.hpp"

#include "hmedian.hpp"
namespace Translocation
{

	HMedian::HMedian(Permutation* permut1,Permutation* permut2,Permutation* permut3)
		:d12(0),d13(0),d23(0),medianscore(0),md1(0),md2(0),md3(0)
	{
		perm1=permut1;
		perm2=permut2;
		perm3=permut3;
		medianperm=new Permutation(*perm1);
	}

	HMedian::~HMedian(void)
	{
		if (medianperm!=NULL)
		{
			delete medianperm;
			medianperm=NULL;
		}
	}
	std::ostream& operator<< (std::ostream& os, const HMedian::Vertex& vtx)
	{
		if (vtx.label==NULL){
			return os;
		}
		os<<*vtx.label;
		os<<vtx.score<<";"<<vtx.d1<<","<<vtx.d2<<","<<vtx.d3<<"\n";
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
	void HMedian::computeMedian(ostream& os, bool wrt)
	{

		Permutation permut12=*perm1;
		Permutation permut13=*perm1;
		Permutation permut23=*perm2;
		renumber(permut12,*perm2);
		renumber(permut13,*perm3);
		renumber(permut23,*perm3);
		//BFEnumTrans bfet;
		HEnumTrans et12;

		HEnumTrans et13;
		HEnumTrans et23;
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

		//boost::unordered_map <Permutation, int, PermutationHash>  marks;
		//boost::unordered_map <Permutation, int, PermutationHash>::iterator mitor;
		Vertex v0;
		int qnold=0;
		int hlold=0;
		int bestsofar=d12+d13+d23;
		std::vector<HTransParam> s2;

		std::vector<HTransParam> s3;
		std::vector<HTransParam>::iterator sitor2,sitor3;
		vhetp setr;
		vhetp ::iterator ritor,ritor2;
		std::vector<vector<int > > wgtmatrix;
		std::vector<int > weight;

		std::stack<Vertex> soth;
		v0.label=new Permutation(*perm1);
		v0.d1=0;
		v0.d2=d12;
		v0.d3=d13;
		v0.score=mmax;//ÒÑ¸Ä±ä

		Vertex v=v0;

		do
		{
			Permutation permut4=*v.label;

			renumber(permut4,*perm2);
			et12.getDistance(permut4);

			et12.findAllTrans(permut4);
			
			s2.clear();
			et12.getAllTrans(s2);
			//-----------
			//os<<permut4;
			//os<<et12;	
			//for (int i=0; i<s2.size(); ++i)
			//{
			//	os<<s2[i]<<"\n";
			//}
			//------------
			sort(s2.begin(),s2.end());//,TransParamCompare

			//(S3;N3)  find all sorting reversals(vlabel, ¦Ð3);
			permut4=*v.label;
			renumber(permut4,*perm3);
			et13.getDistance(permut4);

			et13.findAllTrans(permut4);
			s3.clear();
			et13.getAllTrans(s3);

			sort(s3.begin(),s3.end());

			setr.clear();
			sitor2=s2.begin();
			sitor3=s3.begin();
			ExtTransParam etp;
			//set_intersection(s2.begin(),s2.end(),s3.begin(),s3.end(),bir);
			for (; sitor2 != s2.end() && sitor3 != s3.end(); ){
				if ( *sitor2 < *sitor3 )
					++sitor2;
				else if ( *sitor3 < *sitor2 )
					++sitor3;
				else{
					etp.cycnum1= sitor3->cycnum1;
					etp.cycnum2= sitor3->cycnum2;
					etp.grayIndex1= sitor3->grayIndex1;
					etp.grayIndex2= sitor3->grayIndex2;
					etp.leafchange=sitor3->leafchange;
					setr.push_back(std::make_pair(*sitor2,etp));
					sitor2++;
					sitor3++;
				}
			}
backr:
			if (setr.empty())
			{
				if (!soth.empty())
				{				
					v=soth.top();
					soth.pop();

					setr=v.other;
				}
				else
					break;

			}
			//no commute add 1; //begin
			for (int i=0; i<weight.size(); ++i)
			{
				weight[i]=0;				
				for (int j=0; j<wgtmatrix[i].size(); ++j)
				{
					wgtmatrix[i][j]=0;
				}
			}

			for (int i=weight.size(); i<setr.size(); ++i)
			{				
				weight.push_back(0);
				wgtmatrix.push_back(weight);//???

			}
			for (int i=0; i<setr.size(); ++i)
			{
				for (int j=i+1; j<setr.size(); ++j)
				{
					if (setr[i].first.transStyle!=setr[j].first.transStyle)
					{
						weight[i]++;weight[j]++;
						wgtmatrix[j][i]=1;

					}
					else
					if (setr[i].first.chromIndex1==setr[j].first.chromIndex1 && setr[i].first.geneIndex1==setr[j].first.geneIndex1)
					{
						weight[i]++;weight[j]++;
						wgtmatrix[j][i]=1;
					}
					else
					if (setr[i].first.chromIndex2==setr[j].first.chromIndex2 && setr[i].first.geneIndex2==setr[j].first.geneIndex2)
					{
						weight[i]++;weight[j]++;
						wgtmatrix[j][i]=1;
					}
					else
					if (setr[i].first.chromIndex1==setr[j].first.chromIndex2 && setr[i].first.geneIndex1==setr[j].first.geneIndex2)
					{
						weight[i]++;weight[j]++;
						wgtmatrix[j][i]=1;
					}
					else
					if (setr[i].first.chromIndex2==setr[j].first.chromIndex1 && setr[i].first.geneIndex2==setr[j].first.geneIndex1)
					{
						weight[i]++;weight[j]++;
						wgtmatrix[j][i]=1;
					}
					else
					if (setr[i].first.chromIndex1==setr[j].first.chromIndex1 && setr[i].first.chromIndex2==setr[j].first.chromIndex2 )
					{
						//if (setr[i].first.transStyle!=setr[j].first.transStyle)
						//{
						//	weight[i]++;weight[j]++;
						//	wgtmatrix[j][i]=1;

						//}
						//else 
						if (setr[i].first.transStyle==PrefixPrefix)
						{
							if ((setr[i].first.geneIndex1 <setr[j].first.geneIndex1 && setr[i].first.geneIndex2 > setr[j].first.geneIndex2)
								|| (setr[i].first.geneIndex1 > setr[j].first.geneIndex1 && setr[i].first.geneIndex2 < setr[j].first.geneIndex2))
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
						else if (setr[i].first.transStyle==PrefixSuffix)
						{
							if ((setr[i].first.geneIndex1 <setr[j].first.geneIndex1 && setr[i].first.geneIndex2 < setr[j].first.geneIndex2)
								|| (setr[i].first.geneIndex1 > setr[j].first.geneIndex1 && setr[i].first.geneIndex2 > setr[j].first.geneIndex2))
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
					}
					else
					if (setr[i].first.chromIndex2==setr[j].first.chromIndex1  )
					{
						if (setr[i].first.transStyle==PrefixPrefix)
						{
							if ( setr[i].first.geneIndex2 <= setr[j].first.geneIndex1  )
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
						else if (setr[i].first.transStyle==PrefixSuffix)
						{
							if ( setr[i].first.geneIndex2 >= setr[j].first.geneIndex1 )
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
					}
					else if (setr[i].first.chromIndex1==setr[j].first.chromIndex2)
					{
						if (setr[i].first.transStyle==PrefixPrefix)
						{
							if ( setr[i].first.geneIndex1 >= setr[j].first.geneIndex2 ) 
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
						else if (setr[i].first.transStyle==PrefixSuffix)
						{
							if ( setr[i].first.geneIndex1 <= setr[j].first.geneIndex2 )
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
					}
					else
					if (setr[i].first.chromIndex2==setr[j].first.chromIndex2  )
					{
						if (setr[i].first.transStyle==PrefixPrefix)
						{
							if ((setr[i].first.geneIndex2 <setr[j].first.geneIndex2 && setr[i].first.chromIndex1 > setr[j].first.chromIndex1)
								|| (setr[i].first.geneIndex2 > setr[j].first.geneIndex2 && setr[i].first.chromIndex1 < setr[j].first.chromIndex1))
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
						else if (setr[i].first.transStyle==PrefixSuffix)
						{
							if ((setr[i].first.geneIndex2 <setr[j].first.geneIndex2 && setr[i].first.chromIndex1 < setr[j].first.chromIndex1)
								|| (setr[i].first.geneIndex2 > setr[j].first.geneIndex2 && setr[i].first.chromIndex1 > setr[j].first.chromIndex1))
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
					}
					else
					if (setr[i].first.chromIndex1==setr[j].first.chromIndex1  )
					{
						if (setr[i].first.transStyle==PrefixPrefix)
						{
							if ((setr[i].first.geneIndex1 <setr[j].first.geneIndex1 && setr[i].first.chromIndex2 > setr[j].first.chromIndex2)
								|| (setr[i].first.geneIndex1 > setr[j].first.geneIndex1 && setr[i].first.chromIndex2 < setr[j].first.chromIndex2))
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
						else if (setr[i].first.transStyle==PrefixSuffix)
						{
							if ((setr[i].first.geneIndex1 <setr[j].first.geneIndex1 && setr[i].first.chromIndex2 < setr[j].first.chromIndex2)
								|| (setr[i].first.geneIndex1 > setr[j].first.geneIndex1 && setr[i].first.chromIndex2 > setr[j].first.chromIndex2))
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
						}
					}
				}//for j
			}//for i

			vhetp vtp;
			vtp.clear();
			int maxwgt=0;
			int maxidx=-1;
			do 
			{
				maxwgt=0;
				maxidx=-1;
				for(int i=0; i<weight.size(); ++i){
					if (weight[i]>maxwgt)
					{
						maxwgt=weight[i];
						maxidx=i;
					}
				}
				if (maxwgt>0)
				{
					vtp.push_back(setr[maxidx]);
					setr[maxidx].first.chromIndex1=-1;//!!
					if (vtp.size()==setr.size()-1)
					{
						int i=0;
					}
					//setr.pop_back();
					weight[maxidx]=0;
					//weight.pop_back();
					for (int i=0; i<maxidx; ++i)
					{
						if (wgtmatrix[maxidx][i]==1)
						{
							wgtmatrix[maxidx][i]=0;
							weight[i]--;

						}
					}
					for (int i=maxidx+1; i<weight.size(); ++i)
					{
						if (wgtmatrix[i][maxidx]==1)
						{
							wgtmatrix[i][maxidx]=0;
							weight[i]--;

						}
					}
					//??
				}
			} while (maxwgt>0);
			assert(vtp.size()<setr.size());
			//no commute //end
			int gi1,gi2,gj1,gj2;
			//no parallel //begin
			for (int i=0; i<setr.size(); ++i)
			{
				if (setr[i].first.chromIndex1==-1)
					continue;
				for (int j=i+1; j<setr.size(); ++j)
				{
					if (setr[j].first.chromIndex1==-1)
						continue;
					if (setr[i].first.cycnum1==setr[j].first.cycnum1 && setr[i].first.cycnum1!=-1 )//first
					{
						if(setr[i].first.grayIndex1<setr[i].first.grayIndex2){
							gi1=setr[i].first.grayIndex1;
							gi2=setr[i].first.grayIndex2;
						}
						else{
							gi1=setr[i].first.grayIndex2;
							gi2=setr[i].first.grayIndex1;
						}
						if(setr[j].first.grayIndex1<setr[j].first.grayIndex2){
							gj1=setr[j].first.grayIndex1;
							gj2=setr[j].first.grayIndex2;
						}
						else{
							gj1=setr[j].first.grayIndex2;
							gj2=setr[j].first.grayIndex1;
						}

						if (gi1==gj1 || gi1==gj2 || gi2==gj1 || gi2==gj2)//??
						{
							weight[i]++;weight[j]++;
							wgtmatrix[j][i]=1;
						}
						else
						if (gi1<gj1 && gj1<gi2 && gi2<gj2)
						{
							weight[i]++;weight[j]++;
							wgtmatrix[j][i]=1;
						}
						else if (gj1<gi1 && gi1<gj2 && gj2<gi2)
						{
							weight[i]++;weight[j]++;
							wgtmatrix[j][i]=1;
						}
						else
						if (setr[i].second.cycnum1==setr[j].second.cycnum1 && setr[i].second.cycnum1!=-1 )//second
						{
							if(setr[i].second.grayIndex1<setr[i].second.grayIndex2){
								gi1=setr[i].second.grayIndex1;
								gi2=setr[i].second.grayIndex2;
							}
							else{
								gi1=setr[i].second.grayIndex2;
								gi2=setr[i].second.grayIndex1;
							}
							if(setr[j].second.grayIndex1<setr[j].second.grayIndex2){
								gj1=setr[j].second.grayIndex1;
								gj2=setr[j].second.grayIndex2;
							}
							else{
								gj1=setr[j].second.grayIndex2;
								gj2=setr[j].second.grayIndex1;
							}

							if (gi1==gj1 || gi1==gj2 || gi2==gj1 || gi2==gj2)//??
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
							else
							if (gi1<gj1 && gj1<gi2 && gi2<gj2)
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
							else if (gj1<gi1 && gi1<gj2 && gj2<gi2)
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}

						}
						else if (setr[i].second.cycnum1!=setr[j].second.cycnum1 && (setr[i].second.cycnum1!=-1 || setr[j].second.cycnum1!=-1))
						{// no new leaf??

						}
						else //
						{
							weight[i]++;weight[j]++;
							wgtmatrix[j][i]=1;
						}
					}
					else if (setr[i].first.cycnum1!=setr[j].first.cycnum1 && (setr[i].first.cycnum1!=-1 || setr[j].first.cycnum1!=-1))//first
					{// no new leaf??
						if (setr[i].second.cycnum1==setr[j].second.cycnum1 && setr[i].second.cycnum1!=-1 )//second
						{
							if(setr[i].second.grayIndex1<setr[i].second.grayIndex2){
								gi1=setr[i].second.grayIndex1;
								gi2=setr[i].second.grayIndex2;
							}
							else{
								gi1=setr[i].second.grayIndex2;
								gi2=setr[i].second.grayIndex1;
							}
							if(setr[j].second.grayIndex1<setr[j].second.grayIndex2){
								gj1=setr[j].second.grayIndex1;
								gj2=setr[j].second.grayIndex2;
							}
							else{
								gj1=setr[j].second.grayIndex2;
								gj2=setr[j].second.grayIndex1;
							}

							if (gi1==gj1 || gi1==gj2 || gi2==gj1 || gi2==gj2)//??
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
							else
							if (gi1<gj1 && gj1<gi2 && gi2<gj2)
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
							else if (gj1<gi1 && gi1<gj2 && gj2<gi2)
							{
								weight[i]++;weight[j]++;
								wgtmatrix[j][i]=1;
							}
							
						}
						else if (setr[i].second.cycnum1!=setr[j].second.cycnum1 && (setr[i].second.cycnum1!=-1 || setr[j].second.cycnum1!=-1))//second
						{// no new leaf??

						}
						else //
						{
							weight[i]++;weight[j]++;
							wgtmatrix[j][i]=1;
						}
					}
					else
					{
						weight[i]++;weight[j]++;
						wgtmatrix[j][i]=1;
					}

				}
			}


			do 
			{
				maxwgt=0;
				maxidx=-1;
				for(int i=0; i<weight.size(); ++i){
					if (weight[i]>maxwgt)
					{
						maxwgt=weight[i];
						maxidx=i;
					}
				}
				if (maxwgt>0)
				{
					vtp.push_back(setr[maxidx]);
					setr[maxidx].first.chromIndex1=-1;//!!
					if (vtp.size()==setr.size()-1)
					{
						int i=0;
					}

					//setr.pop_back();
					weight[maxidx]=0;
					//weight.pop_back();
					for (int i=0; i<maxidx; ++i)
					{
						if (wgtmatrix[maxidx][i]==1)
						{
							wgtmatrix[maxidx][i]=0;
							weight[i]--;

						}
					}
					for (int i=maxidx+1; i<weight.size(); ++i)
					{
						if (wgtmatrix[i][maxidx]==1)
						{
							wgtmatrix[i][maxidx]=0;
							weight[i]--;

						}
					}
					//??
				}
			} while (maxwgt>0);
			//no parallel //end

			assert(vtp.size()<setr.size());
			for ( ritor=setr.begin(),ritor2=setr.end()-1;ritor<ritor2;)//
			{
				if (ritor->first.chromIndex1==-1)
				{
					while (ritor2->first.chromIndex1==-1)
						--ritor2;
					if (ritor<ritor2)
					{					
						(*ritor)=(*ritor2);//??
						ritor2->first.chromIndex1=-1;
					
						++ritor;
						--ritor2;
					}
					else
						break;
				}
				else
					++ritor;
			}
			//////out !!....
#ifdef _DEBUG

				os<<"\n"<<"?v?"<<v;	//fout	
				permut4=*v.label;
				renumber(permut4,*perm2);
				os<<"v2:"<<permut4;
				permut4=*v.label;
				renumber(permut4,*perm3);
				os<<"v3:"<<permut4;
			
			for ( ritor=setr.begin();ritor!=setr.end();++ritor)
			{
				if (ritor->first.chromIndex1==-1)
					break;
					os<<"?tp?"<<ritor->first<<ritor->second<<"\n";	//fout	

			}
#endif
			//////out !!....
			Vertex w;
			w.label=new Permutation(*v.label);//
			int chrom1,chrom2;
			int rnum=0;
			for ( ritor=setr.begin();ritor!=setr.end();++ritor)
			{
				if (ritor->first.chromIndex1==-1)
					break;
 
				//if (ritor->first.chromIndex1 == ritor->first.chromIndex2){
#ifdef _DEBUG
				 	os<<rnum<<":";/////////
				 	os<<"!ritor!"<<ritor->first<<"\n";///////////////
#endif
				//	continue;
				//}

				for ( ritor2=ritor+1;ritor2!=setr.end();++ritor2)
				{

					if (ritor2->first.chromIndex1==-1)
						break;
					//if (ritor2->first.chromIndex1 == ritor2->first.chromIndex2){
					//	os<<"!ritor2!"<<ritor2->first;///////////////
					//	continue;
					//}
					//
					chrom1=ritor2->first.chromIndex1;
					chrom2=ritor2->first.chromIndex2;
					if (ritor->first.chromIndex1==chrom1 )//1--1
					{
						if (ritor->first.geneIndex1 < ritor2->first.geneIndex1)//2
						{
							if (ritor->first.transStyle == PrefixPrefix)
							{
								ritor2->first.chromIndex1=ritor->first.chromIndex2;
								ritor2->first.geneIndex1=ritor2->first.geneIndex1+ritor->first.geneIndex2-ritor->first.geneIndex1;
							}
							else if (ritor->first.transStyle == PrefixSuffix)
							{
								ritor2->first.chromIndex1=ritor->first.chromIndex2;
								ritor2->first.geneIndex1=(*w.label).getLength(ritor->first.chromIndex1)-ritor2->first.geneIndex1-2;
								if (ritor2->first.transStyle == PrefixPrefix)
								{
									ritor2->first.transStyle = PrefixSuffix;
								}
								else
								{
									ritor2->first.transStyle = PrefixPrefix;
								}

							}
						}

					}
					else
						if (ritor->first.chromIndex1==chrom2 )//1--2
						{
							if (ritor->first.geneIndex1 < ritor2->first.geneIndex2)//2
							{
								if (ritor->first.transStyle == PrefixPrefix)
								{
									ritor2->first.chromIndex2=ritor->first.chromIndex2;
									ritor2->first.geneIndex2=ritor2->first.geneIndex2+ritor->first.geneIndex2-ritor->first.geneIndex1;
								}
								else if (ritor->first.transStyle == PrefixSuffix)
								{
									ritor2->first.chromIndex2=ritor->first.chromIndex2;
									ritor2->first.geneIndex2=(*w.label).getLength(ritor->first.chromIndex1)-ritor2->first.geneIndex2-2;
									if (ritor2->first.transStyle == PrefixPrefix)
									{
										ritor2->first.transStyle = PrefixSuffix;
									}
									else
									{
										ritor2->first.transStyle = PrefixPrefix;
									}

								}
							}
						}
					if (ritor->first.chromIndex2==chrom2 )//2--2
					{
						if (ritor->first.geneIndex2 > ritor2->first.geneIndex2)//3
						{
							if (ritor->first.transStyle == PrefixSuffix)
							{
								ritor2->first.chromIndex2=ritor->first.chromIndex1;
								ritor2->first.geneIndex2=ritor->first.geneIndex1+ritor->first.geneIndex2-ritor2->first.geneIndex2;
								if (ritor2->first.transStyle == PrefixPrefix)
								{
									ritor2->first.transStyle = PrefixSuffix;
								}
								else
								{
									ritor2->first.transStyle = PrefixPrefix;
								}
							}
						}
						else {//4
							if (ritor->first.transStyle == PrefixPrefix)
							{
								ritor2->first.chromIndex2=ritor->first.chromIndex1;
								ritor2->first.geneIndex2=ritor2->first.geneIndex2-ritor->first.geneIndex2+ritor->first.geneIndex1;

							}
							else
							{
								ritor2->first.geneIndex2=ritor2->first.geneIndex2-ritor->first.geneIndex2-ritor->first.geneIndex1+(*w.label).getLength(ritor->first.chromIndex1)-2;

							}
						}
					}
					else
					if (ritor->first.chromIndex2==chrom1 )//2--1
					{
						if (ritor->first.geneIndex2 > ritor2->first.geneIndex1)//3
						{
							if (ritor->first.transStyle == PrefixSuffix)
							{
								ritor2->first.chromIndex1=ritor->first.chromIndex1;
								ritor2->first.geneIndex1=ritor->first.geneIndex1+ritor->first.geneIndex2-ritor2->first.geneIndex1;
								if (ritor2->first.transStyle == PrefixPrefix)
								{
									ritor2->first.transStyle = PrefixSuffix;
								}
								else
								{
									ritor2->first.transStyle = PrefixPrefix;
								}
							}
						}
						else {//4
							if (ritor->first.transStyle == PrefixPrefix)
							{
								ritor2->first.chromIndex1=ritor->first.chromIndex1;
								ritor2->first.geneIndex1=ritor2->first.geneIndex1-ritor->first.geneIndex2+ritor->first.geneIndex1;

							}
							else
							{
								ritor2->first.geneIndex1=ritor2->first.geneIndex1-ritor->first.geneIndex2-ritor->first.geneIndex1+(*w.label).getLength(ritor->first.chromIndex1)-2;

							}
						}
					}
					//std::cout<<"-->"<<ritor2->first;///////////////
					assert (ritor2->first.chromIndex1 != ritor2->first.chromIndex2);
					if (ritor2->first.chromIndex1>ritor2->first.chromIndex2)
					{
						int tmp=ritor2->first.chromIndex1;
						ritor2->first.chromIndex1=ritor2->first.chromIndex2;
						ritor2->first.chromIndex2=tmp;
						tmp=ritor2->first.geneIndex1;
						ritor2->first.geneIndex1=ritor2->first.geneIndex2;
						ritor2->first.geneIndex2=tmp;

						//tmp=ritor2->first.cycnum1;
						//ritor2->first.cycnum1=ritor2->first.cycnum2;
						//ritor2->first.cycnum2=tmp;

						//tmp=ritor2->first.grayIndex1;
						//ritor2->first.grayIndex1=ritor2->first.grayIndex2;
						//ritor2->first.grayIndex2=tmp;

						//tmp=ritor2->second.cycnum1;
						//ritor2->second.cycnum1=ritor2->second.cycnum2;
						//ritor2->second.cycnum2=tmp;

						//tmp=ritor2->second.grayIndex1;
						//ritor2->second.grayIndex1=ritor2->second.grayIndex2;
						//ritor2->second.grayIndex2=tmp;

					}
				}//for
				//??

				w.label->doTranslocation(ritor->first);//??

				++rnum;

				/////check !!....
#ifdef _DEBUG
				permut4=*w.label;
				renumber(permut4,*perm1);
				int wd1=et12.getDistance(permut4);

				permut4=*w.label;
				renumber(permut4,*perm2);
				int wd2=et12.getDistance(permut4);
				os<<"v2:"<<permut4;
				permut4=*w.label;
				renumber(permut4,*perm3);
				int wd3=et12.getDistance(permut4);
				os<<"v3:"<<permut4;

				os<<"?w?"<<w<<wd1<<","<< wd2<<","<< wd3<<":"<<(wd1+wd2+wd3)<<"\n";	//fout	

				assert(wd1==v.d1+rnum);
				assert(wd2==v.d2-rnum);
				assert(wd3==v.d3-rnum);
#endif
				/////check !!....
			}//for ritor
			
			w.d1=v.d1+rnum;
			w.d2=v.d2-rnum;
			w.d3=v.d3-rnum;
			w.score=w.d1+w.d2+w.d3;
			///
			++hlold;

			if (wrt && ((hlold&0x000003FF)==0))
			{
				os<<"?hl?"<<hlold<<"\n";	//

				std::cout<<"?hl?"<<hlold<<std::endl;	//

			}		
			///
			if (w.score<bestsofar)
			{
				bestsofar=w.score;
				*medianperm=*w.label;
				md1=w.d1;
				md2=w.d2;
				md3=w.d3;
				medianscore=w.score;
				if (wrt)
				{
					os<<"?w?"<<w;
				}
			}
			if (w.score==mmin)
			{
				pmed=true;
				delete v.label;
				delete w.label;
				break;
			}
			if (w.score<v.score)
			{
				///check !!....
				permut4=*w.label;
				renumber(permut4,*perm1);
				int wd1=et12.getDistance(permut4);

				permut4=*w.label;
				renumber(permut4,*perm2);
				int wd2=et12.getDistance(permut4);

				permut4=*w.label;
				renumber(permut4,*perm3);
				int wd3=et12.getDistance(permut4);
				if (wrt)
				{
					os<<"?w?"<<w.d1<<","<< w.d2<<","<< w.d3<<":"<<w.score<<"==";
					os<<wd1<<","<< wd2<<","<< wd3<<":"<<(wd1+wd2+wd3)<<"\n";	//fout	
				}
				assert(wd1+wd2+wd3==w.score);
				///check !!....

				if (!vtp.empty())
				{
					v.other=vtp;
					soth.push(v);
				}
				else
				{
					delete v.label;
				}

				v=w;
				v.other.clear();

			}
			else
			{	
				delete v.label;
				delete w.label;
				v=soth.top();
				soth.pop();
				setr.clear();
				setr=v.other;
				if (wrt)
				{
					os<<"?back?"<<soth.size()<<"\n";	//fout	
				}
				goto backr;
			}
		}while (!soth.empty());
		while (!soth.empty())
		{

			v=soth.top();
			delete v.label;
			v.other.clear();
			soth.pop();
		}
		//marks.clear();
#ifdef _DEBUG_OUT
		std::cout<<"?HMedian?"<<medianscore<<":\n"<<*medianperm<<"\n";				
#endif
		//assert(soth.empty());
	}


	ostream& operator<<(ostream& os,const HMedian& mdn)
	{
		os<<"perm1:"<<*mdn.perm1;
		os<<"perm2:"<<*mdn.perm2;
		os<<"perm3:"<<*mdn.perm3;
		os<<"distance:"<<mdn.d12<<","<<mdn.d13<<","<<mdn.d23<<"\n";
		os<<"Hmedian:"<<*mdn.medianperm;
		os<<"Hmedian score:"<<mdn.medianscore<<"="<<mdn.md1<<"+"<<mdn.md2<<"+"<<mdn.md3<<"\n";
		return os;
	}
	void HMedian::printScore(ostream& os)
	{
		//os<<"distance:"<<d12<<","<<d13<<","<<d23<<"\n";
		//os<<"median:"<<*mdn.medianperm;
		os<<"Hmedian score:"<<medianscore<<"="<<md1<<"+"<<md2<<"+"<<md3<<"\n";
		
	}
}