#include <iostream>
#include <stdexcept>
#include <ctime>
 
#include "transparam.hpp"
#include "ctimer.hpp"
#include "gpermutation.hpp"
#include "genumtrans.hpp"
#include "gbfenumtrans.hpp"
#include "genumtranstree.hpp"
#include "gtranstest.hpp"
using std::cout;
using std::cin;

 
namespace GeneralTranslocation{
 

inline
void gcreateRandomLeafPermut(GeneralPermutation& permut)
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
void gcreateRandomPermut(GeneralPermutation& permut, int m)
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
        permut.doGeneralTranslocation(tp);
    }

}

void grandomTest(bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime)
{

    Common::chromNumber=20;//2,3,4,5,6,7,8,9,10;
    Common::geneNumber=60;//20,30;40;50;60;70;80;90;100;
	int ln1=2;
    int chln1[]={10,10};//
	int perm1[]={1,2,3,4,5,6,7,8,9,10,
                 11,12,13,14,15,16,17,18,19,20,
                 //21,22,23,24,25,26,27,28,29,30
                 //,31,32,33,34,35,36,37,38,39,40
                 //,41,42,43,44,45,46,47,48,49,50
                 //,51,52,53,54,55,56,57,58,59,60
                 //,61,62,63,64,65,66,67,68,69,70
                 //,71,72,73,74,75,76,77,78,79,80
                 //,81,82,83,84,85,86,87,88,89,90
                 //,91,92,93,94,95,96,97,98,99,100
                 };
	int ln2=4;
    int chln2[]={5,5,5,5};//
	int perm2[]={1,2,3,4,5,
				6,7,8,9,10,
                 11,12,13,14,15,
				 16,17,18,19,20,
                 //21,22,23,24,25,26,27,28,29,30
                 //,31,32,33,34,35,36,37,38,39,40
                 //,41,42,43,44,45,46,47,48,49,50
                 //,51,52,53,54,55,56,57,58,59,60
                 //,61,62,63,64,65,66,67,68,69,70
                 //,71,72,73,74,75,76,77,78,79,80
                 //,81,82,83,84,85,86,87,88,89,90
                 //,91,92,93,94,95,96,97,98,99,100
                 };
	GeneralPermutation permut(perm1,20,chln1,ln1);//,perm2
	GeneralPermutation permut2(perm2,20,chln2,ln2);//,perm2
    //addCap(permut,permut2);

	srand((unsigned)time(NULL)) ;
	gcreateRandomLeafPermut(permut);
	gcreateRandomPermut(permut,Common::geneNumber);//Common::geneNumber * 1/5,2/5,3/5,4/5,5/5


	std::cout<<permut;
	    
	gtestPermut(permut,permut2,tr,wrt,bf,enumtime,bftime,trtime);
	

    vector<TransParam> trprms;
    GeneralEnumTrans etrans;
    etrans.getOneTransSequence(permut,trprms);
    etrans.generateOneTransSequence(cout,permut,trprms);

}
void grandomTest(bool tr,bool wrt,bool bf)
{
    double usetime,bfusetime,trusetime;
    grandomTest(tr,wrt,bf, usetime,bfusetime,trusetime);
}
void grandomEnumTestALL(int num)
{

    try{
        int i=10;
    //for(int i=4;i<=14;i+=2){//1
        Common::chromNumber=i;
        Common::geneNumber=i*3;
        int* chln=new int[Common::chromNumber];
        for(int j=0;j<Common::chromNumber;++j)
            chln[j]=10;
        int*  perm1=new int[Common::geneNumber];
        for(int j=0;j<Common::geneNumber;++j)
             perm1[j]=j+1;

        GeneralPermutation permut(perm1,10,chln,i);//??
		GeneralPermutation rperm=permut;
        int m=Common::geneNumber/5;//4
        srand((unsigned)time(NULL)) ;
        for(int rt=m; rt<=Common::geneNumber; rt+=m){
            double allustm=0.0;
            double allbfustm=0.0;
            double enumtime, bftime;

            for(int i=0; i<num; ++i){

                GeneralPermutation perm=permut;
                //createRandomLeafPermut(permut);
                gcreateRandomPermut(perm,rt);//rt
                int enumnum=gtestEnumTrans(perm, enumtime, false);
                int bfnum=gbfEnumTrans(perm, rperm, bftime, false);

                if(enumnum!=bfnum)
                    throw(std::runtime_error("algorithm"));
                //if(enumtime!=0)allustm+=bftime/enumtime;
                allustm+=enumtime;
                allbfustm+=bftime;

            }
            //std::cout<<allustm/num<<"\n";
            std::cout<<rt<<":";

            std::cout<<allbfustm<<"/"<<allustm<<"=";
            std::cout<<(allbfustm / allustm)<<"\n";
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

}//namespace


