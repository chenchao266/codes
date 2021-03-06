/*4.3.3源程序*/
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#define N 20/*迷宫的大小，可改变*/
int  oldmap[N][N];/*递归用的数组,用全局变量节约时间*/
int  yes=0;/*yes是判断是否找到路的标志,1找到，0没找到*/
int  way[100][2],wayn=0;/*way数组是显示路线用的,wayn是统计走了几个格子*/
void Init(void);/*图形初始化*/
void Close(void);/*图形关闭*/
void DrawPeople(int *x,int *y,int n);/*画人工探索物图*/
void PeopleFind(int (*x)[N]);/*人工探索*/
void WayCopy(int (*x)[N],int (*y)[N]);/*为了8个方向的递归，把旧迷宫图拷贝给新数组*/
int  FindWay(int (*x)[N],int i,int j);/*自动探索函数*/
void MapRand(int (*x)[N]);/*随机生成迷宫函数*/
void PrMap(int (*x)[N]);/*输出迷宫图函数*/
void Result(void);/*输出结果处理*/
void Find(void);/*成功处理*/
void NotFind(void);/*失败处理*/
void main(void)/*主函数*/
{
   int map[N][N]; /*迷宫数组*/
   char ch;
   clrscr();
   printf("\n Please select hand(1) else auto\n");/*选择探索方式*/
   scanf("%c",&ch);
   Init();  /*初始化*/
   MapRand(map);/*生成迷宫*/
   PrMap(map);/*显示迷宫图*/
   if(ch=='1')
      PeopleFind(map);/*人工探索*/
   else
      FindWay(map,1,1);/*系统自动从下标1,1的地方开始探索*/
   Result();/*输出结果*/
   Close();
}
void Init(void)/*图形初始化*/
{
   int gd=DETECT,gm;
   initgraph(&gd,&gm,"c:\\tc");
}
void DrawPeople(int *x,int *y,int n)/*画人工控制图*/
{/*如果将以下两句注释掉，则显示人工走过的路径，*/
   setfillstyle(SOLID_FILL,WHITE);  /*设置白色实体填充样式*/
   bar(100+(*y)*15-6,50+(*x)*15-6,100+(*y)*15+6,50+(*x)*15+6);
/*恢复原通路*/
   switch(n)/*判断x,y的变化，8个方向的变化*/
   {
      case 1: (*x)--;break; /*上*/
      case 2: (*x)--;(*y)++;break ;/*右上*/
      case 3: (*y)++;break;  /*右*/
      case 4: (*x)++;(*y)++;break; /*右下*/
      case 5: (*x)++;break;  /*下*/
      case 6: (*x)++;(*y)--;break; /*左下*/
      case 7: (*y)--;break;  /*左*/
      case 8: (*x)--;(*y)--;break; /*左上*/
   }
   setfillstyle(SOLID_FILL,RED);/*新位置显示探索物*/
   bar(100+(*y)*15-6,50+(*x)*15-6,100+(*y)*15+6,50+(*x)*15+6);
}
void PeopleFind(int (*map)[N])/*人工手动查找*/
{
   int x,y;
   char c=0;/*接收按键的变量*/
   x=y=1;/*人工查找的初始位置*/
   setcolor(11);
   line(500,200,550,200);
   outtextxy(570,197,"d");
   line(500,200,450,200);
   outtextxy(430,197,"a");
   line(500,200,500,150);
   outtextxy(497,130,"w");
   line(500,200,500,250);
   outtextxy(497,270,"x");
   line(500,200,450,150);
   outtextxy(445,130,"q");
   line(500,200,550,150);
   outtextxy(550,130,"e");
   line(500,200,450,250);
   outtextxy(445,270,"z");
   line(500,200,550,250);
   outtextxy(550,270,"c");/*以上是画8个方向的控制介绍*/
   setcolor(YELLOW);
   outtextxy(420,290,"Press 'Enter' to end");/*压回车键结束*/
   setfillstyle(SOLID_FILL,RED);
   bar(100+y*15-6,50+x*15-6,100+y*15+6,50+x*15+6);/*入口位置显示*/
   while(c!=13)/*如果按下的不是回车键*/
   {
      c=getch();/*接收字符后开始各个方向的探索*/
      if(c=='w'&&map[x-1][y]!=1)
	 DrawPeople(&x,&y,1);/*上*/
      else
	 if(c=='e'&&map[x-1][y+1]!=1)
	    DrawPeople(&x,&y,2);/*右上*/
	 else
	    if(c=='d'&&map[x][y+1]!=1)
	       DrawPeople(&x,&y,3);/*右*/
	    else
	       if(c=='c'&&map[x+1][y+1]!=1)
		  DrawPeople(&x,&y,4);/*右下*/
	       else
		  if(c=='x'&&map[x+1][y]!=1)
		     DrawPeople(&x,&y,5);/*下*/
		  else
		     if(c=='z'&&map[x+1][y-1]!=1)
			DrawPeople(&x,&y,6); /*左下*/
		     else
			if(c=='a'&&map[x][y-1]!=1)
			   DrawPeople(&x,&y,7); /*左*/
			else if(c=='q'&&map[x-1][y-1]!=1)
			   DrawPeople(&x,&y,8); /*左上*/
   }
   setfillstyle(SOLID_FILL,WHITE); /*消去红色探索物，恢复原迷宫图*/
   bar(100+y*15-6,50+x*15-6,100+y*15+6,50+x*15+6);
   if(x==N-2&&y==N-2)/*人工控制找成功的话*/
      yes=1; /*如果成功标志为1*/
}
void WayCopy(int (*oldmap)[N],int (*map)[N])/*拷贝迷宫数组 */
{
   int i,j;
   for(i=0;i<N;i++)
      for(j=0;j<N;j++)
	 oldmap[i][j]=map[i][j];
}
int FindWay(int (*map)[N],int i,int j)/*递归找路*/
{
   if(i==N-2&&j==N-2)/*走到出口*/
   {
      yes=1;/*标志为1,表示成功*/
      return;
   }
   map[i][j]=1;/*走过的地方变为1*/
   WayCopy(oldmap,map); /*拷贝迷宫图*/
   if(oldmap[i+1][j+1]==0&&!yes)/*判断右下方是否可走*/
   {
      FindWay(oldmap,i+1,j+1);
      if(yes)/*如果到达出口了，再把值赋给显示路线的way数组,也正是这个原因,所以具体路线是从最后开始保存*/
      {
	 way[wayn][0]=i;
	 way[wayn++][1]=j;
	 return;
      }
   }
   WayCopy(oldmap,map);
   if(oldmap[i+1][j]==0&&!yes)/*判断下方是否可以走,如果标志yes已经是1也不用找下去了*/
   {
      FindWay(oldmap,i+1,j);
      if(yes)
      {
	 way[wayn][0]=i;
	 way[wayn++][1]=j;
	 return;
      }
   }
   WayCopy(oldmap,map);
   if(oldmap[i][j+1]==0&&!yes)/*判断右方是否可以走*/
   {
      FindWay(oldmap,i,j+1);
      if(yes)
      {
	 way[wayn][0]=i;
	 way[wayn++][1]=j;
	 return;
      }
   }
   WayCopy(oldmap,map);
   if(oldmap[i-1][j]==0&&!yes)/*判断上方是否可以走*/
   {
      FindWay(oldmap,i-1,j);
      if(yes)
      {
	 way[wayn][0]=i;
	 way[wayn++][1]=j;
	 return;
      }
   }
   WayCopy(oldmap,map);
   if(oldmap[i-1][j+1]==0&&!yes)/*判断右上方是否可以走*/
   {
      FindWay(oldmap,i-1,j+1);
      if(yes)
      {
	 way[wayn][0]=i;
	 way[wayn++][1]=j;
	 return;
      }
   }
   WayCopy(oldmap,map);
   if(oldmap[i+1][j-1]==0&&!yes)/*判断左下方是否可以走*/
   {
      FindWay(oldmap,i+1,j-1);
      if(yes)
      {
	 way[wayn][0]=i;
	 way[wayn++][1]=j;
	 return;
      }
   }
   WayCopy(oldmap,map);
   if(oldmap[i][j-1]==0&&!yes)/*判断左方是否可以走*/
   {
      FindWay(oldmap,i,j-1);
      if(yes)
      {
	 way[wayn][0]=i;
	 way[wayn++][1]=j;
	 return;
      }
   }
   WayCopy(oldmap,map);
   if(oldmap[i-1][j-1]==0&&!yes)/*判断左上方是否可以走*/
   {
      FindWay(oldmap,i-1,j-1);
      if(yes)
      {
	 way[wayn][0]=i;
	 way[wayn++][1]=j;
	 return;
      }
   }
   return;
}
void MapRand(int (*map)[N])/*开始的随机迷宫图*/
{
   int i,j;
   cleardevice();/*清屏*/
   randomize(); /*随机数发生器*/
   for(i=0;i<N;i++)
   {
      for(j=0;j<N;j++)
      {
	 if(i==0||i==N-1||j==0||j==N-1)/*最外面一圈为墙壁*/
	    map[i][j]=1;
	 else
	    if(i==1&&j==1||i==N-2&&j==N-2)/*出发点与终点表示为可走的*/
	       map[i][j]=0;
	    else
	       map[i][j]=random(2);/*其它的随机生成0或1*/
      }
   }
}
void PrMap(int (*map)[N])/*输出迷宫图*/
{
   int i,j;
   for(i=0;i<N;i++)
      for(j=0;j<N;j++)
	 if(map[i][j]==0)
	 {
	    setfillstyle(SOLID_FILL,WHITE);/*白色为可走的路*/
	    bar(100+j*15-6,50+i*15-6,100+j*15+6,50+i*15+6);
	 }
	 else
	 {
	    setfillstyle(SOLID_FILL,BLUE);/*蓝色为墙壁*/
	    bar(100+j*15-6,50+i*15-6,100+j*15+6,50+i*15+6);
	 }
}
void Find(void)/*找到通路*/
{
   int i;
   setfillstyle(SOLID_FILL,RED);/*红色输出走的具体路线*/
   wayn--;
   for(i=wayn;i>=0;i--)
   {
      bar(100+way[i][1]*15-6,50+way[i][0]*15-6,100+
      way[i][1]*15+6,50+way[i][0]*15+6);
      sleep(1);/*控制显示时间*/
   }
   bar(100+(N-2)*15-6,50+(N-2)*15-6,100+
	(N-2)*15+6,50+(N-2)*15+6); /*在目标点标红色*/
   setcolor(GREEN);
   settextstyle(0,0,2);/*设置字体大小*/
   outtextxy(130,400,"Find a way!");
}
void NotFind(void)/*没找到通路*/
{
   setcolor(GREEN);
   settextstyle(0,0,2);/*设置字体大小*/
   outtextxy(130,400,"Not find a way!");
}
void Result(void)/*结果处理*/
{
   if(yes)/*如果找到*/
      Find();
   else/*没找到路*/
      NotFind();
   getch();
}
void Close(void)/*图形关闭*/
{
   closegraph();
}
