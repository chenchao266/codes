/*2. Դ����*/
/***dblink.c***/
#include <stdlib.h>
#include <graphics.h>
typedef struct Link/*˫�����㶨��*/
{
   int data;
   struct Link *right;
   struct Link *left;
   int num;/*����������,Ϊ����ʾʱ������ȷλ��*/
}linkx,*linky;
void Init(void);/*ͼ������*/
void Close(void);/*ͼ�ιر�*/
void InitList(void);/*����˫����,�߽����߲���*/
void PrLink(linky p,int n);/*ÿ�β�����������*/
void DrawChange(int data,int i,int n);/*����������ָ��仯*/
void main(void)
{
   Init();/*ͼ�ιر�*/
   InitList();/*��������*/
   Close();/*ͼ�ιر�*/
   exit(0);
}
void InitList(void) /*����˫������,�߽����߲���*/
{
   linky head,p,q,s;
   int n=0;
   char str[5];
   randomize();/*�����������*/
   setcolor(YELLOW);
   outtextxy(250,20,"any key to continue");
   getch();
   head=s=(linky)malloc(sizeof(linkx));
   s->data=random(100);/*�������100���ڵ�����*/
   s->num=n;
   sprintf(str,"%d",s->data);/*������ת�����ַ��������*/
   settextstyle(0,0,2);
   setcolor(11);
   outtextxy(50+n*70,50,str);/*��ʾ����*/
   sleep(1);
   s->right=NULL;
   s->left=NULL;
   PrLink(head,n);/*ÿ�β��������ֺ���ʾ��ǰ����*/
   n++;
   while(n!=7)
   {
      s=(linky)malloc(sizeof(linkx));
      s->data=random(100);
      s->num=n;
      sprintf(str,"%d",s->data);/*������ת�����ַ��������*/
      setcolor(11);
      outtextxy(50+n*70,50,str);
      sleep(1);
      p=head;/*ÿ����һ����㣬���ý����뵽����˫������*/
      if(s->data<=head->data)/*С��ͷ��㣬����ͷ*/
      {
	 DrawChange(s->data,-1,n);/*��ʾ����ľ������*/
	 s->right=head;
	 s->left=NULL;
	 s->num=0;
	 head->left=s;
	 head=s;
	 q=head->right;/*��������������Ŷ���1���൱�����ݺ���*/
	 while(q!=NULL)
	 {
	    q->num++;
	    q=q->right;
	 }
      }
    else/*�������*/
    {
      while(s->data>p->data&&p!=NULL)
      {
	 q=p;
	 p=p->right;
      }
      if(p==NULL)/*������ǵ�ǰ������������β��*/
      {
	 DrawChange(s->data,n-1,n);/*��ʾ����ľ������*/
	 q->right=s;
	 s->right=NULL;
	 s->left=q;
	 s->num=n;
      }
      else  /*������λ��λ������֮��*/
      {
	 q->right->left=s;
	 s->right=q->right;
	 s->left=q;
	 q->right=s;
	 s->num=q->num+1;
	 DrawChange(s->data,q->num,n);/*��ʾ����ľ������*/
	 /*��������������Ŷ���1*/
	 s=s->right;
	 while(s!=NULL)
	 {
	    s->num++;
	    s=s->right;
	 }
      }
   }
   PrLink(head,n);/*ÿ�β��������ݺ���ʾ������*/
   n++;
   }
}
/*���������ľ�����̣�data��Ҫ��������ݣ�iΪ������ǰ�������ţ�nΪ��ǰ���������Ƚ�ǰ�����ͺ��֮���ָ���߲�������ʾ�½�������̣���������������̣��ָ�ɾ����ǰ������ָ����*/
void DrawChange(int data,int i,int n)
{
   char str[5];
   setfillstyle(SOLID_FILL,BLACK);
   setcolor(RED);/*����������������ú�ɫ��ʾ*/
   sprintf(str,"%d",data);
   outtextxy(50+70*i+35,100+n*50,str);/*��������λ��*/
   bar(50+70*i+35,100+(n-1)*50-20,50+70*i+65,100+(n-1)*50+20);
   /*ȥ��������λ��ԭ�����ָ����*/
   setcolor(YELLOW);
   if(i!=-1) /*���ǲ���ͷ���½���ǰ��ָ����*/
   {
      line(50+70*i+34,100+n*50,50+70*i+30,100+n*50);
      line(50+70*i+30,100+n*50,50+70*i+30,100+n*50-25);
      line(50+70*i+30,100+n*50-25,50+70*i+27,100+n*50-22);
      line(50+70*i+30,100+n*50-25,50+70*i+33,100+n*50-22);
      sleep(1);
   }
   if(i!=n-1)/*���ǲ���β���½��ĺ��ָ����*/
   {
      line(50+70*i+61,100+n*50,50+70*i+65,100+n*50);
      line(50+70*i+65,100+n*50,50+70*i+65,100+n*50-25);
      line(50+70*i+65,100+n*50-25,50+70*i+62,100+n*50-22);
      line(50+70*i+65,100+n*50-25,50+70*i+68,100+n*50-22);
      sleep(1);
   }
   setcolor(6);
   if(i!=-1)/*���ǲ���ͷ���½��ǰ�����ĺ��ָ����*/
   {
      line(50+70*i+20,100+n*50-25,50+70*i+20,110+n*50);
      line(50+70*i+20,110+n*50,50+70*i+34,110+n*50);
      line(50+70*i+34,110+n*50,50+70*i+31,110+n*50-3);
      line(50+70*i+34,110+n*50,50+70*i+31,110+n*50+3);
      sleep(1);
   }
   if(i!=n-1) /*���ǲ���β���½���̽���ǰ��ָ����*/
   {
      line(50+70*i+75,100+n*50-25,50+70*i+75,110+n*50);
      line(50+70*i+75,110+n*50,50+70*i+61,110+n*50);
      line(50+70*i+61,110+n*50,50+70*i+64,110+n*50-3);
      line(50+70*i+61,110+n*50,50+70*i+64,110+n*50+3);
   }
   sleep(2);
   setcolor(WHITE);
   if(i!=n-1&&i!=-1)/*��һ���ڵ�����һ����㲻�ָ�ָ��*/
   {
      line(50+70*i+35,100+(n-1)*50,50+70*i+65,100+(n-1)*50);/*��ǰ��ָ��*/
      line(50+70*i+35,100+(n-1)*50,50+70*i+40,95+(n-1)*50);
      line(50+70*i+35,110+(n-1)*50,50+70*i+65,110+(n-1)*50);/*����ָ��*/
      line(50+70*i+65,110+(n-1)*50,50+70*i+60,115+(n-1)*50);
   }
   bar(0,100+(n-1)*50+21,640,120+n*50);/*����������̵�ָ����*/
}
void PrLink(linky p,int n)/*ÿ�β�����������*/
{
   char str[5];
   while(p!=NULL)/*��Ϊ�վ����*/
   {
      sprintf(str,"%d",p->data);
      setcolor(GREEN);
      outtextxy(50+70*p->num,100+n*50,str); /*�������*/
      setcolor(WHITE);
      if(p->left!=NULL)/*��һ���ڵ㲻��ʾ��ָ��*/
      {
	 line(50+70*(p->num-1)+35,100+n*50,50+70*(p->num-1)+65,
	     100+n*50);/*����ָ��*/
	 line(50+70*(p->num-1)+35,100+n*50,50+70*(p->num-1)+
	     40,95+n*50);
      }
      if(p->right!=NULL)
      {
	 line(50+70*p->num+35,110+n*50,50+70*p->num+65,110+n*50);/*����ָ��*/
	 line(50+70*p->num+65,110+n*50,50+70*p->num+60,115+n*50);
      }
      p=p->right;
   }
}
void Init(void)/*ͼ������*/
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
