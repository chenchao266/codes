/*     �ڴ����ʵ�鱨��
��Ŀ����дһ��������ģ���ڴ�Ĺ���
һ�� �������                   
1. ��дһ��������ģ���ڴ�Ĺ���Ҫ�� 
.���û�����������������ý����㷨�� 
.����ϵͳ�ڴ�����Ϊ100KB�� 
.Ҫ�ܴ����ڴ���յ�ʱ�������ںϲ������⡣ 
.�ܹ��������µ����Σ� 
..��1��������ֵĽ���i����jKB�ڴ棬�������ж��Ƿ���
���䣬����ܷ��䣬Ҫ�ѷ�����׵�ַHandle�������Ļ
�ϡ����ܷ�������ַ�����NULL���� 
..��2���ͷ�������׵�ַΪHandle���ڴ�顣�ͷųɹ�����
Success�����򷵻�Failure 
..��3���ڴ���������Ҫ������ڴ�ʹ������Ϳ�������� 
�ڴ��������ĸ�ʽΪ�� 
Haddress�÷������׵�ַ
Eaddress�÷�����β��ַ
Len	�������� 
Process ���ʹ�ã�����ʹ�õĽ��̺ţ�����Ϊ0 */

#include <iostream.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#define ALL_LEN 100
 int Random();
 void Lost_Time(long tt);

///////////////////////////////////////////////////////


class Memory{//����һ���ڴ����...
private:
	int Eaddress,Haddress;//dfdfaafaf,,,,
	int Len,User;
	int a[ALL_LEN];//�����ALL_LEN���ȵ��ڴ�..������ֵ���ʹ�����Ľ���.
public:
	Memory(){
		Haddress=0;
		Eaddress=0;
		Len     =ALL_LEN;
		User    =0;
		for(int i=0;i<ALL_LEN;i++)a[i]=0;
	}
	int OkorError(int ELen);//�ж��Ƿ���Է���..���Ծͷ���ͷ��ַ..����-1
	int FineProcess(int EUser);//���ҽ���...�ҵ����ظý���ͷ��ַ...
	int FineLen(int EUser);//������̺�...�������ĳ���...
	void GetProcess(int EHaddress,int EUser,int ELen);//�ѽ��̷����ڴ�..
	void DelProcess(int EHaddress,int ELen);//���̴��ڴ���ɾ��....
	void PrintMemory();//����ڴ�״̬...
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


void Lost_Time(long tt)//��ʱ����...
{   time_t t;
	time_t k,g;
	g=k=time(&t);
    while((k=time(&t))-g<tt);//��ʱԼtime��
}//Lost_Time()
int Random()//����ÿ�����ж���ͬ�������
{
	time_t t;
	srand((unsigned)time(&t));//��������ӷ�����
	return rand()%20+20;//20-40�������
}//Random()