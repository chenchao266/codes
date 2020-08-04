/*2. 源程序*/
/********tree2.c二叉树演示********/
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
typedef struct TREE
{
   char data;/*树的结点数据*/
   struct TREE *lchild;
   struct TREE *rchild;
   int x;/*树的x坐标*/
   int y;/*树的y坐标*/
}Tree;
struct OUTPUT
{
   int x;/*三种遍历的x坐标*/
   int y;/*三种遍历的y坐标*/
   int num;
}s;
int nodeNUM=0;/*统计当前的结点数字,最多26个*/
char way;/*自动建立树和手动建立树的标志,2手动,1自动*/
char str[3];/*显示结点数据的字符串*/
void Init();/*图形初始化*/
void Close();/*图形关闭*/
Tree *CreatTree();/*文本模式下创建树的过程*/
Tree *InitTree(int h,int t,int w);/*创建树,h层次,t横坐标,w树之间的宽度,n树的建立方式*/
void DrawTree(Tree *t);/*用图形显示创建好的树*/
void Preorder(Tree *t);/*前序遍历*/
void Midorder(Tree *t);/*中序遍历*/
void Posorder(Tree *t);/*后序遍历*/
void DrawNode(Tree *t,int color);/*遍历时显示每个结点的过程*/
void ClrScr();/*清空树的区域*/
void main()
{
   Tree *root;
   randomize();
   root=CreatTree();/*创建树*/
   Init();
   DrawTree(root);/*每次遍历前显示白色的树*/
   sleep(1);
   s.x=100;s.y=300;s.num=1;/*每次遍历前设置显示遍历顺序显示的x,y坐标*/
   Preorder(root);/*前序遍历*/
   getch();
   ClrScr();
   DrawTree(root);
   sleep(1);
   s.x=100;
   s.y=350;
   s.num=1;
   Midorder(root);/*中序遍历*/
   getch();
   ClrScr();
   DrawTree(root);
   sleep(1);
   s.x=100;
   s.y=400;
   s.num=1;
   Posorder(root);/*后序遍历*/
   Close();
}
/*清空树的区域*/
void ClrScr()
{
   setcolor(BLACK);
   setfillstyle(SOLID_FILL,BLACK);
   bar(0,20,640,280);
}
/*文本模式下创建树的过程*/
Tree *CreatTree()
{
   Tree *root;
   clrscr();
   printf("please input n\n");
   printf("1.computer creat\n");
   printf("2.people creat\n");
   way=getch();/*输入创建树的方法,1电脑自动建立,2人工手动建立*/
   if(way!='2')
      way='1';/*其他数字默认自动建立*/
   if(way=='2')/*手动建立提示输入结点*/
      printf("Please creat the tree\n");
   root=InitTree(1,320,150);
   system("pause");
   return root;
}
/*生成二叉树，h表示层次，t表示横坐标，w表示结点左右子树的宽度，随机数n确定结点是空或非空，如n为0，则为空*，但要限定确保结点数不少于三个*/
Tree *InitTree(int h,int t,int w)
{
   char ch;
   int n;/*自动建立时随机赋值判断是否是NULL的标志*/
   Tree *node;
   if(way=='2')/*手动建立需要自己输入*/
      scanf("%c",&ch);
   else/*自动建立的赋值*/
   {
      n=random(5);
      if(n==0&&nodeNUM>=3)/*随机赋值时候确保自动建立的二叉树有三个结点*/
	 ch='.';
      else
	 ch=65+random(25);
   }
   if(ch=='.')/*输入空格代表NULL*/
      return NULL;
   else
   {
      if(h==6||nodeNUM==26)/*如果树的层次已经到5或者结点树到达26个就自动返回NULL*/
	 return NULL;
      node=(Tree*)malloc(sizeof(Tree));
      node->data=ch;
      node->x=t;/*树的x坐标是传递过来的横坐标*/
      node->y=h*50;/*树的y坐标与层次大小有关*/
      nodeNUM++;
      node->lchild=InitTree(h+1,t-w,w/2);
      node->rchild=InitTree(h+1,t+w,w/2);
   }
   return node;
}
/*用图形显示创建好的树*/
void DrawTree(Tree *t)
{
   if(t!=NULL)
   {
      setcolor(BLACK);
      setfillstyle(SOLID_FILL,BLACK);
      fillellipse(t->x,t->y,9,9);
      setcolor(WHITE);
      circle(t->x,t->y,10); /*画圆*/
      sprintf(str,"%c",t->data);/*将内容转换成字符串输出*/
      outtextxy(t->x-3,t->y-2,str);
      if(t->lchild!=NULL)/*左子树*/
      {
	 line(t->x-5,t->y+12,t->lchild->x+5,t->lchild->y-12);
	 DrawTree(t->lchild);
      }
      if(t->rchild!=NULL)/*右子树*/
      {
	 line(t->x+5,t->y+12,t->rchild->x-5,t->rchild->y-12);
	 DrawTree(t->rchild);
      }
   }
}
/*遍历时显示每个结点的过程*/
void DrawNode(Tree *t,int color)
{
   setcolor(YELLOW);
   setfillstyle(SOLID_FILL,YELLOW);
   fillellipse(t->x,t->y,10,10);
   setcolor(RED);
   sprintf(str,"%c",t->data);/*将内容转换成字符串输出*/
   outtextxy(t->x-3,t->y-2,str);
   setcolor(color);
   outtextxy(s.x,s.y,str);
   setcolor(RED);
   sprintf(str,"%d",s.num);/*将遍历次序用数字显示在树的结点上*/
   outtextxy(t->x-3,t->y-20,str);
   s.num++;
   sleep(1);
}
/*前序遍历*/
void Preorder(Tree *t)
{
   if(t!=NULL)
   {
      s.x+=15;
      DrawNode(t,GREEN);
      Preorder(t->lchild);
      Preorder(t->rchild);
   }
}
/*中序遍历*/
void Midorder(Tree *t)
{
   if(t!=NULL)
   {
      Midorder(t->lchild);
      s.x+=15;
      DrawNode(t,YELLOW);
      Midorder(t->rchild);
   }
}
/*后序遍历*/
void Posorder(Tree *t)
{
   if(t!=NULL)
   {
      Posorder(t->lchild);
      Posorder(t->rchild);
      s.x+=15;
      DrawNode(t,BLUE);
   }
}
/*图形初始化*/
void Init()
{
   int gd=DETECT,gm;
   initgraph(&gd,&gm,"c:\\tc");
   cleardevice();
   setcolor(YELLOW);
   outtextxy(250,10,"anykey to continue");
   setcolor(RED);
   outtextxy(20,300,"preorder");
   outtextxy(20,350,"midorder");
   outtextxy(20,400,"posorder");
   getch();
}
/*图形关闭*/
void Close()
{
   getch();
   closegraph();
}
