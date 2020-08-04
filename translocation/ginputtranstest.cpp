#include <iostream>
 
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
void ggetInputPermut(vector<int>& genes, vector<int>& chromln)//int*, int*
{
    //int length;
    int genenum;
    int chromnum;

    cout<<"gene number=";
    cin>>genenum;
    assert(genenum>=4);
    //genes=new int[genenum];

    genes.reserve(genenum);
    cout<<"chromosome number=";
    cin>>chromnum;
    assert((chromnum<genenum)&&(chromnum>1));

    //chromln=new int[chromnum];

    chromln.reserve(chromnum);
    Common::chromNumber=chromnum;
    Common::geneNumber=genenum;

    cin.clear();
    cin.sync();
    int j=0;
    for(int i=0; i<chromnum; ++i){

        cout<<"chromosome "<<i;//<<" length = "
        //cin>>chromln[i];
        cout<<" genes : ";
        int agene;
        int k=0;
        while(cin>>agene){
            genes[j]=agene;//genes.push_back(agene);
            ++j;
            ++k;
            if(j==genenum)
                break;
        }
        chromln[i]=k;
        cin.clear();
        cin.sync();
        //cin.ignore( numeric_limits <streamsize>::max(),'\n');
    }

}
void ginputTest(bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime)
{
    
    //int* genes;
    //int* chromln;
    vector<int> genes;
    vector<int> chromln;
    ggetInputPermut(genes,chromln);

    //(genes,chromln)
	GeneralPermutation permut(&genes[0],genes.size(),&chromln[0],chromln.size());//,perm2
	GeneralPermutation permut2=permut;//??
    gtestPermut(permut, permut2, tr,wrt,bf,enumtime,bftime,trtime);

    //delete [] chromln;
    //delete [] genes;
}

void ginputTest(bool tr,bool wrt,bool bf)
{
    double usetime,bfusetime,trusetime;
    ginputTest(tr,wrt,bf,usetime,bfusetime,trusetime);
}
//===============================//
///read from file??
 
}//namespace


