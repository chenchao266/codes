/*2. Դ����*/
/***bubble.c***/
#include <graphics.h>
# include <dos.h>
#define N 8   /*������������Ϊ8��*/
void Init(int a[]);/*��ʼ״̬*/
void Close(void);/*ͼ��ģʽ����*/
void Pr(int a[],int n);/*�������*/
void DrawChange(int i,int j);/*��������ͷ*/
void Bubble_sort(int a[],int n);/*ð������*/
void main(void)
{
   int i,a[N];
   printf("Please enter 8 int number\n"); /*����8������*/
   for(i=0;i<N;i++)
   scanf("%d",&a[i]);
   Init(a);/*��ʼ״̬*/
   Bubble_sort(a,8);/*��������*/
   Close();/*ͼ��ģʽ����*/
   exit(0);
}
void Init(int a[])/*��ʼ״̬*/
{
   int gd=DETECT,gm,i;
   initgraph(&gd,&gm,"");/*��ʼ��ͼ��ϵͳ*/
   cleardevice();  /*����*/
   setcolor(YELLOW);  /*������ɫ*/
   outtextxy(210,10,"any key to continue");  /*��ʾѹ���������*/
   Pr(a,40);/*��һ���������*/
   getch();  /*�ȴ�ѹ�����*/
}
void Close (void) /*ͼ��ģʽ����*/
{
   getch();  /*�ȴ�ѹ�����*/
   closegraph();  /*�ر�ͼ��ģʽ*/
}
void Pr(int a[],int n)/*�������*/
{
   int i;
   char num[5];
   settextstyle(0,0,2);  /*���������ʽ*/
   setcolor(GREEN);  /*���������ɫ*/
   for(i=100;i<500;i+=50)/*i������ʾλ�úͼ��������±�*/
   {
      sprintf(num,"%d",a[(i-100)/50]); /*����ֵת��Ϊ�ַ���*/
      outtextxy(i,n,num);   /*����ַ���*/
   }
}
void DrawChange(int i,int j)/*��������ͷ������������һ˫���ͷ��*/
{
   setcolor(6);
   line(j*50+120,i+8,j*50+140,i+8); /*������������λ�û�ֱ��*/
   line(j*50+120,i+8,j*50+120+5,i+4);
   line(j*50+120,i+8,j*50+120+5,i+12);
   line(j*50+140,i+8,j*50+140-5,i+4);
   line(j*50+140,i+8,j*50+140-5,i+12);
}
void Bubble_sort(int a[],int n)/*ð������*/
{
   int i,j,t,flag;
   char num1[5],num2[5];
   for(i=0;i<n-1;i++)/*ð������*/
   {
      flag=0;   /*�������ݽ�����־*/
      for(j=0;j<n-1-i;j++)
      {
	 Pr(a,i*40+80);/*�����*/
	 setcolor(BLUE);/*���Ҫ�Ƚϵ�������*/
	 sprintf(num1,"%d",a[j]);/*����������ת���ַ������*/
	 outtextxy(100+j*50,i*40+80,num1);
	 sprintf(num2,"%d",a[j+1]);
	 outtextxy(100+(j+1)*50,i*40+80,num2);
	 sleep(1);/*��ͣ����һ��*/
	/*setfillstyle(SOLID_FILL,BLACK);
	 bar(0,i*40+60,640,i*40+100);*/ /*ֻ��ʾ��ǰ����Ҫ����������*/
	 if(a[j]>a[j+1])/*���ǰ��Ĵ��ں����*/
	 {
	    flag=1; /*�ý�����־*/
	    DrawChange(i*40+80,j);/*��������ͷ*/
	    setcolor(RED);
	    outtextxy(100+j*50,i*40+80,num1);
	    outtextxy(100+(j+1)*50,i*40+80,num2);
	    t=a[j];/*����*/
	    a[j]=a[j+1];
	    a[j+1]=t;
	    sleep(1);
	    setfillstyle(SOLID_FILL,BLACK);/*�ھ��͵ķ�ʽ�����и�ɾ��*/
	    bar(0,i*40+60,640,i*40+100);
	 }
      }
      Pr(a,i*40+80);  /*�����*/
      sleep(1);
      if(flag==0)break; /*������αȽ�û�з�������������ѭ��*/
   }
}
