#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <cassert>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include "ctimer.hpp"
#include "transparam.hpp"
#include "permutation.hpp"
#include "enumtrans.hpp"
#include "bfenumtrans.hpp"
#include "enumtranstree.hpp"
#include "transtest.hpp"
//#pragma comment(lib,"winmm.lib")
using std::cout;
using std::cin;

using std::ofstream;
using Utils::CTimer;
namespace Translocation{

//inline
int bfEnumTrans(Permutation& permut, double& usetime, bool wrt)
{

    int transnum=-1;

    //time_t starttime,endtime;
    //clock_t starttime,endtime;

    ofstream fout;
    if(wrt){
        fout.open("bf.txt");
        fout<<permut;
    }
    //time(&starttime);
    //starttime=clock();
    ///timeBeginPeriod(1);
    ///DWORD starttime = timeGetTime();
    CTimer bftimer;
    bftimer.start();

    BFEnumTrans bfetrans;
    bfetrans.getDistance(permut);
    transnum=bfetrans.findAllTrans(permut, fout, wrt);
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
    ofstream ofs("bfhist.txt",std::ios::app);
    ofs<<permut;
    ofs<<bfetrans;
    ofs<<usetime<<"s"<<"\n";
    ofs<<"==============================="<<"\n";
    ofs.close();

    return transnum;
}

//===============================//
//inline
int testEnumTrans(Permutation& permut, double& usetime, bool wrt)
{
    int transnum=-1;

    //time_t starttime,endtime;
    //clock_t starttime,endtime;
    ofstream fout;
    if(wrt){
        fout.open("a.txt");
        fout<<permut;
    }
    //time(&starttime);
    //starttime=clock();
    ///timeBeginPeriod(1);
    ///DWORD starttime = timeGetTime();
    CTimer enumtimer;
    enumtimer.start();

    EnumTrans etrans;
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

    ofstream ofs("hist.txt",std::ios::app);
    ofs<<permut;
    ofs<<etrans;
    ofs<<usetime<<"s"<<"\n";
    ofs<<"==============================="<<"\n";
    ofs.close();

    return transnum;
}
//inline
void testEnumTransTree(Permutation& permut, double& usetime, bool wrt)
{

    time_t starttime,endtime;

    ofstream fout;
    if(wrt){
        fout.open("atr.txt");
    }
    time(&starttime);
    EnumTransTree ettree;

    ettree.getAllTransSequence(permut,fout,wrt);
    time(&endtime);
    if(wrt){
        fout<<ettree;
        fout.close();
    }
    usetime=difftime(endtime,starttime);// !

    ofstream ofs("histr.txt",std::ios::app);
    ofs<<permut;
    ofs<<ettree;
    ofs<<usetime<<"s"<<"\n";
    ofs<<"==============================="<<"\n";
    ofs.close();

}

//inline
void testPermut(Permutation& permut,bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime)
{
    try{
        cout<<permut;
        int enumnum=testEnumTrans(permut, enumtime, wrt);
        if(bf){
            int bfnum=bfEnumTrans(permut, bftime, wrt);
            if(enumnum!=bfnum)
                throw(std::runtime_error("algorithm"));

            cout<<bftime/enumtime<<"\n";
        }
        if(tr)
            testEnumTransTree(permut,trtime, wrt);
    } catch (std::exception& exp) {
	    cout<<"!exception!";
		cout<<exp.what();
		
	}
	cout<<"!complete!";
        char a;
        cin>>a;
}
//===============================//

}//namespace

