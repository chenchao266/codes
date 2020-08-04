#include <iostream>
#include <fstream>
#include <ctime>
#include <stdexcept>
#include "permutation.hpp"
#include "enumtrans.hpp"
#include "bfenumtrans.hpp"
#include "enumtranstree.hpp"
#include "median.hpp"
#include "hmedian.hpp"
#include "transtest.hpp"
#include "ctimer.hpp"
using std::cout;
using std::cin;
using std::ofstream;
using Utils::CTimer;
namespace Translocation{


inline
void createRandomLeafPermut(Permutation& permut)
{

	for (int i=0; i<Common::chromNumber; i++)
    {
		

        int chi=int((double(rand())/RAND_MAX)*(Common::chromNumber-1));

        int lengthi=permut.getLength(chi);

        int begini=1+int((double(rand())/RAND_MAX)*(lengthi-1));

        int endi=begini-1+int((double(rand())/RAND_MAX)*(lengthi-1-begini));
        assert((begini>0)&&(endi<lengthi-1));
        permut.reverseChrom(chi,begini,endi);
    }

}

inline
void createRandomPermut(Permutation& permut, int m)
{

    for (int i=0; i<m; i++)
    {
        TransParam tp;

        int j=int((double(rand())/RAND_MAX)*(Common::chromNumber-1));

        int k=int((double(rand())/RAND_MAX)*(Common::chromNumber-1));

        if(j==k){
            j=j;//
            k=(j+1)%Common::chromNumber;//??
        }
        if(j>k) {
            int temp=j;
            j=k;
            k=temp;
        }
        tp.chromIndex1=j;//
        tp.chromIndex2=k;//
        int ln=permut.getLength(j)-2;
        tp.geneIndex1=int((double(rand())/RAND_MAX)*ln);
        assert((tp.geneIndex1>=0)&&(tp.geneIndex1<=ln));
        ln=permut.getLength(k)-2;
        tp.geneIndex2=int((double(rand())/RAND_MAX)*ln);
        assert((tp.geneIndex2>=0)&&(tp.geneIndex2<=ln));
        if(i%2==0)
            tp.transStyle=PrefixPrefix;
        else //if(i%2==1)
            tp.transStyle=PrefixSuffix;
        permut.doTranslocation(tp);
    }

}

void randomTest(bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime)
{
    Common::chromNumber=8;//2,3,4,5,6,7,8,9,10;
    Common::geneNumber=80;//20,30;40;50;60;70;80;90;100;
    int chln[]={10,10,10,10,10,10,10,10,10,10};//
	int perm1[]={ 1,2,3,4,5,6,7,8,9,10
                 ,11,12,13,14,15,16,17,18,19,20
                 ,21,22,23,24,25,26,27,28,29,30
                 ,31,32,33,34,35,36,37,38,39,40
                 ,41,42,43,44,45,46,47,48,49,50
                 ,51,52,53,54,55,56,57,58,59,60
                 ,61,62,63,64,65,66,67,68,69,70
                 ,71,72,73,74,75,76,77,78,79,80
                 ,81,82,83,84,85,86,87,88,89,90
                 ,91,92,93,94,95,96,97,98,99,100
                 };
	Permutation permut(perm1,chln);//,perm2
	srand((unsigned)time(NULL)) ;
	createRandomLeafPermut(permut);
	createRandomPermut(permut,Common::geneNumber);//Common::geneNumber * 1/5,2/5,3/5,4/5,5/5
    testPermut(permut,tr,wrt,bf,enumtime,bftime,trtime);
}
void randomTest(bool tr,bool wrt,bool bf)
{
    double usetime,bfusetime,trusetime;
    randomTest(tr,wrt,bf, usetime,bfusetime,trusetime);
}
void randomEnumTestALL(int num)
{
	
    ///0s = 0.015625s ?? 2 core !!
    try{
        int i=10;
    //for(int i=4;i<=14;i+=2){//1
        Common::chromNumber=i;
        Common::geneNumber=i*10;
        int* chln=new int[Common::chromNumber];
        for(int j=0;j<Common::chromNumber;++j)
            chln[j]=10;
        int*  perm1=new int[Common::geneNumber];
        for(int j=0;j<Common::geneNumber;++j)
             perm1[j]=j+1;
		
        Permutation permut(perm1,chln);//,perm2

        int m=Common::geneNumber/5;//4
        srand((unsigned)time(NULL)) ;
        for(int rt=m; rt<=Common::geneNumber; rt+=m){
            double allustm=0.0;
            double allbfustm=0.0;
			double allenum=0.0;
			double minenum=1000000.0;
			double maxenum=0.0;
            double enumtime, bftime;
			
            for(int i=0; i<num; ++i){
				
                Permutation perm=permut;
                //createRandomLeafPermut(permut);
                createRandomPermut(perm,rt);//rt
                int enumnum=testEnumTrans(perm, enumtime, false);
                //int bfnum=bfEnumTrans(perm, bftime, false);
				
                //if(enumnum!=bfnum)
                //    throw(std::runtime_error("algorithm"));
                //if(enumtime!=0)allustm+=bftime/enumtime;
                allustm+=enumtime;
                allbfustm+=bftime;
				allenum+=enumnum;
				if (enumnum<minenum)
				{
					minenum=enumnum;
				}
				if (enumnum>maxenum)
				{
					maxenum=enumnum;
				}
            }
            //std::cout<<allustm/num<<"\n";
            std::cout<<rt<<":";
			std::cout<<minenum<<"-"<<maxenum<<":"<<(allenum/num)<<"\n";
			//std::cout<<allbfustm<<"/"<<allustm<<"=";
            //std::cout<<(allbfustm / allustm)<<"\n";
        }
        delete [] chln;
        delete [] perm1;
        std::cout<<"====================="<<"\n";
    //}//for i
	} catch (std::exception& exp) {
	    cout<<"!exception!";
		cout<<exp.what();

	}
        char a;
        cin>>a;

}

void randomMedianTest(int num, int a,bool wrt)
{
	Common::chromNumber=10;//2,3,4,5,6,7,8,9,10;
	Common::geneNumber=100;//20,30;40;50;60;70;80;90;100;
	int chln[]={10,10,10,10,10,10,10,10,10,10};//
	//	int chln[]={20,20,20,20,20};//,10
	int perm1[]={ 1,2,3,4,5,6,7,8,9,10
		,11,12,13,14,15,16,17,18,19,20
		,21,22,23,24,25,26,27,28,29,30
		,31,32,33,34,35,36,37,38,39,40
		,41,42,43,44,45,46,47,48,49,50
		,51,52,53,54,55,56,57,58,59,60
		,61,62,63,64,65,66,67,68,69,70
		,71,72,73,74,75,76,77,78,79,80
		,81,82,83,84,85,86,87,88,89,90
		,91,92,93,94,95,96,97,98,99,100
	};
	Permutation permut(perm1,chln);//,perm2
	srand((unsigned)time(NULL)) ;
	//Permutation oldPerm=permut;
	ofstream ofs("median.txt",std::ios::app);
	ofstream fout;
	if(wrt){
		fout.open("mdn.txt");
	}
	CTimer mdntimer;
	double totaltime=0;	
	//createRandomLeafPermut(permut);
	for (int rn=5; rn<= 15 ; rn+=1)//50
	{	totaltime=0;	
	for (int i=0; i<num ; ++i)
	{

		Permutation permut1=permut;
		createRandomPermut(permut1,rn); 

		Permutation permut2=permut;
		createRandomPermut(permut2,rn); //rn+5
		Permutation permut3=permut;
		createRandomPermut(permut3,rn*a); //rn

		Median mdn(&permut1,&permut2,&permut3);

		mdntimer.start();

		mdn.computeMedian(fout, wrt);
		double usetime=mdntimer.getSecondes();
		totaltime+=usetime;
		if(wrt){
			fout.flush();

		}

		ofs<<mdn;
		ofs<<usetime<<"s"<<"\n";
		ofs<<"==============================="<<"\n";

	}
	totaltime=totaltime/num;
	ofs<<"average"<<a<<":"<<rn<<":"<<totaltime<<"\n";
	ofs<<"==============================="<<"\n";
	//testPermut(permut,tr,wrt,bf,enumtime,bftime,trtime);
	}
	if(wrt){
		fout.close();

	}
	ofs.close();
}

void randomMedianNMTest(int num, int a,bool wrt)
{
	Common::chromNumber=10;//2,3,4,5,6,7,8,9,10;
	Common::geneNumber=100;//20,30;40;50;60;70;80;90;100;
	int chln[]={10,10,10,10,10,10,10,10,10,10};//
	//	int chln[]={20,20,20,20,20};//,10
	int perm1[]={ 1,2,3,4,5,6,7,8,9,10
		,11,12,13,14,15,16,17,18,19,20
		,21,22,23,24,25,26,27,28,29,30
		,31,32,33,34,35,36,37,38,39,40
		,41,42,43,44,45,46,47,48,49,50
		,51,52,53,54,55,56,57,58,59,60
		,61,62,63,64,65,66,67,68,69,70
		,71,72,73,74,75,76,77,78,79,80
		,81,82,83,84,85,86,87,88,89,90
		,91,92,93,94,95,96,97,98,99,100
	};
	Permutation permut(perm1,chln);//,perm2
	srand((unsigned)time(NULL)) ;
	//Permutation oldPerm=permut;
	ofstream ofs("median.txt",std::ios::app);
	ofstream fout;
	if(wrt){
		fout.open("mdn.txt");
	}
	CTimer mdntimer;
	double totaltime=0;	
		//createRandomLeafPermut(permut);
	for (int rn=5; rn<= 5 ; rn+=1)//50
	{	totaltime=0;	
		for (int i=0; i<num ; ++i)
		{

			Permutation permut1=permut;
			createRandomPermut(permut1,rn); 

			Permutation permut2=permut;
			createRandomPermut(permut2,rn); //rn+5
			Permutation permut3=permut;
			createRandomPermut(permut3,rn*a); //rn

			Median mdn(&permut1,&permut2,&permut3);
 
			mdntimer.start();

			mdn.computeMedianNM(fout, wrt);
			double usetime=mdntimer.getSecondes();
			totaltime+=usetime;
			if(wrt){
				fout.flush();
				
			}

			ofs<<mdn;
			ofs<<usetime<<"s"<<"\n";
			ofs<<"==============================="<<"\n";

		}
		totaltime=totaltime/num;
		ofs<<"averageNM"<<a<<":"<<rn<<":"<<totaltime<<"\n";
		ofs<<"==============================="<<"\n";
		//testPermut(permut,tr,wrt,bf,enumtime,bftime,trtime);
	}
	if(wrt){
		fout.close();

	}
	ofs.close();
}

void randomHMedianTest(int num,int a, bool wrt)
{
	Common::chromNumber=10;//2,3,4,5,6,7,8,9,10;
	Common::geneNumber=100;//20,30;40;50;60;70;80;90;100;
	int chln[]={10,10,10,10,10,10,10,10,10,10};//
	int perm1[]={ 1,2,3,4,5,6,7,8,9,10
		,11,12,13,14,15,16,17,18,19,20
		,21,22,23,24,25,26,27,28,29,30
		,31,32,33,34,35,36,37,38,39,40
		,41,42,43,44,45,46,47,48,49,50
		,51,52,53,54,55,56,57,58,59,60
		,61,62,63,64,65,66,67,68,69,70
		,71,72,73,74,75,76,77,78,79,80
		,81,82,83,84,85,86,87,88,89,90
		,91,92,93,94,95,96,97,98,99,100
	};
	Permutation permut(perm1,chln);//,perm2
		srand((unsigned)time(NULL)) ;
	//Permutation oldPerm=permut;
	ofstream ofs("hmedian.txt",std::ios::app);
	ofstream fout;
	if(wrt){
		fout.open("hmdn.txt");
	}
	CTimer mdntimer;
		//createRandomLeafPermut(permut);
	for (int rn=14; rn<=  15 ; rn+=1)//50
	{		double totaltime=0;	
		for (int i=0; i<num ; ++i)
		{

			Permutation permut1=permut;
			createRandomPermut(permut1,rn); //rn

			Permutation permut2=permut;
			createRandomPermut(permut2,rn); //rn+5
			Permutation permut3=permut;
			createRandomPermut(permut3,rn*a); //rn

			HMedian mdn(&permut1,&permut2,&permut3);


			mdntimer.start();

			mdn.computeMedian(fout, wrt);
			double usetime=mdntimer.getSecondes();
				totaltime+=usetime;
			if(wrt){
				fout.flush();

			}


			ofs<<mdn;
			ofs<<usetime<<"s"<<"\n";
			ofs<<"==============================="<<"\n";
		
		}
		 
		totaltime=totaltime/num;
		ofs<<"average"<<a<<":"<<rn<<":"<<totaltime<<"\n";
		ofs<<"==============================="<<"\n";
	}		

	if(wrt){
		fout.close();
	}
	ofs.close();
}

void randomMHTest(int num,int a, bool wrt)
{
	Common::chromNumber=10;//2,3,4,5,6,7,8,9,10;
	Common::geneNumber=100;//20,30;40;50;60;70;80;90;100;
	int chln[]={10,10,10,10,10,10,10,10,10,10};//
	int perm1[]={ 1,2,3,4,5,6,7,8,9,10
		,11,12,13,14,15,16,17,18,19,20
		,21,22,23,24,25,26,27,28,29,30
		,31,32,33,34,35,36,37,38,39,40
		,41,42,43,44,45,46,47,48,49,50
		,51,52,53,54,55,56,57,58,59,60
		,61,62,63,64,65,66,67,68,69,70
		,71,72,73,74,75,76,77,78,79,80
		,81,82,83,84,85,86,87,88,89,90
		,91,92,93,94,95,96,97,98,99,100
	};
	Permutation permut(perm1,chln);//,perm2
	srand((unsigned)time(NULL)) ;
	//Permutation oldPerm=permut;
	ofstream ofs("mhmedian.txt",std::ios::app);
	ofstream fout;
	if(wrt){
		fout.open("mhmdn.txt");
	}
	CTimer mdntimer;
	//createRandomLeafPermut(permut);
	for (int rn=12; rn<=  15 ; rn+=1)//50
	{	double totaltime=0;	
		double totalhtime=0;	
		double usetime=0;
		double usehtime=0;
		for (int i=0; i<num ; ++i)
		{

		Permutation permut1=permut;
		createRandomPermut(permut1,rn); //rn

		Permutation permut2=permut;
		createRandomPermut(permut2,rn); //rn+5
		Permutation permut3=permut;
		createRandomPermut(permut3,rn*a); //rn

		Median mdn(&permut1,&permut2,&permut3);
		HMedian hmdn(&permut1,&permut2,&permut3);

		mdntimer.start();
		mdn.computeMedianNM(fout, wrt);
		usetime=mdntimer.getSecondes();
		totaltime+=usetime;


		mdntimer.start();
		hmdn.computeMedian(fout, wrt);
		usehtime=mdntimer.getSecondes();
		totalhtime+=usehtime;

		if(wrt){
			fout.flush();

		}


		ofs<<mdn;
		hmdn.printScore(ofs);
		ofs<<usetime<<"s"<<"--"<<usehtime<<"s"<<"\n";
		ofs<<"==============================="<<"\n";

	}

	totaltime=totaltime/num;
	totalhtime=totalhtime/num;
	double bd=totaltime/totalhtime;
	ofs<<"average"<<a<<":"<<rn<<":"<<totaltime<<":"<<totalhtime<<"="<<bd<<"\n";
	ofs<<"==============================="<<"\n";
	}		

	if(wrt){
		fout.close();
	}
	ofs.close();
}
}//namespace

