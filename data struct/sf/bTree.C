/*2. Դ����*/
/********tree2.c��������ʾ********/
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
typedef struct TREE
{
   char data;/*���Ľ������*/
   struct TREE *lchild;
   struct TREE *rchild;
   int x;/*����x����*/
   int y;/*����y����*/
}Tree;
struct OUTPUT
{
   int x;/*���ֱ�����x����*/
   int y;/*���ֱ�����y����*/
   int num;
}s;
int nodeNUM=0;/*ͳ�Ƶ�ǰ�Ľ������,���26��*/
char way;/*�Զ����������ֶ��������ı�־,2�ֶ�,1�Զ�*/
char str[3];/*��ʾ������ݵ��ַ���*/
void Init();/*ͼ�γ�ʼ��*/
void Close();/*ͼ�ιر�*/
Tree *CreatTree();/*�ı�ģʽ�´������Ĺ���*/
Tree *InitTree(int h,int t,int w);/*������,h���,t������,w��֮��Ŀ���,n���Ľ�����ʽ*/
void DrawTree(Tree *t);/*��ͼ����ʾ�����õ���*/
void Preorder(Tree *t);/*ǰ�����*/
void Midorder(Tree *t);/*�������*/
void Posorder(Tree *t);/*�������*/
void DrawNode(Tree *t,int color);/*����ʱ��ʾÿ�����Ĺ���*/
void ClrScr();/*�����������*/
void main()
{
   Tree *root;
   randomize();
   root=CreatTree();/*������*/
   Init();
   DrawTree(root);/*ÿ�α���ǰ��ʾ��ɫ����*/
   sleep(1);
   s.x=100;s.y=300;s.num=1;/*ÿ�α���ǰ������ʾ����˳����ʾ��x,y����*/
   Preorder(root);/*ǰ�����*/
   getch();
   ClrScr();
   DrawTree(root);
   sleep(1);
   s.x=100;
   s.y=350;
   s.num=1;
   Midorder(root);/*�������*/
   getch();
   ClrScr();
   DrawTree(root);
   sleep(1);
   s.x=100;
   s.y=400;
   s.num=1;
   Posorder(root);/*�������*/
   Close();
}
/*�����������*/
void ClrScr()
{
   setcolor(BLACK);
   setfillstyle(SOLID_FILL,BLACK);
   bar(0,20,640,280);
}
/*�ı�ģʽ�´������Ĺ���*/
Tree *CreatTree()
{
   Tree *root;
   clrscr();
   printf("please input n\n");
   printf("1.computer creat\n");
   printf("2.people creat\n");
   way=getch();/*���봴�����ķ���,1�����Զ�����,2�˹��ֶ�����*/
   if(way!='2')
      way='1';/*��������Ĭ���Զ�����*/
   if(way=='2')/*�ֶ�������ʾ������*/
      printf("Please creat the tree\n");
   root=InitTree(1,320,150);
   system("pause");
   return root;
}
/*���ɶ�������h��ʾ��Σ�t��ʾ�����꣬w��ʾ������������Ŀ��ȣ������nȷ������ǿջ�ǿգ���nΪ0����Ϊ��*����Ҫ�޶�ȷ�����������������*/
Tree *InitTree(int h,int t,int w)
{
   char ch;
   int n;/*�Զ�����ʱ�����ֵ�ж��Ƿ���NULL�ı�־*/
   Tree *node;
   if(way=='2')/*�ֶ�������Ҫ�Լ�����*/
      scanf("%c",&ch);
   else/*�Զ������ĸ�ֵ*/
   {
      n=random(5);
      if(n==0&&nodeNUM>=3)/*�����ֵʱ��ȷ���Զ������Ķ��������������*/
	 ch='.';
      else
	 ch=65+random(25);
   }
   if(ch=='.')/*����ո����NULL*/
      return NULL;
   else
   {
      if(h==6||nodeNUM==26)/*������Ĳ���Ѿ���5���߽��������26�����Զ�����NULL*/
	 return NULL;
      node=(Tree*)malloc(sizeof(Tree));
      node->data=ch;
      node->x=t;/*����x�����Ǵ��ݹ����ĺ�����*/
      node->y=h*50;/*����y�������δ�С�й�*/
      nodeNUM++;
      node->lchild=InitTree(h+1,t-w,w/2);
      node->rchild=InitTree(h+1,t+w,w/2);
   }
   return node;
}
/*��ͼ����ʾ�����õ���*/
void DrawTree(Tree *t)
{
   if(t!=NULL)
   {
      setcolor(BLACK);
      setfillstyle(SOLID_FILL,BLACK);
      fillellipse(t->x,t->y,9,9);
      setcolor(WHITE);
      circle(t->x,t->y,10); /*��Բ*/
      sprintf(str,"%c",t->data);/*������ת�����ַ������*/
      outtextxy(t->x-3,t->y-2,str);
      if(t->lchild!=NULL)/*������*/
      {
	 line(t->x-5,t->y+12,t->lchild->x+5,t->lchild->y-12);
	 DrawTree(t->lchild);
      }
      if(t->rchild!=NULL)/*������*/
      {
	 line(t->x+5,t->y+12,t->rchild->x-5,t->rchild->y-12);
	 DrawTree(t->rchild);
      }
   }
}
/*����ʱ��ʾÿ�����Ĺ���*/
void DrawNode(Tree *t,int color)
{
   setcolor(YELLOW);
   setfillstyle(SOLID_FILL,YELLOW);
   fillellipse(t->x,t->y,10,10);
   setcolor(RED);
   sprintf(str,"%c",t->data);/*������ת�����ַ������*/
   outtextxy(t->x-3,t->y-2,str);
   setcolor(color);
   outtextxy(s.x,s.y,str);
   setcolor(RED);
   sprintf(str,"%d",s.num);/*������������������ʾ�����Ľ����*/
   outtextxy(t->x-3,t->y-20,str);
   s.num++;
   sleep(1);
}
/*ǰ�����*/
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
/*�������*/
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
/*�������*/
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
/*ͼ�γ�ʼ��*/
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
/*ͼ�ιر�*/
void Close()
{
   getch();
   closegraph();
}
