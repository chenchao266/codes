/*3. 源程序*/
/***dsdemo.c演示系统***/
#include<stdlib.h>
#include<graphics.h>
#define ESC 0x011b
#define ENTER 0x1c0d
#define DOWN 0x5000
#define UP 0x4800
struct Snow/*雪的一些参数*/
{
   int x;
   int y;
   int speed;/*雪花的速度*/
}snow[100];
struct Star/*星星的一些参数*/
{
   int x;
   int y;
   int color;
}star[200];
int key;/*按键变量*/
int keyx,keyy;/*选项键的坐标*/
int snownum=0;/*雪的个数*/
int size;/*保存区域的大小*/
int change=10;/*变颜色有关*/
void *save1,*save2;/*保存空间*/
void Copy(void);/*保存区域*/
void DrawSnow(void);/*具体实现*/
void Pr(void);/*输出字体*/
void Choose(void);/*选择演示内容*/
void DrawStar(void);/*最后的星星效果*/
void DrawBall(int x,int y,int color);/*画选择的球体*/
void Init(void);/*图形驱动*/
void Close(void);/*图形关闭*/
void main(void)
{
   Init();
   Copy();
   DrawSnow();/*雪花飘动*/
   Choose();/*选择演示内容*/
   DrawStar();/*结束画面*/
   Close();
   exit(0);
}
void Init(void)
{
   int gd=DETECT,gm;
   initgraph(&gd,&gm,"c:\\tc");
   cleardevice();
}
void Close(void)/*图形关闭*/
{
   getch();
   closegraph();
}
void Copy(void)/*保存区域*/
{
   setcolor(0);
   setfillstyle(SOLID_FILL,15);
   fillellipse(200,200,4,4);
   size=imagesize(196,196,204,204);/*定义保存图象区域大小*/
   save1=malloc(size);/*申请空间*/
   save2=malloc(size);
   getimage(196,196,204,204,save1);/*保存雪花*/
   getimage(96,96,104,104,save2); /*保存背景黑色*/
}
void Pr(void)/*在雪中输出文字*/
{
   settextstyle(0,0,1);
   if(change>=20&&change<=30||change>=50&&change<=60||
     change>=100&&change<=110)/*闪烁效果*/
      setcolor(YELLOW);
   else
      setcolor(BLACK);
   outtextxy(250,250,"anykey to continue");
   setcolor(change/10);/*变换颜色显示标题*/
   settextstyle(0,0,3);
   outtextxy(100,200,"Data Structure Demo");/*标题*/
}
/*显示雪花的具体过程*/
void DrawSnow(void)
{
   int i;
   int sx[62];
   randomize();
   for(i=0;i<62;i++) /*定义雪花的x坐标*/
      sx[i]=(i+2)*10;
   cleardevice();
   while(!kbhit())
   {
      Pr();
      if(snownum!=100)/*生成新的雪花*/
      {
	 snow[snownum].speed=2+random(5);/*速度随机定，但不小于2*/
	 i=random(62);
	 snow[snownum].x=sx[i];/*随机取x坐标*/
	 snow[snownum].y=10-random(100);
      }
      for(i=0;i<snownum;i++)/*去雪*/
	 putimage(snow[i].x,snow[i].y,save2,COPY_PUT);
      Pr(); /*显示标题*/
      if(snownum!=100)
	 snownum++;
      setfillstyle(SOLID_FILL,15);/*画雪*/
      for(i=0;i<snownum;i++)
      {
	 snow[i].y+=snow[i].speed;
	 putimage(snow[i].x,snow[i].y,save1,COPY_PUT);
	 if(snow[i].y>500)
	    snow[i].y=10-random(200);
      }
      change++;
      if(change==140)/*颜色变化*/
	 change=10;
   }
}
/*画选择的球体x和y为坐标，color为球的颜色*/
void DrawBall(int x,int y,int color)
{
   setcolor(0);
   setfillstyle(SOLID_FILL,color);
   fillellipse(x,y+10,10,10);
}
/*选择演示函数*/
void Choose(void)
{
   int yes;/*判断是否退出选项，1不退出0退出*/
   int oyes=1;
   while(oyes)
   {
      Init();
      cleardevice();
      yes=1;
      keyx=140;
      keyy=150;
      setcolor(11);
      rectangle(40,40,600,440);/*画边框线/
      setcolor(13);
      settextstyle(0,0,3);/*标题大一些*/
      outtextxy(100,70,"Data Structure Demo");
      settextstyle(0,0,2);/*其它选项小一些*/
      setcolor(RED);
      outtextxy(200,150,"hanoi tower");/*汉诺塔*/
      setcolor(BLUE);
      outtextxy(200,190,"double link");/*双链表*/
      setcolor(GREEN);
      outtextxy(200,230,"bubble sorting"); /*起泡排序*/
      setcolor(YELLOW);
      outtextxy(200,270,"radix sorting"); /*基数排序*/
      setcolor(10);
      outtextxy(200,310,"binary serach");	/*二分查找*/
      setcolor(MAGENTA);
      outtextxy(200,350,"binary tree");  /* 二叉树遍历*/
      setcolor(WHITE);
      outtextxy(200,390,"exit");/*结束程序*/
      DrawBall(keyx,keyy,11);
      while(yes)
      /*选项的循环条件*/
      {
	 key=bioskey(0);
	 if(key==ESC)/*退出系统*/
	    break;
	 if(key==UP)/*上键盘操作*/
	 {
	    DrawBall(keyx,keyy,BLACK);/*先用黑色在原来位置去除球*/
	    if(keyy!=150)
	       keyy-=40;
	    else
	       keyy=390;
	    DrawBall(keyx,keyy,11);/*新位置输出球*/
	 }
	 if(key==DOWN)/*下键盘操作*/
	 {
	    DrawBall(keyx,keyy,BLACK);/*先用黑色在原来位置去除球*/
	    if(keyy!=390)
	       keyy+=40;
	    else
	       keyy=150;
	    DrawBall(keyx,keyy,11);/*新位置输出球*/
	 }
	 if(key==ENTER)/*确定键*/
	 {
	    switch(keyy)/*判断内容*/
	    {
	       case 150:system("hanoi");yes=0;break;/*调用汉诺塔*/
	       case 190:system("dblink");yes=0;break;/*调用双链表*/
	       case 230:system("bubble");yes=0;break;/*调用起泡排序*/
	       case 270:system("radix");yes=0;break;/*调用基数排序*/
	       case 310:system("binary");yes=0;break;/*调用二分查找*/
	       case 350:system("tree2");yes=0;break; /*调用二叉树*/
	       case 390: yes=0;oyes=0;/*exit选项*/
	    }/*endswitch*/
	 } /*endenter*/
      } /*endwhile*/
   } /*endwhile*/
}
/*结束画面*/
void DrawStar(void)
{
   int i;
   cleardevice();
   setcolor(GREEN);
   settextstyle(0,0,2);
   while(!kbhit())
   {
      for(i=0;i<200;i++)/*随机生成星星*/
      {
	 star[i].x=random(640);
	 star[i].y=random(480);
	 star[i].color=random(13)+1;
      }
      for(i=0;i<200;i++)/*输出星星*/
      {
	 putpixel(star[i].x,star[i].y,star[i].color);
	 delay(100);
      }
      outtextxy(100,200,"Thank you for use this system");
      sleep(1);
      for(i=0;i<200;i++)/*去除星星*/
      {
	 putpixel(star[i].x,star[i].y,0);
	 delay(50);
      }
   }
}
