/*3. Դ����*/
/***radix.c *****/
#define N 52
#include <graphics.h>
#include <stdlib.h>
void Init(void);/*ͼ������*/
void Close(void);/*ͼ�ιر�*/
void Play(void);/*���Ƶľ������*/
void Rand(int i,int j);/*������ƺ���*/
void Sort();
void DrawPuke(int kind,int x,int y,char num);/*����*/
char p[4][13]=
      {{'2','3','4','5','6','7','8','9','0','J','Q','K','A'},
       {'2','3','4','5','6','7','8','9','0','J','Q','K','A'},
       {'2','3','4','5','6','7','8','9','0','J','Q','K','A'},
       {'2','3','4','5','6','7','8','9','0','J','Q','K','A'}};
/*10��0����ʾ*/
struct Pai
{
   char num;
   char kind;
   int realnum;
   int link;
}newp[52];
/****������*****/
void main(void)
{
   Init();/*��ʼ��*/
   Play();
   getch();
   cleardevice(); /*����*/
   Sort(); /*����*/
   Close(); /*�ر�*/
   exit(0);
}
/**������ƺ�����i��j�����к��У���4��13��*/
void Rand(int i,int j)
{
   int kind,num;
   char n;
   randomize();/*���������*/
   while(1)/*ѭ�������ǿ������Ʒ�Ϊֹ*/
   {
      kind=random(4);/*����4���ڵ������*/
      num=random(13); /*����13���ڵ������*/
      if(p[kind][num]!=-1)/*�Ʒ����Ժ���Ӧλ�õ�Ԫ����-1*/
      {
	 n=p[kind][num];/*���˿�����ȡ��*/
	 p[kind][num]=-1;
	 newp[j*4+i].kind=kind;/*���ƵĻ�ɫ����*/
	 newp[j*4+i].num=n; /*���Ƶ���ֵ���ű���*/
	 newp[j*4+i].realnum=num; /*���ƵĶ�Ӧʮ������ֵ����*/
	 break;
      }
   }
   DrawPuke(kind,i,j,n);/*��ʾ��*/
}
/*���ƺ�����kind��ɫ�� i�����У�j�����У�num��ʾ��ֵ���� */
void DrawPuke(int kind,int i,int j,char num)
{
   char str[3];
   bar(50+j*45-15,50+i*100-30,50+j*45+15,50+i*100+30);/*������*/
   setcolor(BLUE);
   rectangle(50+j*45-13,50+i*100-28,50+j*45+13,50+i*100+28);
   switch(kind)/*��ʽ���ж�*/
   {
      case 0:setcolor(BLACK);sprintf(str,"%c",3);break;/* a����*/
      case 1:setcolor(RED);sprintf(str,"%c",3);break;/* ?����*/
      case 2:setcolor(RED);sprintf(str,"%c",4);break;/* ������*/
      case 3:setcolor(BLACK);sprintf(str,"%c",5);break;/* ��ݻ�*/
   }
   settextstyle(0,0,1);
   outtextxy(50+j*45-11,50+i*100-26,str);/*��ʾ�Ƶ����Ͻǻ�ɫ*/
   outtextxy(50+j*45+5,50+i*100+20,str); /*��ʾ�Ƶ����½ǻ�ɫ*/
   if(num!='0')/*���������*/
   {
      settextstyle(0,0,2);
      sprintf(str,"%c",num);
      outtextxy(50+j*45-5,50+i*100-5,str);/*��ʾ�ƵĴ�С*/
   }
   else/*���10��ʱ��*/
   {
      sprintf(str,"%d",10);
      outtextxy(50+j*45-6,50+i*100-5,str);
   }
}
void Play(void)/*���Ƶľ������*/
{
   int i,j;
   for(j=0;j<13;j++)
   {
      for(i=0;i<4;i++)
      {
	 Rand(i,j);/*�������*/
	 delay(10000);/*��ʱ*/
      }
   }
}
/*��������*/
void Sort()
{
   int i,j,k,t,p,f[13],e[13];
   for(i=0;i<N;i++) /*��52����������*/
      newp[i].link=i+1;
   newp[N-1].link=-1;
   for(i=0;i<4;i++)/*��ʼ������*/
   {
      f[i]=-1;
      e[i]=0;
   }
   p=0; /*��һ�η���*/
   do{
      k=newp[p].kind;
      if(f[k]==-1)
         f[k]=p;
      else
         newp[e[k]].link=p;
      e[k]=p;
      p=newp[p].link;
   }while(p!=-1);
   j=0;  /*��һ���ռ�,��52������������*/
   p=f[j];
   t=e[j];
   for(k=j+1;k<4;k++)
   {
      newp[t].link=f[k];
      t=e[k];
   }
   newp[t].link=-1;
   cleardevice();
   for(i=0;i<4;i++) /*�����һ�η�����*/
   {
      p=f[i];
      for(j=0;j<13;j++)
      {
	 DrawPuke(i, i, j,newp[p].num);
	 p=newp[p].link;
      }
   }
   getch();
   cleardevice();
   p=f[0];/*�����һ����������ݵ���ʼλ�ã����еڶ��η���*/
   for(i=0;i<13;i++)/*��ʼ������*/
   {
      f[i]=-1;
      e[i]=0;
   }
   do{
      k=newp[p].realnum;
      if(f[k]==-1)
	 f[k]=p;
      else
	 newp[e[k]].link=p;
      e[k]=p;
      p=newp[p].link;
   }while(p!=-1);
   j=0;  /*�ڶ����ռ�,��52������������*/
   p=f[j];
   t=e[j];
   for(k=j+1;k<13;k++)
   {
      newp[t].link=f[k];
      t=e[k];
   }
   newp[t].link=-1;
   for(j=0;j<13;j++) /*����ڶ��η�����*/
   {
      p=f[j];
      for(i=0;i<4;i++)
      {
	 DrawPuke(i, i, j,newp[p].num);
	 p=newp[p].link;
      }
   }
}
/*ͼ������*/
void Init(void)
{
   int gd=DETECT,gm;
   initgraph(&gd,&gm,"c:\\tc");
   cleardevice();
}
/*ͼ�ιر�*/
void Close(void)
{
   getch();
   closegraph();
}
