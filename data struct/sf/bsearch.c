/*2. 源程序*/
/***binary.c二分查找程序***/
#include <graphics.h>
#include <stdlib.h>
#define N 15  /*结点数设为15*/
void find(int x[],int y,int z);/*具体查找过程*/
void Init(int x[],int *n);/*图形初始化*/
void Close();/*图形关闭*/
void Put(int x[],int y);/*输出数值*/
void Up(int x);/*画上箭*/
void Down(int x);/*画下箭*/
void Mid(int x);/*画中间箭*/
void Clr(int x);/*擦除画面上的一些内容*/
void Sort(int x[],int n);/*对随机数组排序*/
void main(void)
{
   int a[N],x;
   Init(a,&x);/*x为要查找的数*/
   Put(a,N);
   find(a,x,N);
   Close();
   exit(0);
}
void Mid(int n)/*画中间键*/
{
   setcolor(WHITE);/*中间箭的颜色为白色,以下三条线画成了箭头,以下两个函数一样*/
   line(25+n*40,120,25+n*40,80);
   line(25+n*40,120,20+n*40,110);
   line(25+n*40,120,30+n*40,110);
}
void Down(int n)/*画上箭*/
{
   setcolor(6);
   line(25+n*40,120,25+n*40,80);
   line(25+n*40,120,20+n*40,110);
   line(25+n*40,120,30+n*40,110);
}
void Up(int n)/*画下箭*/
{
   setcolor(6);
   line(25+n*40,180,25+n*40,220);
   line(25+n*40,180,20+n*40,190);
   line(25+n*40,180,30+n*40,190);
}
void Clr(int y)/*擦除画面上的一些内容*/
{
   setfillstyle(SOLID_FILL,0);/*每次演示的时候先将下面显示的文字给去处掉*/
   bar(0,y+50,640,y-50);/*这里是用矩形的方式*/
}
void Put(int a[],int n)/*输出数值*/
{
   int i;
   char num[5];
   setcolor(GREEN);
   settextstyle(0,0,2);/*设置字体的大小*/
   for(i=0;i<n;i++)
   {
      sprintf(num,"%d",a[i]);/*将数字转换成字符串输出*/
      outtextxy(20+i*40,150,num);
   }
   settextstyle(0,0,1);
   setcolor(BLUE);
   outtextxy(250,250,"anykey to continue");
   getch();
}
void find(int a[],int y,int n)/*具体的查找*/
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
      Clr(230);/*这里三个Clr是为了把屏幕上的箭头和文字删了*/
      mid=(high+low)/2;/*计算中间位置*/
      Up(high); /*显示上边箭头*/
      Down(low); /*显示右边箭头*/
      Mid(mid);/*画好了三个箭头后开始查找*/
      if(a[mid]==y) /*如果找到跳出循环*/
	 break;
      if(a[mid]<y)
      {
	 low=mid+1;  /*修改左边界*/
	 sprintf(str2,"%d",a[mid]);
	 outtextxy(250,250,str2); /*显示比较数据的情况*/
	 outtextxy(300,250,"<");
	 outtextxy(350,250,str1);
      }
      else
      {
	 high=mid-1;/*修改右边界*/
	 sprintf(str2,"%d",a[mid]);
	 outtextxy(250,250,str2); /*显示比较数据的情况*/
	 outtextxy(300,250,">");
	 outtextxy(350,250,str1);
      }
      sleep(2);/*间隔一秒好执行下一次*/
   }
   setcolor(11);
   if(low<=high)/*找到*/
      outtextxy(250,350,"FIND");
   else/*没找到*/
      outtextxy(250,350,"NOT FIND");
}
void Sort(int a[],int n)/*冒泡排序，自上而下*/
{
   int i,j,t;
   for(i=0;i<n-1;i++)
      for(j=0;j<n-1-i;j++)
	 if(a[j]>a[j+1])
	 {
	    t=a[j];
	    a[j]=a[j+1];
	    a[j+1]=t;
	 }  /*数据交换*/
}
void Init(int a[],int *x)/*图形驱动*/
{
   int i;
   int gd=DETECT,gm;
   randomize();/*随机函数*/
   for(i=0;i<N;i++)/*随机生成数组*/
      a[i]=random(100); /*生成100以内的数*/
   Sort(a,N);/*排序*/
   for(i=0;i<N;i++)
      printf("%-3d",a[i]); /*输出数据*/
   printf("\n");
   printf("please input a num you want to find: ");/*输入要找的数*/
   scanf("%d",x);
   initgraph(&gd,&gm,"c:\\tc");/*tc所在目录的路径*/
   cleardevice();
}
void Close()/*图形驱动结束*/
{
   getch();
   closegraph(); /*关闭图形系统*/
}
