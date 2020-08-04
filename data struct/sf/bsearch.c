/*2. Դ����*/
/***binary.c���ֲ��ҳ���***/
#include <graphics.h>
#include <stdlib.h>
#define N 15  /*�������Ϊ15*/
void find(int x[],int y,int z);/*������ҹ���*/
void Init(int x[],int *n);/*ͼ�γ�ʼ��*/
void Close();/*ͼ�ιر�*/
void Put(int x[],int y);/*�����ֵ*/
void Up(int x);/*���ϼ�*/
void Down(int x);/*���¼�*/
void Mid(int x);/*���м��*/
void Clr(int x);/*���������ϵ�һЩ����*/
void Sort(int x[],int n);/*�������������*/
void main(void)
{
   int a[N],x;
   Init(a,&x);/*xΪҪ���ҵ���*/
   Put(a,N);
   find(a,x,N);
   Close();
   exit(0);
}
void Mid(int n)/*���м��*/
{
   setcolor(WHITE);/*�м������ɫΪ��ɫ,���������߻����˼�ͷ,������������һ��*/
   line(25+n*40,120,25+n*40,80);
   line(25+n*40,120,20+n*40,110);
   line(25+n*40,120,30+n*40,110);
}
void Down(int n)/*���ϼ�*/
{
   setcolor(6);
   line(25+n*40,120,25+n*40,80);
   line(25+n*40,120,20+n*40,110);
   line(25+n*40,120,30+n*40,110);
}
void Up(int n)/*���¼�*/
{
   setcolor(6);
   line(25+n*40,180,25+n*40,220);
   line(25+n*40,180,20+n*40,190);
   line(25+n*40,180,30+n*40,190);
}
void Clr(int y)/*���������ϵ�һЩ����*/
{
   setfillstyle(SOLID_FILL,0);/*ÿ����ʾ��ʱ���Ƚ�������ʾ�����ָ�ȥ����*/
   bar(0,y+50,640,y-50);/*�������þ��εķ�ʽ*/
}
void Put(int a[],int n)/*�����ֵ*/
{
   int i;
   char num[5];
   setcolor(GREEN);
   settextstyle(0,0,2);/*��������Ĵ�С*/
   for(i=0;i<n;i++)
   {
      sprintf(num,"%d",a[i]);/*������ת�����ַ������*/
      outtextxy(20+i*40,150,num);
   }
   settextstyle(0,0,1);
   setcolor(BLUE);
   outtextxy(250,250,"anykey to continue");
   getch();
}
void find(int a[],int y,int n)/*����Ĳ���*/
{
   int low,high,mid,i;
   char str1[5],str2[5];
   sprintf(str1,"%d",y);
   low=0;
   high=n-1;
   setcolor(RED);
   settextstyle(0,0,2);
   outtextxy(200,10,"FIND");
   outtextxy(330,10,str1);
   while(low<=high)
   {
      Clr(250);
      Clr(80);
      Clr(230);/*��������Clr��Ϊ�˰���Ļ�ϵļ�ͷ������ɾ��*/
      mid=(high+low)/2;/*�����м�λ��*/
      Up(high); /*��ʾ�ϱ߼�ͷ*/
      Down(low); /*��ʾ�ұ߼�ͷ*/
      Mid(mid);/*������������ͷ��ʼ����*/
      if(a[mid]==y) /*����ҵ�����ѭ��*/
	 break;
      if(a[mid]<y)
      {
	 low=mid+1;  /*�޸���߽�*/
	 sprintf(str2,"%d",a[mid]);
	 outtextxy(250,250,str2); /*��ʾ�Ƚ����ݵ����*/
	 outtextxy(300,250,"<");
	 outtextxy(350,250,str1);
      }
      else
      {
	 high=mid-1;/*�޸��ұ߽�*/
	 sprintf(str2,"%d",a[mid]);
	 outtextxy(250,250,str2); /*��ʾ�Ƚ����ݵ����*/
	 outtextxy(300,250,">");
	 outtextxy(350,250,str1);
      }
      sleep(2);/*���һ���ִ����һ��*/
   }
   setcolor(11);
   if(low<=high)/*�ҵ�*/
      outtextxy(250,350,"FIND");
   else/*û�ҵ�*/
      outtextxy(250,350,"NOT FIND");
}
void Sort(int a[],int n)/*ð���������϶���*/
{
   int i,j,t;
   for(i=0;i<n-1;i++)
      for(j=0;j<n-1-i;j++)
	 if(a[j]>a[j+1])
	 {
	    t=a[j];
	    a[j]=a[j+1];
	    a[j+1]=t;
	 }  /*���ݽ���*/
}
void Init(int a[],int *x)/*ͼ������*/
{
   int i;
   int gd=DETECT,gm;
   randomize();/*�������*/
   for(i=0;i<N;i++)/*�����������*/
      a[i]=random(100); /*����100���ڵ���*/
   Sort(a,N);/*����*/
   for(i=0;i<N;i++)
      printf("%-3d",a[i]); /*�������*/
   printf("\n");
   printf("please input a num you want to find: ");/*����Ҫ�ҵ���*/
   scanf("%d",x);
   initgraph(&gd,&gm,"c:\\tc");/*tc����Ŀ¼��·��*/
   cleardevice();
}
void Close()/*ͼ����������*/
{
   getch();
   closegraph(); /*�ر�ͼ��ϵͳ*/
}
