#include "mem.h"
int main()
{   Memory Memo;
	int ELen,Len;
	int k=1,Ha,H,e=0;
	for(int i=0;i<10;i++,e++){
		if(i==e)ELen=Random();//产生随机数..
		else  e=i;
		H=Memo.OkorError(ELen);//调用判断函数...
		cout<< endl << "The " << i+1 << " process arrive(Len=" << ELen<<")....." << endl;
		if(H==(-1)){
			cout<<"Failure............Wait....."<<endl;
			cout<< "The process " << k << " end................"<< endl;
		    Ha=Memo.FineProcess(k);
		    Len=Memo.FineLen(k);
	    Memo.DelProcess(Ha,Len);
		cout<< "\t\t  MEMORY" << endl;
		Memo.PrintMemory();
		//cout<< "Any Key Stop........" << endl;//随便按任意键..程序暂停运行..
		Lost_Time(2);                 	
i--;  k++;
		}//if
	    else{
			cout<< "Success..................." << endl;
			Memo.GetProcess(H,i+1,ELen);
            cout<< "\t\t  MEMORY" << endl;
			Memo.PrintMemory();
            Lost_Time(2);
		}//else
		}//for
	return 0;
}//main()