/*     内存管理实验报告
题目：编写一个程序来模拟内存的管理
一． 需求分析                   
1. 编写一个程序来模拟内存的管理，要求： 
.采用活动分区方案，但不采用紧凑算法。 
.假设系统内存容量为100KB。 
.要能处理内存回收的时候上下邻合并的问题。 
.能够处理以下的情形： 
..（1）随机出现的进程i申请jKB内存，程序能判断是否能
分配，如果能分配，要把分配的首地址Handle输出到屏幕
上。不能分配输出字符串“NULL”。 
..（2）释放随机的首地址为Handle的内存块。释放成功返回
Success，否则返回Failure 
..（3）内存情况输出，要求输出内存使用情况和空闲情况。 
内存情况输出的格式为： 
Haddress该分区的首地址
Eaddress该分区的尾地址
Len	分区长度 
Process 如果使用，则是使用的进程号，否则为0 */

#include <iostream.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#define ALL_LEN 100
 int Random();
 void Lost_Time(long tt);

///////////////////////////////////////////////////////


class Memory{//定义一个内存的类...
private:
	int Eaddress,Haddress;//dfdfaafaf,,,,
	int Len,User;
	int a[ALL_LEN];//虚拟的ALL_LEN长度的内存..数组数值存放使用它的进程.
public:
	Memory(){
		Haddress=0;
		Eaddress=0;
		Len     =ALL_LEN;
		User    =0;
		for(int i=0;i<ALL_LEN;i++)a[i]=0;
	}
	int OkorError(int ELen);//判断是否可以分配..可以就返回头地址..否则返-1
	int FineProcess(int EUser);//查找进程...找到返回该进和头地址...
	int FineLen(int EUser);//输入进程号...返回它的长度...
	void GetProcess(int EHaddress,int EUser,int ELen);//把进程放入内存..
	void DelProcess(int EHaddress,int ELen);//进程从内存中删除....
	void PrintMemory();//输出内存状态...
};



int Memory::OkorError(int ELen)
{   Len=ELen;
	int k=0,pos;
	int *p;  p=a;
	for(int i=0;i<ALL_LEN;i++,p++){
		while(*p!=0){
			if(i>1){ if(*(p-1)==0)k=0;}//if
			break;
		}//while
		if(*p!=0)continue;
		if(*p==0&&k==0)pos=i;  k++;
		if(k==Len)return pos;
	}//for
	return (-1);
}//OkorError()
////////////////////////////////////////////////////////////
int Memory::FineProcess(int EUser)
{   User=EUser;
	for(int i=0;i<ALL_LEN;i++)if(a[i]==User)break;
	return i;
}//FineProcess()
////////////////////////////////////////////////////////////
int Memory::FineLen(int EUser)
{   User =EUser;
	int k=0;
	for(int i=0;i<ALL_LEN;i++)if(a[i]==User)k++;
	return k;
}//FineLen()
////////////////////////////////////////////////////////////
void Memory::GetProcess(int EHaddress,int EUser,int ELen)
{   Haddress=EHaddress;
	User    =EUser;
	Len     =ELen;
	for(int i=Haddress;i<Len+Haddress;i++)a[i]=User;
}//GetProcess()
////////////////////////////////////////////////////////////
void Memory::DelProcess(int EHaddress,int ELen)
{   Haddress=EHaddress;
	Len     =ELen;
	for(int i=Haddress;i<Len+Haddress;i++)a[i]=0;
}//DelProcess()
///////////////////////////////////////////////////////////
void Memory::PrintMemory()
{   int *p;
	int H=-1,E=0;
	p=&a[1];
	cout<<"Heard Address\t"<<"End Address\t"<<"Len\t"<<"Ueser"<<endl;
	for(int i=0;i<ALL_LEN;i++,p++){
		E=i;
		if(*p!=*(p-1)){ cout<<H+1<<"\t\t"<<E<<"\t\t"<<E-H<<"\t"<<*(p-1)<<endl;
			H=i;
		}//if
	}//for
}//PrintMemory()


void Lost_Time(long tt)//延时函数...
{   time_t t;
	time_t k,g;
	g=k=time(&t);
    while((k=time(&t))-g<tt);//延时约time秒
}//Lost_Time()
int Random()//产生每次运行都不同的随机数
{
	time_t t;
	srand((unsigned)time(&t));//随机数种子发生器
	return rand()%20+20;//20-40的随机数
}//Random()