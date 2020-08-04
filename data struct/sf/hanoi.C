/*5. Դ����*/
/********hanoi.c*********/
#include <graphics.h>
struct H
{
   int data[15];/*���ÿ���̵Ĵ���*/
   int top;/*ÿ�����ľ���߶�*/
}num[3];/*������*/
void move(char x,char y,struct H num[3]);/*�ƶ��ľ������*/
void hanoi(char x,char y,char z,int n,struct H num[3]);/*�ݹ�*/
void Init(void);/*��ʼ��*/
void Close(void);/*ͼ�ιر�*/
int computer=1;/*�Զ��������ֶ����Ƶı�־*/
int speed=0;/*ȫ�ֱ���speed��Ҫ����ʾ���̵��ٶ�*/
void main(void)
{
   Init();/*��ʼ״̬*/
   Close();/*ͼ�ιر�*/
   exit(0);
}
void Init(void)/*��ʼ��*/
{
   int gd=DETECT,gm;
   int i,n,color;
   clrscr();
   printf("please input n(n<=10): ");/*����Ҫ��ʾ��������*/
   scanf("%d",&n);
   printf("Please input 1 or 2:\n1.computer 2.people\n");
   scanf("%d",&i);
   if(i==2)/*ѡ���ֶ����Ʊ�־Ϊ0*/
      computer=0;
   if(n<1||n>10)
      n=10;/*Խ��Ļ�n��10����*/
   if(computer)/*������Զ����ƵĻ������ٶ�*/
   {
      printf("please input speed: ");/*�����ٶ�*/
      scanf("%d",&speed);
   }
   initgraph(&gd,&gm,"c:\\tc");
   cleardevice();
   for(i=0;i<3;i++)
      num[i].top=-1;/*�����ط��ĸ߶ȿ�ʼ��Ϊ-1*/
   for(i=0;i<n;i++)/*��һ��ʼ������A�ϵ�����*/
   {
      num[0].top++;/*ջ�ĸ߶ȼ�1*/
      num[0].data[num[0].top]=i; /*�������Ӵ���Ϊ0������Ϊ1��2����n-1*/
      color=num[0].data[num[0].top]+1;/*���ӵ���ɫ����Ϊջ�����Ӵ��ż�1*/
      setfillstyle(SOLID_FILL,color);
      bar(100-(33-3*num[0].data[num[0].top]),400-20*i-8,100+
(33-3*num[0].data[num[0].top]),400-20*i+8); /*������*/
   }
   setcolor(YELLOW);
   outtextxy(180,450,"any key to continue");
   settextstyle(0,0,2);
   outtextxy(90,420,"A"); /*������־*/
   outtextxy(240,420,"B");
   outtextxy(390,420,"C");
   getch();/*�����ַ����ִ�еݹ����*/
   hanoi('a','b','c',n,num);
}
void move(char x,char y,struct H num[3])/*�ƶ��ľ������*/
{
   int i;
   char num1[3],num2[3];
   sprintf(num1,"%c",x-32);/*��Сд��ɴ�д����ת�����ַ������*/
   sprintf(num2,"%c",y-32);
   setfillstyle(SOLID_FILL,BLACK);/*��ԭ���ĵط���ȥͿ��*/
   bar(0,0,640,60);
   setcolor(RED);
   outtextxy(150,30,num1);/*����ƶ�����*/
   outtextxy(200,30,"--->");
   outtextxy(310,30,num2);
   settextstyle(0,0,2);
   setfillstyle(SOLID_FILL,BLACK);/*��ԭ���ĵط���ȥͿ��*/
   bar(100+150*(x-97)-(33-3*num[x-97].data[num[x-97].top]),
400-20*num[x-97].top-8,100+150*(x-97)+(33-3*
num[x-97].data[num[x-97].top]),400-20*num[x-97].top+8);
   num[y-97].top++;/*��ջ��Ŀ����top��1*/
   num[y-97].data[num[y-97].top]=num[x-97].data[num[x-97].top];/*��Ŀ������ӵĴ�����Դ�����ӵĴ�����ͬ*/
   num[x-97].top--;/*��ջ��ԭ���ط���top��1*/
   setfillstyle(SOLID_FILL,num[y-97].data[num[y-97].top]+1);/*������ɫ������ջ�����Ӵ��ż�1*/
   bar(100+150*(y-97)-(33-3*num[y-97].data[num[y-97].top]),
400-20*num[y-97].top-8,100+150*(y-97)+
(33-3*num[y-97].data[num[y-97].top]),400-20*num[y-97].top+8);
   if(computer)/*�Զ����ƾ���delay*/
      delay(speed);/*��ʱ����*/
   else
      getch();/*�ֶ����ƵĻ����Լ�������������*/
}
void hanoi(char one,char two,char three,int n,struct H num[3])/*�ݹ�nΪ��������numΪ��ջ*/
{
   if(n==1)
      move(one,three,num);/*�������Ϊ1����������Ӵ�����A�ƶ�������C*/
   else
   {
      hanoi(one,three,two,n-1,num);/*������A��ǰn-1�������Ƶ�����B*/
      move(one,three,num);/*������A�ĵ�n�������Ƶ�����C*/
      hanoi(two,one,three,n-1,num); /*������B��n-1�������Ƶ�����C*/
   }
}
void Close(void)/*ͼ�ιر�*/
{
   getch();
   closegraph();
}
