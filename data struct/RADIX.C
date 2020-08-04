/*3. 源程序*/
/***radix.c *****/
#define N 52
#include <graphics.h>
#include <stdlib.h>
void Init(void);/*图形驱动*/
void Close(void);/*图形关闭*/
void Play(void);/*发牌的具体过程*/
void Rand(int i,int j);/*随机发牌函数*/
void Sort();
void DrawPuke(int kind,int x,int y,char num);/*画牌*/
char p[4][13]=
      {{'2','3','4','5','6','7','8','9','0','J','Q','K','A'},
       {'2','3','4','5','6','7','8','9','0','J','Q','K','A'},
       {'2','3','4','5','6','7','8','9','0','J','Q','K','A'},
       {'2','3','4','5','6','7','8','9','0','J','Q','K','A'}};
/*10用0来表示*/
struct Pai
{
   char num;
   char kind;
   int realnum;
   int link;
}newp[52];
/****主函数*****/
void main(void)
{
   Init();/*初始化*/
   Play();
   getch();
   cleardevice(); /*清屏*/
   Sort(); /*排序*/
   Close(); /*关闭*/
   exit(0);
}
/**随机发牌函数，i和j代表行和列，共4行13列*/
void Rand(int i,int j)
{
   int kind,num;
   char n;
   randomize();/*随机种子数*/
   while(1)/*循环条件是可以有牌发为止*/
   {
      kind=random(4);/*生成4以内的随机数*/
      num=random(13); /*生成13以内的随机数*/
      if(p[kind][num]!=-1)/*牌发好以后相应位置的元素置-1*/
      {
	 n=p[kind][num];/*从扑克牌中取数*/
	 p[kind][num]=-1;
	 newp[j*4+i].kind=kind;/*将牌的花色保存*/
	 newp[j*4+i].num=n; /*将牌的面值符号保存*/
	 newp[j*4+i].realnum=num; /*将牌的对应十进制数值保存*/
	 break;
      }
   }
   DrawPuke(kind,i,j,n);/*显示牌*/
}
/*画牌函数，kind花色， i代表行，j代表列，num表示面值符号 */
void DrawPuke(int kind,int i,int j,char num)
{
   char str[3];
   bar(50+j*45-15,50+i*100-30,50+j*45+15,50+i*100+30);/*画空牌*/
   setcolor(BLUE);
   rectangle(50+j*45-13,50+i*100-28,50+j*45+13,50+i*100+28);
   switch(kind)/*花式的判断*/
   {
      case 0:setcolor(BLACK);sprintf(str,"%c",3);break;/* a黑桃*/
      case 1:setcolor(RED);sprintf(str,"%c",3);break;/* ?红心*/
      case 2:setcolor(RED);sprintf(str,"%c",4);break;/* ¨方块*/
      case 3:setcolor(BLACK);sprintf(str,"%c",5);break;/* §草花*/
   }
   settextstyle(0,0,1);
   outtextxy(50+j*45-11,50+i*100-26,str);/*显示牌的左上角花色*/
   outtextxy(50+j*45+5,50+i*100+20,str); /*显示牌的右下角花色*/
   if(num!='0')/*输出其它牌*/
   {
      settextstyle(0,0,2);
      sprintf(str,"%c",num);
      outtextxy(50+j*45-5,50+i*100-5,str);/*显示牌的大小*/
   }
   else/*输出10的时候*/
   {
      sprintf(str,"%d",10);
      outtextxy(50+j*45-6,50+i*100-5,str);
   }
}
void Play(void)/*发牌的具体过程*/
{
   int i,j;
   for(j=0;j<13;j++)
   {
      for(i=0;i<4;i++)
      {
	 Rand(i,j);/*随机发牌*/
	 delay(10000);/*延时*/
      }
   }
}
/*基数排序*/
void Sort()
{
   int i,j,k,t,p,f[13],e[13];
   for(i=0;i<N;i++) /*将52张牌链起来*/
      newp[i].link=i+1;
   newp[N-1].link=-1;
   for(i=0;i<4;i++)/*初始化队列*/
   {
      f[i]=-1;
      e[i]=0;
   }
   p=0; /*第一次分配*/
   do{
      k=newp[p].kind;
      if(f[k]==-1)
         f[k]=p;
      else
         newp[e[k]].link=p;
      e[k]=p;
      p=newp[p].link;
   }while(p!=-1);
   j=0;  /*第一次收集,将52张牌链接起来*/
   p=f[j];
   t=e[j];
   for(k=j+1;k<4;k++)
   {
      newp[t].link=f[k];
      t=e[k];
   }
   newp[t].link=-1;
   cleardevice();
   for(i=0;i<4;i++) /*输出第一次分配结果*/
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
   p=f[0];/*保存第一次排序后数据的起始位置，进行第二次分配*/
   for(i=0;i<13;i++)/*初始化队列*/
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
   j=0;  /*第二次收集,将52张牌链接起来*/
   p=f[j];
   t=e[j];
   for(k=j+1;k<13;k++)
   {
      newp[t].link=f[k];
      t=e[k];
   }
   newp[t].link=-1;
   for(j=0;j<13;j++) /*输出第二次分配结果*/
   {
      p=f[j];
      for(i=0;i<4;i++)
      {
	 DrawPuke(i, i, j,newp[p].num);
	 p=newp[p].link;
      }
   }
}
/*图形驱动*/
void Init(void)
{
   int gd=DETECT,gm;
   initgraph(&gd,&gm,"c:\\tc");
   cleardevice();
}
/*图形关闭*/
void Close(void)
{
   getch();
   closegraph();
}
