/*2. 源程序*/
/***bubble.c***/
#include <graphics.h>
# include <dos.h>
#define N 8   /*定义排序数据为8个*/
void Init(int a[]);/*初始状态*/
void Close(void);/*图形模式结束*/
void Pr(int a[],int n);/*输出数组*/
void DrawChange(int i,int j);/*画交换箭头*/
void Bubble_sort(int a[],int n);/*冒泡排序*/
void main(void)
{
   int i,a[N];
   printf("Please enter 8 int number\n"); /*输入8个整数*/
   for(i=0;i<N;i++)
   scanf("%d",&a[i]);
   Init(a);/*初始状态*/
   Bubble_sort(a,8);/*具体排序*/
   Close();/*图形模式结束*/
   exit(0);
}
void Init(int a[])/*初始状态*/
{
   int gd=DETECT,gm,i;
   initgraph(&gd,&gm,"");/*初始化图形系统*/
   cleardevice();  /*清屏*/
   setcolor(YELLOW);  /*设置颜色*/
   outtextxy(210,10,"any key to continue");  /*显示压任意键继续*/
   Pr(a,40);/*第一次输出数组*/
   getch();  /*等待压任意键*/
}
void Close (void) /*图形模式结束*/
{
   getch();  /*等待压任意键*/
   closegraph();  /*关闭图形模式*/
}
void Pr(int a[],int n)/*输出数组*/
{
   int i;
   char num[5];
   settextstyle(0,0,2);  /*设置输出样式*/
   setcolor(GREEN);  /*设置输出颜色*/
   for(i=100;i<500;i+=50)/*i控制显示位置和计算数组下标*/
   {
      sprintf(num,"%d",a[(i-100)/50]); /*将数值转化为字符串*/
      outtextxy(i,n,num);   /*输出字符串*/
   }
}
void DrawChange(int i,int j)/*画交换箭头，画五根线组成一双向箭头线*/
{
   setcolor(6);
   line(j*50+120,i+8,j*50+140,i+8); /*按给出的坐标位置画直线*/
   line(j*50+120,i+8,j*50+120+5,i+4);
   line(j*50+120,i+8,j*50+120+5,i+12);
   line(j*50+140,i+8,j*50+140-5,i+4);
   line(j*50+140,i+8,j*50+140-5,i+12);
}
void Bubble_sort(int a[],int n)/*冒泡排序*/
{
   int i,j,t,flag;
   char num1[5],num2[5];
   for(i=0;i<n-1;i++)/*冒泡排序*/
   {
      flag=0;   /*设置数据交换标志*/
      for(j=0;j<n-1-i;j++)
      {
	 Pr(a,i*40+80);/*输出数*/
	 setcolor(BLUE);/*输出要比较的两个数*/
	 sprintf(num1,"%d",a[j]);/*将两个数字转成字符串输出*/
	 outtextxy(100+j*50,i*40+80,num1);
	 sprintf(num2,"%d",a[j+1]);
	 outtextxy(100+(j+1)*50,i*40+80,num2);
	 sleep(1);/*暂停运行一秒*/
	/*setfillstyle(SOLID_FILL,BLACK);
	 bar(0,i*40+60,640,i*40+100);*/ /*只显示当前两个要交换的数据*/
	 if(a[j]>a[j+1])/*如果前面的大于后面的*/
	 {
	    flag=1; /*置交换标志*/
	    DrawChange(i*40+80,j);/*画交换箭头*/
	    setcolor(RED);
	    outtextxy(100+j*50,i*40+80,num1);
	    outtextxy(100+(j+1)*50,i*40+80,num2);
	    t=a[j];/*交换*/
	    a[j]=a[j+1];
	    a[j+1]=t;
	    sleep(1);
	    setfillstyle(SOLID_FILL,BLACK);/*黑巨型的方式把这行给删除*/
	    bar(0,i*40+60,640,i*40+100);
	 }
      }
      Pr(a,i*40+80);  /*输出数*/
      sleep(1);
      if(flag==0)break; /*如果本次比较没有发生交换则跳出循环*/
   }
}
