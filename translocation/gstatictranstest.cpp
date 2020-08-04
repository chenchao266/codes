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
 
void gstaticTest(bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime)
{

    Common::chromNumber=12;
    Common::geneNumber=36;
    int chln1[]={8,4};
	int perm1[]={1,-2,3,8,-9,10,-11,12, 5,-6,7,4};
    int chln2[]={12};
	int perm2[]={1,2,3,4,5,6,7,8, 9,10,11,12};

    GeneralPermutation permut1(perm1,12,chln1,2);
    GeneralPermutation permut2(perm2,12,chln2,1);
	
    gtestPermut(permut1, permut2, tr,wrt,bf,enumtime,bftime,trtime);

}
void gstaticTest(bool tr,bool wrt,bool bf)
{
    double usetime,bfusetime,trusetime;
    gstaticTest(tr,wrt,bf,usetime,bfusetime,trusetime);

}
//===============================//
void gstaticTest2(bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime)
{

    Common::chromNumber=10;
    Common::geneNumber=30;
    int chln1[]={4,6};//
	//5,2,2
    //1-123ª˘“Ú,124-163º”√±


    int perm1[]=
	/*{4,-1,-3,-2,5,
	6,7,
	8,9
	};*/
    { 2,1,3,5,
     6,8,-7,4,10,-9,

    };

	//6,8,2,2,2,2,2,2,2,2

	//Human
    //11,8,8,7,11,5,4,8,7,11,17,4,12,4,4,6,3,5,20,8
/*  {-129,1,2,3,4,5,6,7,8,9,131,
    130,10,11,12,13,14,15,-124,
    -141,16,17,18,19,20,21,-128,
    148,22,23,24,25,26,163,
    162,27,28,29,30,31,32,33,34,35,-142,
    132,36,37,38,133,
    138,39,40,139,
    144,41,42,43,44,45,46,155,
    126,47,48,49,50,51,149,
    -143,52,53,54,55,56,57,58,59,60,-140,
    134,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,161,
    -125,76,77,-150,
    -147,78,79,80,81,82,83,84,85,86,87,145,
    152,88,89,153,
    136,90,91,137,
    154,92,93,94,95,127,
    160,96,151,
    146,97,98,99,135,
    -157,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,-156,
    158,118,119,120,121,122,123,159};
*/

    //Mouse
    //10,16,9,7,5,10,9,4,10,7,14,5,10,7,4,4,20,8,2,2
    int perm2[]=
	/*{4,1,2,3,5,
	6,7,
	8,9
	};*/
	{ 1,2,3,4,
     5,6,7,
     8,9,10

    };
	int chln2[]={4,3,3};//
	//6,5,5,2,2,2,2,2,2,2
/*  {124,12,13,14,15,-9,-8,-7,-6,125,
    126,47,48,-46,-45,-44,-11,-10,-58,-57,-56,92,93,-95,-94,127,
    128,-21,-20,-5,-4,-3,-2,-1,129,
    130,34,35,41,42,43,131,
    132,36,37,38,133,
    134,-64,-63,61,62,65,66,67,68,135,
    136,90,91,-55,-54,51,52,53,137,
    138,39,40,139,
    140,-60,-59,-77,-76,-19,-18,16,17,141,
    142,-97,-96,-75,-74,-73,143,
    144,24,25,78,79,-83,-82,-81,-80,84,85,86,87,145,
    146,-28,-27,-26,147,
    148,22,23,98,99,69,70,-72,-71,149,
    150,-33,-32,-31,-30,-29,151,
    152,88,89,153,
    154,-50,-49,155,
    156,-105,-104,106,107,108,114,115,-117,-116,-103,-102,109,110,111,112,113,-101,-100,157,
    158,118,119,120,121,122,123,159,
    160,161,
    162,163};
*/
    GeneralPermutation permut1(perm1,10,chln1,2);
	GeneralPermutation permut=permut1;
    GeneralPermutation permut2(perm2,10,chln2,3);

	//addCap(permut1,permut2);
	std::cout<<permut1;
	gtestPermut(permut1, permut2, tr,wrt,bf,enumtime,bftime,trtime);
	

    vector<TransParam> trprms;
    GeneralEnumTrans etrans;
    etrans.getOneTransSequence(permut,trprms);
    etrans.generateOneTransSequence(cout,permut,trprms);

    //gtestPermut(permut,tr,wrt,bf,enumtime,bftime,trtime);
}
void gstaticTest2(bool tr,bool wrt,bool bf)
{
    double usetime,bfusetime,trusetime;
    gstaticTest2(tr,wrt,bf,usetime,bfusetime,trusetime);
}
//===============================//
 
}//namespace


