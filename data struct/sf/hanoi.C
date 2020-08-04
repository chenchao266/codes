/*5. 源程序*/
/********hanoi.c*********/
#include <graphics.h>
struct H
{
   int data[15];/*存放每个盘的代号*/
   int top;/*每个塔的具体高度*/
}num[3];/*三个塔*/
void move(char x,char y,struct H num[3]);/*移动的具体过程*/
void hanoi(char x,char y,char z,int n,struct H num[3]);/*递归*/
void Init(void);/*初始化*/
void Close(void);/*图形关闭*/
int computer=1;/*自动控制与手动控制的标志*/
int speed=0;/*全局变量speed主要是演示过程的速度*/
void main(void)
{
   Init();/*初始状态*/
   Close();/*图形关闭*/
   exit(0);
}
void Init(void)/*初始化*/
{
   int gd=DETECT,gm;
   int i,n,color;
   clrscr();
   printf("please input n(n<=10): ");/*输入要演示的盘子数*/
   scanf("%d",&n);
   printf("Please input 1 or 2:\n1.computer 2.people\n");
   scanf("%d",&i);
   if(i==2)/*选择手动控制标志为0*/
      computer=0;
   if(n<1||n>10)
      n=10;/*越界的话n当10处理*/
   if(computer)/*如果是自动控制的话输入速度*/
   {
      printf("please input speed: ");/*输入速度*/
      scanf("%d",&speed);
   }
   initgraph(&gd,&gm,"c:\\tc");
   cleardevice();
   for(i=0;i<3;i++)
      num[i].top=-1;/*三个地方的高度开始都为-1*/
   for(i=0;i<n;i++)/*画一开始的塔座A上的盘子*/
   {
      num[0].top++;/*栈的高度加1*/
      num[0].data[num[0].top]=i; /*最大的盘子代号为0，依次为1，2，…n-1*/
      color=num[0].data[num[0].top]+1;/*盘子的颜色代码为栈顶盘子代号加1*/
      setfillstyle(SOLID_FILL,color);
      bar(100-(33-3*num[0].data[num[0].top]),400-20*i-8,100+
(33-3*num[0].data[num[0].top]),400-20*i+8); /*画矩形*/
   }
   setcolor(YELLOW);
   outtextxy(180,450,"any key to continue");
   settextstyle(0,0,2);
   outtextxy(90,420,"A"); /*塔座标志*/
   outtextxy(240,420,"B");
   outtextxy(390,420,"C");
   getch();/*接收字符后就执行递归操作*/
   hanoi('a','b','c',n,num);
}
void move(char x,char y,struct H num[3])/*移动的具体过程*/
{
   int i;
   char num1[3],num2[3];
   sprintf(num1,"%c",x-32);/*将小写变成大写，并转换成字符串输出*/
   sprintf(num2,"%c",y-32);
   setfillstyle(SOLID_FILL,BLACK);/*把原来的地方移去涂黑*/
   bar(0,0,640,60);
   setcolor(RED);
   outtextxy(150,30,num1);/*输出移动过程*/
   outtextxy(200,30,"--->");
   outtextxy(310,30,num2);
   settextstyle(0,0,2);
   setfillstyle(SOLID_FILL,BLACK);/*把原来的地方移去涂黑*/
   bar(100+150*(x-97)-(33-3*num[x-97].data[num[x-97].top]),
400-20*num[x-97].top-8,100+150*(x-97)+(33-3*
num[x-97].data[num[x-97].top]),400-20*num[x-97].top+8);
   num[y-97].top++;/*入栈，目标点的top加1*/
   num[y-97].data[num[y-97].top]=num[x-97].data[num[x-97].top];/*在目标点盘子的代号与源点盘子的代号相同*/
   num[x-97].top--;/*出栈，原来地方的top减1*/
   setfillstyle(SOLID_FILL,num[y-97].data[num[y-97].top]+1);/*盘子颜色代码是栈顶盘子代号加1*/
   bar(100+150*(y-97)-(33-3*num[y-97].data[num[y-97].top]),
400-20*num[y-97].top-8,100+150*(y-97)+
(33-3*num[y-97].data[num[y-97].top]),400-20*num[y-97].top+8);
   if(computer)/*自动控制就用delay*/
      delay(speed);/*延时函数*/
   else
      getch();/*手动控制的话就自己按键盘来控制*/
}
void hanoi(char one,char two,char three,int n,struct H num[3])/*递归n为盘子数，num为堆栈*/
{
   if(n==1)
      move(one,three,num);/*如果盘子为1，将这个盘子从塔座A移动到塔座C*/
   else
   {
      hanoi(one,three,two,n-1,num);/*将塔座A的前n-1个盘子移到塔座B*/
      move(one,three,num);/*将塔座A的第n个盘子移到塔座C*/
      hanoi(two,one,three,n-1,num); /*将塔座B的n-1个盘子移到塔座C*/
   }
}
void Close(void)/*图形关闭*/
{
   getch();
   closegraph();
}
