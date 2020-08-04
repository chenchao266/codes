/*13. book.cԴ����*/
#include <dos.h>
#include <bios.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LEFT 0x4b00  /*��*/
#define RIGHT 0x4d00 /*��*/
#define DOWN 0x5000 /*��*/
#define UP 0x4800 /*��*/
#define SPACE 0x3920 /*�ո�*/
#define ESC 0x011b /* ESC��*/
#define ENTER 0x1c0d /*�س���*/
#define Backspace 0xe08 /*������*/
#define ALT_B 12288 /*��ϼ�ALT_B */
#define ALT_M 12800/*��ϼ�ALT_M */
#define ALT_H 8960/*��ϼ�ALT_H */
int key;/*��������*/
int textx,texty;/*�������,x��,y��*/
struct menustruct/*�˵��õĽṹ��*/
{
   char name[10];/*���˵���*/
   char str[10][20];/*ѡ��*/
   int n;/*ѡ����*/
}ml[3];/*ʹ����3�����ɸ�����Ҫ��ɾ*/
typedef struct BookList/*��Ľṹ��*/
{
   char num[20];/*ͼ����*/
   char name[20];/*����*/
   int price;/*��ļ۸�*/
   char person[20];/*������*/
   int yes;/*�ж����Ƿ���ڻ����Ѿ����,1����,0���*/
   struct BookList *next;
}Book;
typedef struct MemberList/*��Ա�Ľṹ��*/
{
   char name[20];/*��Ա������*/
   char sex[2];/*��Ա���Ա�*/
   int age;/*��Ա������*/
   struct MemberList *next;
}Member;
char save[4096];/*�����ı�����ռ�*/
/*char sav1[4096];*/
char c[4096];/*����ר�ÿռ�*/
int i,j;/*���ñ���*/
void Menu();/*��ʼ������*/
void Selectitem();/*����˵�*/
void DrawSelectitem();/*��ʾ���˵�*/
void BlackText(int x,int y,char *z);/*ѡ�в˵�*/
void RedText(int x,int y,char *z);/*�����˵�*/
void Run();/*�����������*/
void DrawMl(int n);/*��ʾ�����˵�*/
void MoveMl(int n,int x);/*�˵�ѡ��Ŀ���*/
void Enter(int m,int n);/*�˵�ѡ��ľ��幦��*/
void BookAdd();/*���ͼ��*/
void BookConsult();/*ͼ���ѯ*/
void BookDel();/*ɾ��ͼ������*/
void BookBorrow();/*����*/
void BookReturn(); /*����*/
void MemberAdd(); /*���ӻ�Ա*/
void MemberConsult();/*��ѯ��Ա*/
void MemberDel(); /*ɾ����Ա*/
void MemberBook(); /*��ѯ��Ա������Ϣ*/
void Help(); /*����*/
void Ver(); /*�汾��Ϣ*/
void ClrScr();/*�Զ�����������*/
void DrawFrame(int left,int up,int right,int down,int textcolor,int backgroundcolor);/*���߿�*/
/***������****/
void main(void)
{
   Menu();/*��ʼ������*/
   Run();/*�����������*/
}
/*��ʼ������*/
void Menu()
{
   system("cls"); /*����ϵͳ����������*/
   textbackground(BLUE);/*����������Ϊ��ɫ*/
   window(1,1,25,80);
   clrscr();
   textx=3;/*����ʼ��λ��*/
   texty=2;
   gotoxy(1,2);
   printf("%c",218);/*�����Ͻ�*/
   for(i=0;i<78;i++)
   printf("%c",196); /*��ˮƽֱ��*/
   printf("%c",191);/*�����Ͻ�*/
   for(i=3;i<=23;i++)
   {
      gotoxy(1,i);
      printf("%c",179); /*����ֱ��*/
      gotoxy(80,i);
      printf("%c",179);
   }
   printf("%c",192); /*�����½�*/
   for(i=0;i<78;i++)
      printf("%c",196);
   printf("%c",217); /*�����½�*/
   gotoxy(1,1);
   textcolor(7); /*���û�ɫ*/
   for(i=0;i<80;i++)
      cprintf("%c",219);/*�÷���ʵ�ֻ����˵��Ļ�ɫ������*/
   Selectitem();  /*����ѡ���*/
   DrawSelectitem(); /*��ѡ��*/
   gettext(2,3,78,23,c); /*���浱ǰ�ı�����*/
}
/*����˵�*/
void Selectitem()
{
   strcpy(ml[0].name,"Book");/*����ľ���ѡ��ո���Ϊ�˸��˵���ɫ������ͬ*/
   strcpy(ml[0].str[0],"Add                ");/*���ͼ��*/
   strcpy(ml[0].str[1],"Consult            ");/*��ѯͼ��*/
   strcpy(ml[0].str[2],"Del                ");/*ɾ��ͼ������*/
   strcpy(ml[0].str[3],"Borrow             ");/*����*/
   strcpy(ml[0].str[4],"Return             ");/*����*/
   strcpy(ml[0].str[5],"Exit               ");/*�˳�ϵͳ*/
   ml[0].n=6; /*����˵�������*/
   strcpy(ml[1].name,"Member");
   strcpy(ml[1].str[0],"Add                ");/*����»�Ա*/
   strcpy(ml[1].str[1],"Consult            ");/*��ѯ��Ա����*/
   strcpy(ml[1].str[2],"Del                ");/*ɾ����Ա����*/
   strcpy(ml[1].str[3],"Member&book        ");
   ml[1].n=4;
   strcpy(ml[2].name,"Help");/*ϵͳ����*/
   strcpy(ml[2].str[0],"This System        ");
   strcpy(ml[2].str[1],"Ver           ");
   ml[2].n=2;
}
/*��ʾ������*/
void DrawSelectitem()
{
   for(i=0;i<3;i++)
   RedText(i,1,ml[i].name); /*��ʾ���˵�����������ĸΪ��ɫ*/
}
/*������ʾ�˵�*/
void RedText(int x,int y,char *z)
{
   textbackground(7); /*���ñ�����ɫΪǳ��ɫ*/
   gotoxy(3+x*20,y);
   for(j=0;z[j];j++)
   {
      if(j==0)
	 textcolor(RED);/*��һ����ĸ��ʾ��ɫ*/
      else
	 textcolor(BLACK); /*���ú�ɫ*/
      cprintf("%c",z[j]); /*����˵���*/
   }
}
/*��ʾѡ�в˵�*/
void BlackText(int x,int y,char *z)
{
   textbackground(0); /*���ñ�����ɫΪ��ɫ*/
   textcolor(15); /*�����ı���ɫΪ��ɫ*/
   gotoxy(3+20*x,y);/*��λ����*/
   cputs(z); /*����˵����ַ���*/
}
/*������������*/
void Run()
{
   while(1)
   {
      gotoxy(texty,textx);
      key=bioskey(0);/*���հ���*/
      switch(key)
      {
	 case ALT_B:
	 case ESC: DrawMl(0);break; /*��ʾ�����˵�1*/
	 case ALT_M: DrawMl(1);break;/*��ʾ�����˵�2*/
	 case ALT_H: DrawMl(2);/*��ʾ�����˵�3*/
	 case UP:  /*�Ϲ����Ĳ�������*/
	    {
	       if(textx==3)
		  textx=23;
	       textx--;
	       gotoxy(texty,textx);
	    }break;
	 case DOWN:  /*�¹����Ĳ�������*/
	    {
	       if(textx==23)
		  textx=3;
	       textx++;
	       gotoxy(texty,textx);
	    }break;
	 case LEFT: /*������Ĳ�������*/
	    {
	       if(texty==2)
		  texty=79;
	       texty--;
	       gotoxy(texty,textx);
	    }break;
	 case Backspace: /*������������*/
	    {
	       if(texty==2&&textx==3)
		  continue;
	       else
	       {
		  if(texty!=2)
		     texty--; /*��������ϸ������,�Ȳ�ȥ����,Ȼ���껹Ҫ������һ��*/
		  else
		     if(texty==2)
		     {
			texty=78;
			textx--;
		     }
		     gotoxy(texty,textx);
		     printf(" ");
		     gotoxy(texty,textx);
		  }
	       }break;/*end case 0xe08*/
	 case RIGHT: /*�ҹ����Ĳ�������*/
	    {
	       if(texty==79)
		  texty=2;
	       texty++;
	       gotoxy(texty,textx);
	    }break;
	 case SPACE: /*�ո���Ĳ���*/
	    {
	       if(texty==79)
		  continue;
	       else
	       {
		  gotoxy(texty,textx); /*�ո��ϸ�ڲ���*/
		  printf(" ");
		  texty++;
		  gotoxy(texty,textx);
	       }
	    }break;
	 case ENTER: /*�س��Ŀ��Ʋ���*/
	    {
	       if(textx==23)
		  continue;
	       textx++;
	       texty=2;
	       gotoxy(texty,textx);
	    }break;
	 default : /*�ǿ��Ƽ��Ľ��*/
	    {
	       if(texty==79&&textx==23)/*�������Ͳ������*/
		  continue;
	       else
		  if(texty==79&&textx!=23) /*���е����*/
		  {
		     textx++;
		     texty=2;
		  }
	       gotoxy(texty,textx);/*������*/
	       printf("%c",key);
	       if(texty==79) /*���texty==79�Ͳ�ִ��*/
		  continue;
	       else /*���û����β�ͼ���ִ��,ʹ�����ǰ�ƶ�һλ*/
		  texty++;
	 }
      }
   }/*��ѭ���Ĵ�����*/
}
/*���߿���*/
void DrawFrame(int l,int u,int r,int d,int tcolor,int bcolor)
{
   textbackground(bcolor); /*������ɫ*/
   textcolor(bcolor); /*�ı���ɫ*/
   for(i=l;i<=r;i++) /*�����������*/
   {
      for(j=u;j<=d;j++)
      {
	 gotoxy(i,j);
	 printf("%c",219); /*��������ַ�*/
      }
   }
   textcolor(tcolor);/*�߿���ɫ*/
   for(i=u+1;i<d;i++) /*�ڱ�������������߿���*/
   {
      gotoxy(l,i);
      cprintf("%c",179); /*��ֱ��*/
      gotoxy(r,i);
      cprintf("%c",179);
   }
   for(i=l+1;i<r;i++)
   {
      gotoxy(i,u);
      cprintf("%c",196); /*ˮƽ��*/
      gotoxy(i,d);
      cprintf("%c",196);
   }
   gotoxy(l,u);
   cprintf("%c",218);/*���Ͻ�*/
   gotoxy(r,u);
   cprintf("%c",191);/*���Ͻ�*/
   gotoxy(l,d);
   cprintf("%c",192);/*���½�*/
   gotoxy(r,d);
   cprintf("%c",217); /*���½�*/
/* gettext(l+1,u+1,r-1,d-1,save1);*//*����߿�������*/
}
/*��ʾ��������ѡ����Ŀ*/
void DrawMl(int n)
{
   gettext(1,1,80,25,save);/*���汻�ڸǵĵط�*/
   BlackText(n,1,ml[n].name);/*��ѡ��ʾ���˵�*/
   DrawFrame(3+20*n-1,2,3+20*n+19,3+ml[n].n,0,7);/*�����˵��ı߿�*/
   for(i=3;i<3+ml[n].n;i++)/*�����ѡ�˵���ѡ��*/
   {
      if(i==3)
	 BlackText(n,i,ml[n].str[i-3]);/*Ĭ��ѡ�е�һ��*/
      else
	 RedText(n,i,ml[n].str[i-3]);/*����������ַ���ɫ��ʾ*/
   }
   gotoxy(79,1);
   MoveMl(n,3);/*�˵�ѡ��Ŀ���*/
}
/*�˵�ѡ��Ŀ��ƣ�n����ˮƽ�x����������ѡ��*/
void MoveMl(int n,int x)
{
   int flag=1;
   while(flag)
   {
      gotoxy(79,1);
      key=bioskey(0);/*���հ���*/
      gotoxy(79,1);
      switch(key)
      {
	 case ESC:/*�˳�ѭ��*/
		  puttext(1,1,80,25,save);/*�ָ��򿪲˵�ǰ������*/
		  flag=0;
		  break;
	 case LEFT:/*�Ƶ���ߵ�ѡ��*/
		  puttext(1,1,80,25,save);/*�ָ��򿪲˵�ǰ������*/
		  if(n==0)/*�����ƶ�Խ��Ļ��Ƶ����һ��ѡ��*/
		     DrawMl(2);
		  else
		     DrawMl(n-1);
		  flag=0;
		  break;
	 case RIGHT:/*�ƶ��ұߵ�ѡ��*/
		  puttext(1,1,80,25,save);/*�ָ��򿪲˵�ǰ������*/
		  if(n==2)/*�����ƶ�Խ��Ļ��Ƶ���һ��ѡ��*/
		     DrawMl(0);
		  else
		     DrawMl(n+1);
		  flag=0;
		  break;
	 case UP:/*����ѡ�������ƶ�*/
		  RedText(n,x,ml[n].str[x-3]);/*�����ɫ����*/
		  if(x==3)/*�Ƶ��������ٰ��ϼ������Ƶ�������*/
		     x=3+ml[n].n-1;
		  else
		     x--;/*�ƶ����µ�Ҫ��ʾ������*/
		  BlackText(n,x,ml[n].str[x-3]);/*�����ɫ����*/
		  flag=1;
		  break;
	 case DOWN:/*����ѡ�������ƶ�*/
		  RedText(n,x,ml[n].str[x-3]);
		  if(x==(3+ml[n].n-1))/*�ƶ���������ٰ��¼����Ƶ�������*/
		     x=3;
		  else
		     x++;/*�ƶ����µ�Ҫ��ʾ������*/
		  BlackText(n,x,ml[n].str[x-3]);
		  flag=1;
		  break;
	 case ENTER:
		  puttext(1,1,80,25,save);/*�ָ��򿪲˵�ǰ������*/
		  Enter(n,x-3);/*�˵�ѡ��ľ��幦��*/
		  flag=0;
		  break;
	 }
      gotoxy(79,1);
   }
}
 /*�˵�ѡ��ľ��幦��*/
void Enter(int m,int n)
{
   switch(m)
   {
      case 0:switch(n) /*ѡ����ͼ��˵�ѡ��*/
	     {
		case 0:BookAdd();break;/*���ͼ��*/
		case 1:BookConsult();break;/*ͼ���ѯ*/
		case 2:BookDel();break;/*ɾ��һ��ͼ������*/
		case 3:BookBorrow();break;/*����*/
		case 4:BookReturn();break;/*����*/
		case 5:exit(0);break;
	     } break;/*�˳�ϵͳ*/
      case 1: switch(n) /*ѡ���˻�Ա�˵�ѡ��*/
	      {
		case 0: MemberAdd();break;/*��ӻ�Ա*/
		case 1:  MemberConsult();break;/*��Ա��ѯ*/
		case 2:MemberDel();break;/*ɾ��һ����Ա����*/
		case 3:MemberBook();/*��ѯĳ����Ա����ͼ�����*/
	      }break;
      case 2:switch(n) /*ѡ���˰����˵�ѡ��*/
	     {
		case 0:Help();break;
		case 1:Ver();
	     }
   }/*������switch*/
}
