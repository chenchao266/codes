#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "transparam.hpp"
#include "permutation.hpp"
#include "enumtrans.hpp"
#include "bfenumtrans.hpp"
#include "enumtranstree.hpp"
#include "median.hpp"
#include "hmedian.hpp"
#include "transtest.hpp"
#include "ctimer.hpp"
//#pragma comment(lib,"winmm.lib")
using std::cout;
using std::cin;
using std::ifstream ;
using std::ofstream ;
using std::string; 
using std::vector; 
using Utils::CTimer;

namespace Translocation{

inline
void getInputPermut(vector<int>& genes, vector<int>& chromln)//int*, int*
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
void inputTest(bool tr,bool wrt,bool bf,double& enumtime,double& bftime,double& trtime)
{
    //int* genes;
    //int* chromln;
    vector<int> genes;
    vector<int> chromln;
    getInputPermut(genes,chromln);

    //(genes,chromln)
	Permutation permut(&genes[0],&chromln[0]);//,perm2

    testPermut(permut,tr,wrt,bf,enumtime,bftime,trtime);

    //delete [] chromln;
    //delete [] genes;
}

void inputTest(bool tr,bool wrt,bool bf)
{
    double usetime,bfusetime,trusetime;
    inputTest(tr,wrt,bf,usetime,bfusetime,trusetime);
}
//===============================//
///read from file??
void inputFileTest(const char * filename, bool wrt )
{
	Common::chromNumber=10;
	Common::geneNumber=100;
	Permutation* perms[3];
	ofstream fout;
	if(wrt){
		fout.open("ihmdn.txt");
	}
	ofstream ofs("ihmedian.txt",std::ios::app);

	ifstream ifs(filename);
	CTimer mdntimer;
	string line;
	// error in stream test
	vector<int > genes;
	genes.reserve(100);
	vector<int > lengths;
	lengths.reserve(10);
	std::stringstream strm;
	int i=0;
	while(!ifs.eof()) {
		
		std::getline(ifs,line);
		//std::cout << "{ " << line << " }" << "\n";
		string::size_type fc=line.find_first_of('(');
		if (fc!=string::npos)
		{
			if (i==-1)
			{
				i=0;
				continue;
			}

		while (fc!=string::npos)
		{
			string::size_type fce=line.find_first_of(')',fc);
			string::size_type oldfg=fc+1;
			string::size_type fg=line.find_first_of(',',fc);
			int chln=0;
			int gene=0;
			while (fg<fce)
			{
				strm<<line.substr(oldfg,fg-oldfg);
				strm>>gene;
				genes.push_back(gene);
				strm.clear();
				++chln;
				oldfg=fg+1;
				fg=line.find_first_of(',',oldfg);
				
			}
			fg=fce;
			strm<<line.substr(oldfg,fg-oldfg);
			strm>>gene;
			genes.push_back(gene);
			++chln;
			strm.clear();
			lengths.push_back(chln);
			fc=line.find_first_of('(',fce);
			

		}
			perms[i]=new Permutation(&genes[0],&lengths[0]);
			++i;
			genes.clear();
			genes.resize(0);
			lengths.clear();
			lengths.resize(0);
		}

			
	
		if (i==3)
		{
			HMedian hmdn(perms[0], perms[1], perms[2]);



			mdntimer.start();

			hmdn.computeMedian(fout, wrt);
			double usetime=mdntimer.getSecondes();
			ofs<<hmdn;
			ofs<<usetime<<"s"<<"\n";
			ofs<<"==============================="<<"\n";

			//i=0;
			delete perms[0];
			delete perms[1];
			delete perms[2];
			i=-1;
		}
	
	}

	if(wrt){
		fout.close();
	}

	ofs.close();
}
}//namespace

