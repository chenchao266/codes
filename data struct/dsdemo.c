/*3. Դ����*/
/***dsdemo.c��ʾϵͳ***/
#include<stdlib.h>
#include<graphics.h>
#define ESC 0x011b
#define ENTER 0x1c0d
#define DOWN 0x5000
#define UP 0x4800
struct Snow/*ѩ��һЩ����*/
{
   int x;
   int y;
   int speed;/*ѩ�����ٶ�*/
}snow[100];
struct Star/*���ǵ�һЩ����*/
{
   int x;
   int y;
   int color;
}star[200];
int key;/*��������*/
int keyx,keyy;/*ѡ���������*/
int snownum=0;/*ѩ�ĸ���*/
int size;/*��������Ĵ�С*/
int change=10;/*����ɫ�й�*/
void *save1,*save2;/*����ռ�*/
void Copy(void);/*��������*/
void DrawSnow(void);/*����ʵ��*/
void Pr(void);/*�������*/
void Choose(void);/*ѡ����ʾ����*/
void DrawStar(void);/*��������Ч��*/
void DrawBall(int x,int y,int color);/*��ѡ�������*/
void Init(void);/*ͼ������*/
void Close(void);/*ͼ�ιر�*/
void main(void)
{
   Init();
   Copy();
   DrawSnow();/*ѩ��Ʈ��*/
   Choose();/*ѡ����ʾ����*/
   DrawStar();/*��������*/
   Close();
   exit(0);
}
void Init(void)
{
   int gd=DETECT,gm;
   initgraph(&gd,&gm,"c:\\tc");
   cleardevice();
}
void Close(void)/*ͼ�ιر�*/
{
   getch();
   closegraph();
}
void Copy(void)/*��������*/
{
   setcolor(0);
   setfillstyle(SOLID_FILL,15);
   fillellipse(200,200,4,4);
   size=imagesize(196,196,204,204);/*���屣��ͼ�������С*/
   save1=malloc(size);/*����ռ�*/
   save2=malloc(size);
   getimage(196,196,204,204,save1);/*����ѩ��*/
   getimage(96,96,104,104,save2); /*���汳����ɫ*/
}
void Pr(void)/*��ѩ���������*/
{
   settextstyle(0,0,1);
   if(change>=20&&change<=30||change>=50&&change<=60||
     change>=100&&change<=110)/*��˸Ч��*/
      setcolor(YELLOW);
   else
      setcolor(BLACK);
   outtextxy(250,250,"anykey to continue");
   setcolor(change/10);/*�任��ɫ��ʾ����*/
   settextstyle(0,0,3);
   outtextxy(100,200,"Data Structure Demo");/*����*/
}
/*��ʾѩ���ľ������*/
void DrawSnow(void)
{
   int i;
   int sx[62];
   randomize();
   for(i=0;i<62;i++) /*����ѩ����x����*/
      sx[i]=(i+2)*10;
   cleardevice();
   while(!kbhit())
   {
      Pr();
      if(snownum!=100)/*�����µ�ѩ��*/
      {
	 snow[snownum].speed=2+random(5);/*�ٶ������������С��2*/
	 i=random(62);
	 snow[snownum].x=sx[i];/*���ȡx����*/
	 snow[snownum].y=10-random(100);
      }
      for(i=0;i<snownum;i++)/*ȥѩ*/
	 putimage(snow[i].x,snow[i].y,save2,COPY_PUT);
      Pr(); /*��ʾ����*/
      if(snownum!=100)
	 snownum++;
      setfillstyle(SOLID_FILL,15);/*��ѩ*/
      for(i=0;i<snownum;i++)
      {
	 snow[i].y+=snow[i].speed;
	 putimage(snow[i].x,snow[i].y,save1,COPY_PUT);
	 if(snow[i].y>500)
	    snow[i].y=10-random(200);
      }
      change++;
      if(change==140)/*��ɫ�仯*/
	 change=10;
   }
}
/*��ѡ�������x��yΪ���꣬colorΪ�����ɫ*/
void DrawBall(int x,int y,int color)
{
   setcolor(0);
   setfillstyle(SOLID_FILL,color);
   fillellipse(x,y+10,10,10);
}
/*ѡ����ʾ����*/
void Choose(void)
{
   int yes;/*�ж��Ƿ��˳�ѡ�1���˳�0�˳�*/
   int oyes=1;
   while(oyes)
   {
      Init();
      cleardevice();
      yes=1;
      keyx=140;
      keyy=150;
      setcolor(11);
      rectangle(40,40,600,440);/*���߿���/
      setcolor(13);
      settextstyle(0,0,3);/*�����һЩ*/
      outtextxy(100,70,"Data Structure Demo");
      settextstyle(0,0,2);/*����ѡ��СһЩ*/
      setcolor(RED);
      outtextxy(200,150,"hanoi tower");/*��ŵ��*/
      setcolor(BLUE);
      outtextxy(200,190,"double link");/*˫����*/
      setcolor(GREEN);
      outtextxy(200,230,"bubble sorting"); /*��������*/
      setcolor(YELLOW);
      outtextxy(200,270,"radix sorting"); /*��������*/
      setcolor(10);
      outtextxy(200,310,"binary serach");	/*���ֲ���*/
      setcolor(MAGENTA);
      outtextxy(200,350,"binary tree");  /* ����������*/
      setcolor(WHITE);
      outtextxy(200,390,"exit");/*��������*/
      DrawBall(keyx,keyy,11);
      while(yes)
      /*ѡ���ѭ������*/
      {
	 key=bioskey(0);
	 if(key==ESC)/*�˳�ϵͳ*/
	    break;
	 if(key==UP)/*�ϼ��̲���*/
	 {
	    DrawBall(keyx,keyy,BLACK);/*���ú�ɫ��ԭ��λ��ȥ����*/
	    if(keyy!=150)
	       keyy-=40;
	    else
	       keyy=390;
	    DrawBall(keyx,keyy,11);/*��λ�������*/
	 }
	 if(key==DOWN)/*�¼��̲���*/
	 {
	    DrawBall(keyx,keyy,BLACK);/*���ú�ɫ��ԭ��λ��ȥ����*/
	    if(keyy!=390)
	       keyy+=40;
	    else
	       keyy=150;
	    DrawBall(keyx,keyy,11);/*��λ�������*/
	 }
	 if(key==ENTER)/*ȷ����*/
	 {
	    switch(keyy)/*�ж�����*/
	    {
	       case 150:system("hanoi");yes=0;break;/*���ú�ŵ��*/
	       case 190:system("dblink");yes=0;break;/*����˫����*/
	       case 230:system("bubble");yes=0;break;/*������������*/
	       case 270:system("radix");yes=0;break;/*���û�������*/
	       case 310:system("binary");yes=0;break;/*���ö��ֲ���*/
	       case 350:system("tree2");yes=0;break; /*���ö�����*/
	       case 390: yes=0;oyes=0;/*exitѡ��*/
	    }/*endswitch*/
	 } /*endenter*/
      } /*endwhile*/
   } /*endwhile*/
}
/*��������*/
void DrawStar(void)
{
   int i;
   cleardevice();
   setcolor(GREEN);
   settextstyle(0,0,2);
   while(!kbhit())
   {
      for(i=0;i<200;i++)/*�����������*/
      {
	 star[i].x=random(640);
	 star[i].y=random(480);
	 star[i].color=random(13)+1;
      }
      for(i=0;i<200;i++)/*�������*/
      {
	 putpixel(star[i].x,star[i].y,star[i].color);
	 delay(100);
      }
      outtextxy(100,200,"Thank you for use this system");
      sleep(1);
      for(i=0;i<200;i++)/*ȥ������*/
      {
	 putpixel(star[i].x,star[i].y,0);
	 delay(50);
      }
   }
}
