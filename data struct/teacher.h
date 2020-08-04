/*7. teacher.c源程序*/
/******* teacher.c***********/
/******头文件、函数原型及全局变量定义***********/
#include "stdio.h"
#include "bios.h"
#include "dos.h"
#include "conio.h"
#include "stdlib.h"
#include "string.h"
#include "mem.h"
#include "ctype.h"
#include "alloc.h"
#include "stdio.h"
#include "graphics.h"
#include "d:\cbook\14\c14_m.c" /*根据实际存放位置设*/
typedef struct t1
{
   char no[11];
   char name[15];
   char sex[5];
   char profess[15];
   char dept[15];
   char class[15];
   float workload;
   float lessonf;
   struct t1 *prior;
   struct t1 *next;
}TEACHER;
TEACHER *First;
TEACHER *Last;
TEACHER *p;
void init(); /*初始化双链表*/
void firstr(); /*显示头一条*/
void lastr();/*显示最后一条*/
void priorr(); /*显示前一条*/
void nextr();/*显示下一条*/
void clear(); /*清除显示信息*/
void delete(); /*删除当前记录*/
void save(); /*保存*/
void load();/*读入文件*/
void sort();/*排序*/
void create(); /*创建双链表*/
void print(TEACHER*p); /*输出数据*/
void run(int m,int n); /*调用各功能*/
void button(int buttonx1,int buttony1,int buttonx2,
int buttony2,char *name,int stat); /*按钮*/
void wins(int x1,int y1,int x2,int y2); /*窗口*/
void realwin(int x1,int y1,int x2,int y2);/*校验窗口*/
void mousewin(); /*主控界面*/
void cross(int y1,int x2,int press); /*关闭按钮*/
char *Str[]={"First","Prior","Next","Last","Clear",
    "Delete","Load","Save","Sort","Exit"}; /*命令项*/
/*********主函数**********/
void main()
{
   char ch;
   int gdriver,gmode,i,j;
   init();
   printf("Do you  create new table(Y/N)\n");
   ch=getchar();
   if(ch=='Y'||ch=='y')
      create();
   gdriver=DETECT;
   initgraph(&gdriver,&gmode,"");
   mousewin();
   closegraph();
}
/**按钮实现函数，buttonx1和 buttony1为按钮的左上角的坐标，buttonx2和 buttony2为按钮的右下角的坐标，*name为按钮上显示的字符串名，stat为按钮类型，0为平面，1为凸起，2为凹下，3为兰色平面*/
void button(int buttonx1,int buttony1,int buttonx2,
    int buttony2,char *name,int stat)
{
   if(stat==0) /*显示平面按钮*/
   {
      setcolor(BLACK);
      outtextxy(buttonx1+3,buttony1+4,name);
   }
   if(stat==1) /*显示凸起按钮*/
   {
      setfillstyle(1,WHITE);
      bar(buttonx1,buttony1,buttonx2,buttony2);
      setfillstyle(1,DARKGRAY);
      bar(buttonx1+1,buttony1+1,buttonx2,buttony2);
      setfillstyle(1,LIGHTGRAY);
      bar(buttonx1+1,buttony1+1,buttonx2-1,buttony2-1);
      setcolor(BLACK);
      outtextxy(buttonx1+3,buttony1+4,name);
   }
   if(stat==2) /*显示凹下按钮*/
   {
      setfillstyle(1,DARKGRAY);
      bar(buttonx1,buttony1,buttonx2,buttony2);
      setfillstyle(1,WHITE);
      bar(buttonx1+1,buttony1+1,buttonx2,buttony2);
      setfillstyle(1,LIGHTGRAY);
      bar(buttonx1+1,buttony1+1,buttonx2-1,buttony2-1);
      setcolor(BLACK);
      outtextxy(buttonx1+3,buttony1+4,name);
   }
   if(stat==3) /*显示平面兰色背景按钮*/
   {
      setfillstyle(1,DARKGRAY);
      bar(buttonx1,buttony1,buttonx2,buttony2);
      setfillstyle(1,WHITE);
      bar(buttonx1+1,buttony1+1,buttonx2,buttony2);
      setfillstyle(1,BLUE);
      bar(buttonx1+1,buttony1+1,buttonx2-1,buttony2-1);
      setcolor(WHITE);
      outtextxy(buttonx1+3,buttony1+4,name);
   }
}
/****窗口函数，x1和y1为左上角坐标，x2和y2为右下角坐标*/
void wins(int x1,int y1,int x2,int y2)
{
   char lable[][10]={"No","Name","Sex",
   "Profess","Dept", "Class","Workload","LessonF"}; /*标签信息*/
   int m,n;
   setfillstyle(1,WHITE);/*画窗口*/
   bar(x1,y1,x2,y2);
   setfillstyle(1,LIGHTGRAY);
   bar(x1+1,y1+1,x2,y2);
   setfillstyle(1,BLUE); /*画兰色标题栏*/
   bar(x1+3,y1+3,x2-3,y1+20);
   cross(y1,x2,0); /*画关闭按钮*/
   outtextxy(x1+5,y1+8,"teacher LessonF");/*输出标题文字*/
   for(m=0;m<4;m++)
      for(n=0;n<2;n++)
      {
	 button(x1+n*250+20,y1+50+m*30+20,x1+n*250+90,
	 y1+50+m*30+40,lable[n+m*2],2);/*标签信息*/
	 setfillstyle(1,WHITE);
	 bar(x1+n*250+100,y1+50+m*30+20,x1+n*250+200,y1+50+m*30+40);
	 /*白色背景显示数据区*/
      }
   settextstyle(0,0,1);
   for(m=0;m<2;m++)
      for(n=0;n<5;n++)
	 button(x1+n*100+10,y1+200+m*30+20,x1+n*100+80,
         y1+200+m*30+40,Str[n+m*5],1); /*显示凸起命令按钮*/
}
/*关闭按钮函数y1和x2分别是窗口的左上角的y坐标和右下角的x坐标，*/
void cross(int y1,int x2,int press)
{
   if(press==1)
      setfillstyle(1,BLACK);
   else
      setfillstyle(1,WHITE);
   bar(x2-17,y1+5,x2-5,y1+18);/*画矩形区域*/
   setfillstyle(1,LIGHTGRAY);
   bar(x2-16,y1+6,x2-5,y1+18);
   setwritemode(EMPTY_FILL);/*用背景颜色填充*/
   setcolor(BLACK);
   setlinestyle(0,0,3); /*设置线型*/
   line(x2-14,y1+8,x2-8,y1+16);/*画交叉线*/
   line(x2-8,y1+8,x2-14,y1+16);
}
/*鼠标操作控制处理函数*/
void mousewin()
{
   union REGS inregs,outregs;
   int x1,y1,i,m,n,state;
   int press=0,close=0,cpress=0;
   int winx1=70,winy1=100,winx2=570,winy2=400;/*设置窗口坐标*/
   MouseSetXY(50,50);  /*设置鼠标位置*/
   MouseOn(); /*显示鼠标*/
   wins(winx1,winy1,winx2,winy2);/*显示窗口主界面*/
   while(1)
   {
      press=0;/*是否选择了exit菜单*/
      cpress=0; /*是否单击了命令按钮*/
      MouseStatus(&x1,&y1,&state);/*读鼠标状态*/
      while(x1>winx1+3&&x1<winx1+45&&y1>winy1+21&&y1<winy1+35)
      {/*鼠标在main*/
	 if(press==0)
	    button(winx1+3,winy1+21,winx1+45,winy1+35,"main",1); /*凸起*/
	 delay(1000);
	 MouseStatus(&x1,&y1,&state);
	 while(state==1) /*单击了鼠标左键*/
	 {
	    settextstyle(0,0,0);
	    button(winx1+3,winy1+21,winx1+45,winy1+35,"main",2);/*凹下*/
	    MouseStatus(&x1,&y1,&state);
	    delay(1000);
	    press=1;
	 }
	 if(press==1)
	    button(winx1+3,winy1+35,winx1+45,winy1+50,"exit",1);
	 }
	 while(press==1&&(x1>winx1+3&&x1<winx1+45&&y1>winy1+35&&y1<winy1+50))/*鼠标在exit上*/
	 {
	    button(winx1+3,winy1+35,winx1+45,winy1+50,"exit",3);/*蓝色平面*/
	    delay(1000);
	    MouseStatus(&x1,&y1,&state);
	    if(state==1) /*单击了exit菜单，退出*/
		exit(0);
	 }
	 if(!(x1>winx1+3&&x1<winx1+45&&y1>winy1+21&&y1<winy1+35)
	 &&press==0) /*鼠标不在main上*/
	 {
	    setfillstyle(1,LIGHTGRAY);
	    bar(winx1+3,winy1+21,winx1+45,winy1+35);
	    settextstyle(0,0,0);
	    button(winx1+3,winy1+21,winx1+45,winy1+35,"main",0);
	    delay(1000);
	 }
	 if(press==1&&!(x1>winx1+3&&
	 x1<winx1+45&&y1>winy1+35&&y1<winy1+50))
	 {  /*鼠标不在exit上*/
	    settextstyle(0,0,0);
	    button(winx1+3,winy1+35,winx1+45,winy1+50,"exit",1);
	    delay(1000);
	 }
	 if(state==1&&(!(x1>winx1+3&&x1<winx1+45
	 &&y1>winy1+21&&y1<winy1+35)))
	 {/*在main区域外单击了鼠标*/
	    setfillstyle(1,LIGHTGRAY);
	    bar(winx1+3,winy1+35,winx1+45,winy1+50);
	    press=0;
	 }
	 while(state==1)
	 {
	    MouseStatus(&x1,&y1,&state);
	    if(x1>winx2-17&&y1>winy1+5&&x1<winx2-5&&y1<winy1+18)
	    {/*鼠标在关闭按钮上单击*/
	       cross(winy1,winx2,1);/*凸起显示关闭按钮*/
	       delay(1000);
	       close=1;
	    }
	 }
	 if(state==0)
	 {
	    cross(winy1,winx2,0);/*平面显示关闭按钮*/
	    if(close==1)  exit(0);
	 }
	 while(x1>winx1+5&&y1>winy1+220&&x1<winx2-20&&y1<winy2-20)
	 {
	    if(cpress==0)  /*鼠标在命令按钮上*/
	    {
	       settextstyle(0,0,1);
	       for(m=0;m<2;m++) /*显示命令按钮*/
		  for(n=0;n<5;n++)
		     button(winx1+n*100+10,winy1+200+m*30+20,winx1+n*100+80,
		     winy1+200+m*30+40,Str[n+m*5],1);
	       delay(1000);
	       MouseStatus(&x1,&y1,&state);
	       if(state==1)/*判断单击了哪个命令按钮*/
	       {
		  settextstyle(0,0,0);
		  if(x1>80&&x1<150)
		     if(y1>320&&y1<340)/*单击了First命令按钮*/
			{m=0;n=0;}
		     else
			{m=1;n=0;} /*单击了Delete命令按钮*/
		  if(x1>180&&x1<250)
		     if(y1>320&&y1<340)
			{m=0;n=1;} /*单击了Prior命令按钮*/
		     else
			{m=1;n=1;} /*单击了Load命令按钮*/
		  if(x1>280&&x1<350)
		     if(y1>320&&y1<340)
			{m=0;n=2;} /*单击了Next命令按钮*/
		     else
			{m=1;n=2;} /*单击了Save命令按钮*/
		  if(x1>380&&x1<450)
		     if(y1>320&&y1<340)
			{m=0;n=3;} /*单击了Last命令按钮*/
		     else
			{m=1;n=3;} /*单击了Sort命令按钮*/
		  if(x1>480&&x1<550)
		     if(y1>320&&y1<340)
			{m=0;n=4;} /*单击了Clear命令按钮*/
		     else
			{m=1;n=4;} /*单击了Exit命令按钮*/
	       cpress=1;
	       break;
	       }
	    }
	 }
      if(cpress==1)
      {
	 button(winx1+n*100+10,winy1+200+m*30+20,winx1+n*100+80,
	 winy1+200+m*30+40,Str[n+m*5],2); /*单击的按钮凹下显示*/
	 run(m,n); /*调用相应的功能*/
      }
   } /*结束while（1）*/
}
/********功能调用函数，参数m和n代表按钮行和列位置********/
void run(int m,int n)
{
   switch(n+m*5)
   {
      case 0:firstr();break;
      case 1:priorr();break;
      case 2:nextr();break;
      case 3:lastr();break;
      case 4:clear();break;
      case 5:delete();break;
      case 6:load();break;
      case 7:save();break;
      case 8:sort();break;
      case 9:exit(0);
   }/*结束switch*/
}
