/*7. teacher.cԴ����*/
/******* teacher.c***********/
/******ͷ�ļ�������ԭ�ͼ�ȫ�ֱ�������***********/
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
#include "d:\cbook\14\c14_m.c" /*����ʵ�ʴ��λ����*/
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
void init(); /*��ʼ��˫����*/
void firstr(); /*��ʾͷһ��*/
void lastr();/*��ʾ���һ��*/
void priorr(); /*��ʾǰһ��*/
void nextr();/*��ʾ��һ��*/
void clear(); /*�����ʾ��Ϣ*/
void delete(); /*ɾ����ǰ��¼*/
void save(); /*����*/
void load();/*�����ļ�*/
void sort();/*����*/
void create(); /*����˫����*/
void print(TEACHER*p); /*�������*/
void run(int m,int n); /*���ø�����*/
void button(int buttonx1,int buttony1,int buttonx2,
int buttony2,char *name,int stat); /*��ť*/
void wins(int x1,int y1,int x2,int y2); /*����*/
void realwin(int x1,int y1,int x2,int y2);/*У�鴰��*/
void mousewin(); /*���ؽ���*/
void cross(int y1,int x2,int press); /*�رհ�ť*/
char *Str[]={"First","Prior","Next","Last","Clear",
    "Delete","Load","Save","Sort","Exit"}; /*������*/
/*********������**********/
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
/**��ťʵ�ֺ�����buttonx1�� buttony1Ϊ��ť�����Ͻǵ����꣬buttonx2�� buttony2Ϊ��ť�����½ǵ����꣬*nameΪ��ť����ʾ���ַ�������statΪ��ť���ͣ�0Ϊƽ�棬1Ϊ͹��2Ϊ���£�3Ϊ��ɫƽ��*/
void button(int buttonx1,int buttony1,int buttonx2,
    int buttony2,char *name,int stat)
{
   if(stat==0) /*��ʾƽ�水ť*/
   {
      setcolor(BLACK);
      outtextxy(buttonx1+3,buttony1+4,name);
   }
   if(stat==1) /*��ʾ͹��ť*/
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
   if(stat==2) /*��ʾ���°�ť*/
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
   if(stat==3) /*��ʾƽ����ɫ������ť*/
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
/****���ں�����x1��y1Ϊ���Ͻ����꣬x2��y2Ϊ���½�����*/
void wins(int x1,int y1,int x2,int y2)
{
   char lable[][10]={"No","Name","Sex",
   "Profess","Dept", "Class","Workload","LessonF"}; /*��ǩ��Ϣ*/
   int m,n;
   setfillstyle(1,WHITE);/*������*/
   bar(x1,y1,x2,y2);
   setfillstyle(1,LIGHTGRAY);
   bar(x1+1,y1+1,x2,y2);
   setfillstyle(1,BLUE); /*����ɫ������*/
   bar(x1+3,y1+3,x2-3,y1+20);
   cross(y1,x2,0); /*���رհ�ť*/
   outtextxy(x1+5,y1+8,"teacher LessonF");/*�����������*/
   for(m=0;m<4;m++)
      for(n=0;n<2;n++)
      {
	 button(x1+n*250+20,y1+50+m*30+20,x1+n*250+90,
	 y1+50+m*30+40,lable[n+m*2],2);/*��ǩ��Ϣ*/
	 setfillstyle(1,WHITE);
	 bar(x1+n*250+100,y1+50+m*30+20,x1+n*250+200,y1+50+m*30+40);
	 /*��ɫ������ʾ������*/
      }
   settextstyle(0,0,1);
   for(m=0;m<2;m++)
      for(n=0;n<5;n++)
	 button(x1+n*100+10,y1+200+m*30+20,x1+n*100+80,
         y1+200+m*30+40,Str[n+m*5],1); /*��ʾ͹�����ť*/
}
/*�رհ�ť����y1��x2�ֱ��Ǵ��ڵ����Ͻǵ�y��������½ǵ�x���꣬*/
void cross(int y1,int x2,int press)
{
   if(press==1)
      setfillstyle(1,BLACK);
   else
      setfillstyle(1,WHITE);
   bar(x2-17,y1+5,x2-5,y1+18);/*����������*/
   setfillstyle(1,LIGHTGRAY);
   bar(x2-16,y1+6,x2-5,y1+18);
   setwritemode(EMPTY_FILL);/*�ñ�����ɫ���*/
   setcolor(BLACK);
   setlinestyle(0,0,3); /*��������*/
   line(x2-14,y1+8,x2-8,y1+16);/*��������*/
   line(x2-8,y1+8,x2-14,y1+16);
}
/*���������ƴ�����*/
void mousewin()
{
   union REGS inregs,outregs;
   int x1,y1,i,m,n,state;
   int press=0,close=0,cpress=0;
   int winx1=70,winy1=100,winx2=570,winy2=400;/*���ô�������*/
   MouseSetXY(50,50);  /*�������λ��*/
   MouseOn(); /*��ʾ���*/
   wins(winx1,winy1,winx2,winy2);/*��ʾ����������*/
   while(1)
   {
      press=0;/*�Ƿ�ѡ����exit�˵�*/
      cpress=0; /*�Ƿ񵥻������ť*/
      MouseStatus(&x1,&y1,&state);/*�����״̬*/
      while(x1>winx1+3&&x1<winx1+45&&y1>winy1+21&&y1<winy1+35)
      {/*�����main*/
	 if(press==0)
	    button(winx1+3,winy1+21,winx1+45,winy1+35,"main",1); /*͹��*/
	 delay(1000);
	 MouseStatus(&x1,&y1,&state);
	 while(state==1) /*������������*/
	 {
	    settextstyle(0,0,0);
	    button(winx1+3,winy1+21,winx1+45,winy1+35,"main",2);/*����*/
	    MouseStatus(&x1,&y1,&state);
	    delay(1000);
	    press=1;
	 }
	 if(press==1)
	    button(winx1+3,winy1+35,winx1+45,winy1+50,"exit",1);
	 }
	 while(press==1&&(x1>winx1+3&&x1<winx1+45&&y1>winy1+35&&y1<winy1+50))/*�����exit��*/
	 {
	    button(winx1+3,winy1+35,winx1+45,winy1+50,"exit",3);/*��ɫƽ��*/
	    delay(1000);
	    MouseStatus(&x1,&y1,&state);
	    if(state==1) /*������exit�˵����˳�*/
		exit(0);
	 }
	 if(!(x1>winx1+3&&x1<winx1+45&&y1>winy1+21&&y1<winy1+35)
	 &&press==0) /*��겻��main��*/
	 {
	    setfillstyle(1,LIGHTGRAY);
	    bar(winx1+3,winy1+21,winx1+45,winy1+35);
	    settextstyle(0,0,0);
	    button(winx1+3,winy1+21,winx1+45,winy1+35,"main",0);
	    delay(1000);
	 }
	 if(press==1&&!(x1>winx1+3&&
	 x1<winx1+45&&y1>winy1+35&&y1<winy1+50))
	 {  /*��겻��exit��*/
	    settextstyle(0,0,0);
	    button(winx1+3,winy1+35,winx1+45,winy1+50,"exit",1);
	    delay(1000);
	 }
	 if(state==1&&(!(x1>winx1+3&&x1<winx1+45
	 &&y1>winy1+21&&y1<winy1+35)))
	 {/*��main�����ⵥ�������*/
	    setfillstyle(1,LIGHTGRAY);
	    bar(winx1+3,winy1+35,winx1+45,winy1+50);
	    press=0;
	 }
	 while(state==1)
	 {
	    MouseStatus(&x1,&y1,&state);
	    if(x1>winx2-17&&y1>winy1+5&&x1<winx2-5&&y1<winy1+18)
	    {/*����ڹرհ�ť�ϵ���*/
	       cross(winy1,winx2,1);/*͹����ʾ�رհ�ť*/
	       delay(1000);
	       close=1;
	    }
	 }
	 if(state==0)
	 {
	    cross(winy1,winx2,0);/*ƽ����ʾ�رհ�ť*/
	    if(close==1)  exit(0);
	 }
	 while(x1>winx1+5&&y1>winy1+220&&x1<winx2-20&&y1<winy2-20)
	 {
	    if(cpress==0)  /*��������ť��*/
	    {
	       settextstyle(0,0,1);
	       for(m=0;m<2;m++) /*��ʾ���ť*/
		  for(n=0;n<5;n++)
		     button(winx1+n*100+10,winy1+200+m*30+20,winx1+n*100+80,
		     winy1+200+m*30+40,Str[n+m*5],1);
	       delay(1000);
	       MouseStatus(&x1,&y1,&state);
	       if(state==1)/*�жϵ������ĸ����ť*/
	       {
		  settextstyle(0,0,0);
		  if(x1>80&&x1<150)
		     if(y1>320&&y1<340)/*������First���ť*/
			{m=0;n=0;}
		     else
			{m=1;n=0;} /*������Delete���ť*/
		  if(x1>180&&x1<250)
		     if(y1>320&&y1<340)
			{m=0;n=1;} /*������Prior���ť*/
		     else
			{m=1;n=1;} /*������Load���ť*/
		  if(x1>280&&x1<350)
		     if(y1>320&&y1<340)
			{m=0;n=2;} /*������Next���ť*/
		     else
			{m=1;n=2;} /*������Save���ť*/
		  if(x1>380&&x1<450)
		     if(y1>320&&y1<340)
			{m=0;n=3;} /*������Last���ť*/
		     else
			{m=1;n=3;} /*������Sort���ť*/
		  if(x1>480&&x1<550)
		     if(y1>320&&y1<340)
			{m=0;n=4;} /*������Clear���ť*/
		     else
			{m=1;n=4;} /*������Exit���ť*/
	       cpress=1;
	       break;
	       }
	    }
	 }
      if(cpress==1)
      {
	 button(winx1+n*100+10,winy1+200+m*30+20,winx1+n*100+80,
	 winy1+200+m*30+40,Str[n+m*5],2); /*�����İ�ť������ʾ*/
	 run(m,n); /*������Ӧ�Ĺ���*/
      }
   } /*����while��1��*/
}
/********���ܵ��ú���������m��n����ť�к���λ��********/
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
   }/*����switch*/
}
