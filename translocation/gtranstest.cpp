#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "transparam.hpp"
#include "ctimer.hpp"
#include "gpermutation.hpp"
#include "genumtrans.hpp"
#include "gbfenumtrans.hpp"
#include "genumtranstree.hpp"
//#include "transtest.hpp"
//#include "gtranstest.hpp"
//#include "pathcycle.hpp"
//#pragma comment(lib,"winmm.lib")
using std::cout;
using std::cin;

using std::ofstream;
using Utils::CTimer;
namespace GeneralTranslocation{

//inline
int gbfEnumTrans(GeneralPermutation& permut,GeneralPermutation& rpermut, double& usetime, bool wrt)
{

    int transnum=-1;

    //time_t starttime,endtime;
    //clock_t starttime,endtime;

    ofstream fout;
    if(wrt){
        fout.open("gbf.txt");
        fout<<permut;
    }
    //time(&starttime);
    //starttime=clock();
    ///timeBeginPeriod(1);
    ///DWORD starttime = timeGetTime();
    CTimer bftimer;
    bftimer.start();

    GeneralBFEnumTrans bfetrans;
    bfetrans.getDistance(permut,rpermut);
    transnum=bfetrans.findAllTrans(permut,rpermut, fout, wrt);
    //time(&endtime);
    //endtime=clock();
    //double dt=difftime(endtime,starttime);
    //usetime=(double)(endtime-starttime)/(double)CLOCKS_PER_SEC;//
    ///DWORD endtime = timeGetTime();
    ///timeEndPeriod(1);
    ///usetime=endtime-starttime;
    usetime=bftimer.getSecondes();

    if(wrt){
        fout.close();
    }
    ofstream ofs("gbfhist.txt",std::ios::app);
    ofs<<permut;
    ofs<<bfetrans;
    ofs<<usetime<<"s"<<"\n";
    ofs<<"==============================="<<"\n";
    ofs.close();

    return transnum;
}

//===============================//
//inline
int gtestEnumTrans(GeneralPermutation& permut, double& usetime, bool wrt)
{
    int transnum=-1;

    //time_t starttime,endtime;
    //clock_t starttime,endtime;
    ofstream fout;
    if(wrt){
        fout.open("ga.txt");
        fout<<permut;
    }
    //time(&starttime);
    //starttime=clock();
    ///timeBeginPeriod(1);
    ///DWORD starttime = timeGetTime();
    CTimer enumtimer;
    enumtimer.start();

    GeneralEnumTrans etrans;
    etrans.getDistance(permut);
    transnum=etrans.findAllTrans(permut);

    //time(&endtime);
    //endtime=clock();
    //usetime=(double)(endtime-starttime)/(double)CLOCKS_PER_SEC;

    ///DWORD endtime = timeGetTime();
    ///timeEndPeriod(1);
    ///usetime=endtime-starttime;
    usetime=enumtimer.getSecondes();
    if(wrt){
        etrans.generatePerms(fout,permut);

        fout<<etrans;
        fout.close();
    }

    ofstream ofs("ghist.txt",std::ios::app);
    ofs<<permut;
    ofs<<etrans;
    ofs<<usetime<<"s"<<"\n";
    ofs<<"==============================="<<"\n";
    ofs.close();

    return transnum;
}
//inline
void gtestEnumTransTree(GeneralPermutation& permut, double& usetime, bool wrt)
{

    time_t starttime,endtime;

    ofstream fout;
    if(wrt){
        fout.open("gatr.txt");
    }
    time(&starttime);
    GeneralEnumTransTree ettree;

    ettree.getAllTransSequence(permut,fout,wrt);
    time(&endtime);
    if(wrt){
        fout<<ettree;
        fout.close();
    }
    usetime=difftime(endtime,starttime);// !

    ofstream ofs("ghistr.txt",std::ios::app);
    ofs<<permut;
    ofs<<ettree;
    ofs<<usetime<<"s"<<"\n";
    ofs<<"==============================="<<"\n";
    ofs.close();

}

//inline
void gtestPermut(GeneralPermutation& permut, GeneralPermutation& rpermut,bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime)
{
    try{
        cout<<permut;
        //int enumnum=gtestEnumTrans(permut, enumtime, wrt);
        if(bf){
            int bfnum=gbfEnumTrans(permut, rpermut, bftime, wrt);
            //if(enumnum!=bfnum)
            //    throw(std::runtime_error("algorithm"));

            //cout<<bftime/enumtime<<"\n";
        }
        //if(tr)
            //gtestEnumTransTree(permut,trtime, wrt);
    } catch (std::exception& exp) {
	    cout<<"!gexception!";
		cout<<exp.what();
	}
	cout<<"!gcomplete!";
        char a;
        cin>>a;
}
//===============================//
 
}//namespace


