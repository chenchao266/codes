/*12.3.2 Դ����*/
/******ͷ�ļ���.h��***********/
#include "stdio.h"    /*I/O����*/
# include "bios.h"  /*ROM���������������*/
#include "dos.h"    /*dos�ӿں���*/
#include "conio.h"   /*��Ļ��������*/
#include "stdlib.h"   /*����˵��*/
#include "string.h"  /*�ַ�������*/
#include "mem.h"  /*�ڴ��������*/
#include "ctype.h" /*�ַ���������*/
#include "alloc.h"  /*��̬��ַ���亯��*/
/****��������*******/
typedef struct z1    /*�������ݽṹ*/
{
   char no[11];     /*���*/
   char name[15];  /*����*/
   float jbgz;     /*��������*/
   float koukuan; /*�ۿ�*/
   float yfgz;  /*Ӧ������*/
   float shuijin;  /*˰��*/
   float sfgz;   /*ʵ������*/
   struct z1 *prior;/*ǰ��ָ��*/
   struct z1 *next;/*���ָ��*/
}SALARY;     /*�ṹ��������*/
struct z1 *First;   /*˫����ͷָ��*/
struct z1 *Last;   /*˫����βָ��*/
/******����ԭ��*********/
void init();    /*��ʼ��*/
void create();  /*��������*/
void calc();   /*����Ӧ������*/
void delete(); /*ɾ��*/
void search(); /*����*/
void save();  /*�����ļ�*/
void load();  /*��ȡ�ļ�*/
void computer(); /*�����������Ʊ������*/
void insert();  /*����*/
void append(); /*׷��*/
void copy(); /*�����ļ�*/
void sort();  /*����*/
void index();  /*����*/
void total();  /*����ϼ�*/
void list(); /*��ʾ��������*/
void print(SALARY *p);  /*���������¼*/
void display();  /*����������ʾ*/
float fax(float x);  /*����˰��*/
int menu_select();  /*���˵�*/
/*******��������ʼ**********/
main()
{
   int i;
   clrscr();
   for(;;)
   {
      switch(menu_select())   /*���ò˵���������һ������ֵ*/
      {
	 case 0:init();break;    /*��ʼ��*/
	 case 1:create();break; /*�������ݴ���˫����*/
	 case 2:list();break;  /*��ʾ��������*/
	 case 3:display();break;  /*������ʾ*/
	 case 4:calc();break;  /*����ʵ������*/
	 case 5:search();break;  /*����ְ����Ϣ����*/
	 case 6:delete();break;  /*ɾ����¼*/
	 case 7:insert();break;  /*�����¼*/
	 case 8:append();break;  /*׷�Ӽ�¼*/
	 case 9:save();break;  /*�����ļ�*/
	 case 10:load(); break;  /*��ȡ�ļ�*/
	 case 11:copy();break;  /*�����ļ�*/
	 case 12:sort();break;  /*��ʵ��������������*/
	 case 13:computer();break; /*��������Ʊ����*/
	 case 14:index();break;  /*��ְ��������*/
	 case 15:total();break;  /*��������ϼ�*/
	 case 16:exit(0);    /*�˳�*/
      }
   }
}
/*�˵���������������ֵΪ���ͣ�������ѡ�Ĳ˵���*/
menu_select()
{
   char *f[]= {     /*����˵��ַ�������*/
   "**************MENU*************", /*�˵��ı�����*/
   " 0. init list", /*��ʼ��˫����*/
   " 1. Enter list", /*�������ݣ�����˫����*/
   " 2. List all \n",  /*��ʾ���м�¼*/
   " 3. Display record by step", /*��ʾ������¼*/
   " 4. Calc the salary", /*����ʵ������*/
   " 5. Search record on name", /*���Ҽ�¼*/
   " 6. Delete a record", /*ɾ����¼*/
   " 7. Insert record to list", /*�����¼*/
   " 8. Append  record to file", /*׷�Ӽ�¼*/
   " 9. Save data to the file",  /*�����ļ�*/
   " 10. Load data from the file",/*��ȡ�ļ�*/
   " 11. Copy the file to new file", /*�����ļ�*/
   " 12. Sort on sfgz",  /*����*/
   " 13. Computer ticket number", /*��������Ʊ����*/
   " 14. Index on number",  /*����*/
   " 15. Total on number",  /*����ϼ�*/
   " 16. Quit" };  /*�˳�*/
   char s[80];
   int  i;
   int key=0; /*��¼��ѹ��ֵ*/
   int c=0;
   gotoxy(1,25); /*�ƶ����*/
   printf("press any key enter menu......\n");/*ѹ������������˵�*/
   getch();
   clrscr();  /*����*/
   textcolor(YELLOW); /*�����ı���ɫΪ��ɫ*/
   textbackground(BLUE);  /*���ñ�����ɫΪ��ɫ*/
   gotoxy(10,2);
   putch(0xda); /*������ϽǱ߿�*/
   for(i=1;i<44;i++)
   putch(0xc4); /*����ϱ߿�ˮƽ��*/
   putch(0xbf);  /*������ϽǱ߿�  ��*/
   for(i=3;i<22;i++)/*����������ߵĴ�ֱ��*/
   {
      gotoxy(10,i);putch(0xb3);
      gotoxy(54,i);putch(0xb3);
   }
   gotoxy(10,22);putch(0xc0); /*������ϽǱ߿�*/
   for(i=1;i<44;i++)
      putch(0xc4);  /*����±߿�ˮƽ��*/
   putch(0xd9);  /*������½Ǳ߿�*/
   window(11,3,53,21); /* ������ʾ�˵��Ĵ��ڣ���С���ݲ˵��������*/
   clrscr();   /*����*/
   for(i=0;i<18;i++)
   {
      gotoxy(10,i+1);
      cprintf("%s",f[i]); /*����˵�������*/
   }
   i=1;
   gotoxy(10,2);  /*����Ĭ��ѡ���ڵ�һ��*/
   textbackground(LIGHTGREEN);/*���ñ�����ɫΪǳ��*/
   cprintf("%s",f[1]);  /*����˵����ʾѡ��*/
   gotoxy(10,2);  /*�ƶ���굽�˵��ĵ�һ��*/
   while(key!=13)   /*��ѹ�����ǻس���ʱ*/
   {
      while(bioskey(1)==0);  /*��ѯ�Ƿ�ѹ����һ����*/
      key=bioskey(0);   /*������һ���ڼ���ѹ�µļ�*/
      key=key&0xff?key&0xff:key>>8; /*����ѹ�ļ������ж�*/
      gotoxy(10,i+1);
      textbackground(BLUE);/*���ñ�����ɫΪ��ɫ*/
      cprintf("%s",f[i]);  /*����˵���*/
      if(key==72) i=i==1?17:i-1; /*��ѹ���Ϲ�������i��1�����ѵ���һ�������ƣ������һ��*/
      if(key==80)i=i==17?1:i+1; /*��ѹ���¹�������i��1�����ѵ����һ�������ƣ��򵽵�һ��*/
      gotoxy(10,i+1); /*����ƶ�i����һ��*/
      textbackground(LIGHTGREEN); /*��������ɫ��Ϊǳ��*/
      cprintf("%s",f[i]);  /*����˵���*/
      c=i-1;  /*�������˵�ѡ���������ֵ*/
   }
   textbackground(BLACK);  /*���ñ�����ɫΪ��ɫ*/
   window(1,1,80,25);  /*�ָ�ԭ���ڴ�С*/
   return c;  /*���ش����˵�ѡ�������ֵ*/
}
/*��ʼ������*/
void init()
{
   First=NULL;
   Last=NULL;
}
/*�������ݣ�����˫����*/
void create()
{
   int x; /*��¼������*/
   int i; /*��¼�����¼��*/
   int flag=0; /*���������*/
   float temp;   /*������ʱ����*/
   SALARY *info,*p; /*������ʱ����*/
   if(First!=NULL)
   init();/*���ͷָ��Ϊ�գ����ó�ʼ������*/
   p=First; /*��ͷָ�뿪ʼ*/
   for(;;)
   {
      if(flag==1)
	 break; /*���flag=1����������*/
      i=0;
      x=0;   /*ȷ���ƶ���������*/
      clrscr(); /*����*/
      gotoxy(1,3);
      printf("*************gongziguanli*************");/*�������*/
      gotoxy(1,4);
      printf("                      --Enter @ end--");/*��ʾ����@����*/
      gotoxy(1,5);
      printf("|------------------------------------|");/*����������ʼ��*/
      gotoxy(1,6);
      printf("|    no    |      name      |  jbgz  |");/*����ֶα��⣬ע��ո���*/
      for(;;)
      {
	 gotoxy(1,7+x);
	 printf("|----------|----------------|--------|");/*��������ˮƽ��*/
	 info=(SALARY *)malloc(sizeof(SALARY));/*����һ����¼�ռ�*/
	 if(!info)
	 {
	    printf("\nout of memory");/*��û�еõ��ռ䣬����ڴ������Ϣ*/
	    exit(0);/*�˳�����*/
	 }
	 info->next=NULL;/*�½��ĺ��Ϊ��*/
	 info->prior=NULL;/*�½���ǰ��Ϊ��*/
	 gotoxy(1,8+x);printf("|"); /*������ݼ�ķָ���*/
	 gotoxy(12,8+x);printf("|");
	 gotoxy(29,8+x);printf("|");
	 gotoxy(38,8+x);printf("|");
	 gotoxy(2,8+x);/*��굽������λ��*/
	 inputs(info->no,10);/*�����ţ�����֤���Ȳ�����10*/
	 if(info->no[0]=='@')
	 {
	    flag=1;
	    break;
	 } /*������ַ�Ϊ@��������*/
	 gotoxy(13,8+x);  /*��굽��������λ��*/
	 inputs(info->name,14); /*��������������֤���Ȳ�����14*/
	 gotoxy(30,8+x); /*��굽�����������λ��*/
	 scanf("%f",&temp); /*����������ʵ���ʱ����*/
	 info->jbgz=temp; /*�������ʸ�ֵ*/
	 info->koukuan=0; /*��ʼ�ۿ�Ϊ0�������㹤��ʱ����*/
	 info->sfgz=0; /*��ʼʵ������Ϊ0�������㹤��ʱ����*/
	 info->shuijin=0; /*��ʼ˰��Ϊ0�������㹤��ʱ����*/
	 info->yfgz=0; /*��ʼӦ������Ϊ0�������㹤��ʱ����*/
	 if(p==NULL)/*���pΪ�գ�˵��������ǵ�һ�����*/
	 {
	    First=Last=info;/*ͷָ���βָ��*/
	    First->prior=NULL; /*ͷָ���ǰ���ǿ�*/
	    Last->next=NULL; /*βָ��ĺ���ǿ�*/
	 }
	 else /*����Ľ�㲻�ǵ�һ��㣬�������ͷ���֮ǰ*/
	 {
	    info->next=p;/*�½��ĺ��ָ��ԭ����ͷ���*/
	    info->prior=p->prior;/*�½���ǰ��ָ��ԭ����ͷ����ǰ��*/
	    p->prior=info;/*ԭ������ǰ��ָ���½��*/
	 }
	 p=info;  /*�½���Ϊp��㣬��ʱ��ͷ���*/
	 First=info; /*�½���Ϊͷ���*/
	 x+=2;   /*��Ϊˮƽ�ߣ��������������*/
	 gotoxy(1,8+x);
	 i++;   /*�����¼����1*/
	 if(i%9==0)
	    break; /*����9����¼����һҳ����*/
      }
      gotoxy(1,8+x);
   }
   printf("|----------|----------------|--------|");/*�����βˮƽ��*/
}
/*�ַ����������֤����*/
inputs(char *s, int count)
{
   char p[255];
   do{
      scanf("%s",p);/*�����ַ���*/
      if(strlen(p)>count)printf("\n too long! \n");/*���г���У�飬����countֵ������*/
   }while(strlen(p)>count);
   strcpy(s,p); /*��������ַ����������ַ���s��*/
}
/*��ʾ����*/
void list()
{
   int i=0,n;  /*��¼�������*/
   SALARY *p; /*�����ƶ�ָ��*/
   clrscr();  /*����*/
   p=First;  /*��ͷָ�뿪ʼ*/
   while(i%10==0&&p!=NULL)  /*��ѭ�����ƻ�ҳ*/
   {
      i=0;       /*��¼һҳ���������*/
      clrscr();   /*����*/
      printf("\n\n\n"); /*��������*/
      printf("*************************************SALARY***********************************\n");   /*��������*/
      printf("|nO        |      name     |   jbgz  | koukuan |   yfgz  | shuijin |   sfgz  |\n");  /*����ֶα���*/
      printf("|----------|---------------|---------|---------|---------|---------|---------|\n");  /*���ˮƽ��*/
      while(p!=NULL)  /*��p��Ϊ��ʱ*/
      {
	 printf("|%-10s|%-15s|%9.2f|%9.2f|%9.2f|%9.2f|%9.2f|\n",
	 p->no,p->name,p->jbgz,p->koukuan,p->yfgz,p->shuijin,p->sfgz);
	 p=p->next;  /*ָ�����*/
	 i++;   /*���������1 */
	 if(i%10==0)
	    break;  /*�����10�У�������ѭ��*/
      }
      printf("****************************************end***********************************\n");  /*�����ҳ�Ľ�����*/
      printf("Press any key congtinue...\n");
      getch(); /*���һҳ��ͣһ�£�ѹ���������*/
   }  /*���ص���ѭ����׼����һҳ�����*/
}
/*������ʾ��¼*/
void display()
{
   int ch;   /*�˵�ѡ���ַ�*/
   SALARY *p;  /*������ʱָ��*/
   clrscr();  /*����*/
   p=First;  /*��ͷָ�뿪ʼ*/
   if(p==NULL)  /*ָ��Ϊ�գ�������Ϊ�գ�����������*/
   {
      printf("record is empty!\n");
      return;
   }
   else
   {
      print(p);   /*�������������ʾͷ�����Ϣ*/
      for(;;)  /*ѭ����ʼ*/
      {
	 printf("\n\n0. Quit 1. Prior      2.  Next      3.  First        4.Last \n\n");
/*�˵���Quit�˳� Priorǰ�� Next���  First��һ�� Last���һ��*/
	 printf("\nplease select 0~4:"); /*��ʾ����˵�ѡ��0~4*/
	 scanf("%d",&ch); /*����˵�ѡ��0~4*/
	 if(ch==0) break;  /*���ѡ��Ϊ0���˳�*/
	 switch(ch)   /*����������ѡ���ַ��ж�*/
	 {
	    case 1:p=p->prior;if(p==NULL)p=First;break;  /*ָ��ǰ����û��ǰ��ָ���һ��*/
	    case 2:p=p->next;if(p==NULL) p=Last;break; /*ָ���̣�û�к��ָ�����һ��*/
	    case 3:p=First;break;  /*ָ���һ�����*/
	    case 4:p=Last;break; /*ָ�����һ�����*/
	 }
	 print(p);  /*�������������ʾָ����ָ�����Ϣ*/
      }
   }
}
/*���ָ����ָ�����Ϣ*/
void print(SALARY *p)
{
   if(p)   /*���ָ�벻Ϊ�գ�����ʾ��Ϣ*/
   {
      clrscr();  /*����*/
      printf("\n\n\n");  /*����*/
      printf("************************************SALARY************************************\n");  /*���������Ϣ*/
      printf("|no        |      name     |   jbgz  | koukuan|   yfgz  | shuijin |   sfgz   |\n"); /*����ֶα���*/
      printf("|----------|---------------|---------|---------|---------|---------|---------|\n"); /*����ָ���*/
      printf("|%-10s|%-15s|%9.2f|%9.2f|%9.2f|%9.2f|%9.2f|\n", p->no,p->name,
      p->jbgz,p->koukuan,p->yfgz,p->shuijin,p->sfgz);/*��������Ϣ*/
      printf("***************************************end***************************** *******\n");  /*����������*/
   }
}
/*����ʵ������*/
void calc()
{
   int x; /*������*/
   int i=0; /*��¼��*/
   float temp; /*��ʱ�����ۿ�*/
   SALARY *p;  /*�ƶ�ָ��*/
   clrscr();   /*����*/
   p=First;  /*��ͷָ�뿪ʼ*/
   while(p!=NULL)  /*��p��Ϊ��ʱ��Ҳ�����м�¼ʱ����ѭ�����ƻ�ҳ*/
   {
      x=0;
      clrscr();
      gotoxy(1,3);
      printf("***************************gongziguanli************************************");   /*�������*/
      gotoxy(1,5);
      printf("|--------------------------------------------------------------------------|");
      gotoxy(1,6);/*��������ֶα���*/
      printf("|    no    |      name      |  jbgz  |  koukuan |  yfgz  |shuijin |  sfgz  |");
      while(p!=NULL) /*��p��Ϊ��ʱ��Ҳ�����м�¼ʱ����ѭ�����ƴ���9��*/
      {
	 gotoxy(1,7+x);
	 printf("|----------|----------------|--------|----------|--------|--------|--------|");
	 gotoxy(1,8+x);printf("|");
	 gotoxy(12,8+x);printf("|");
	 gotoxy(29,8+x);printf("|");
	 gotoxy(38,8+x);printf("|");
	 gotoxy(49,8+x);printf("|");
	 gotoxy(58,8+x);printf("|");
	 gotoxy(67,8+x);printf("|");
	 gotoxy(76,8+x);printf("|");
	 gotoxy(2,8+x);
	 printf("%-10s",p->no); /*������*/
	 gotoxy(13,8+x);
	 printf("%-15s",p->name); /*�������*/
	 gotoxy(30,8+x);
	 printf("%.2f",p->jbgz);  /*�����������*/
	 gotoxy(39,8+x);
	 scanf("%f",&temp);  /*����ۿ�*/
	 p->koukuan=temp; /*�ۿ��ֶθ�ֵ*/
	 p->yfgz=p->jbgz-p->koukuan; /*����Ӧ������=��������-�ۿ�*/
	 gotoxy(50,8+x);
	 printf("%.2f",p->yfgz);  /*���Ӧ������*/
	 gotoxy(59,8+x);
	 p->shuijin=(p->yfgz-1000)*fax(p->yfgz); /*����˰�ʼ���˰��*/
	 printf("%.2f",p->shuijin);  /*���˰��*/
	 gotoxy(68,8+x);
	 p->sfgz=p->yfgz-p->shuijin;  /*����ʵ������*/
	 printf("%-8.2f",p->sfgz);  /*���ʵ������*/
	 x+=2;   /*��������������*/
	 i++;   /*��¼����1*/
	 if(i%9==0)break;  /*��¼������9����������ѭ��*/
	 p=p->next;   /*ָ����ƣ�������һ��*/
      }
      gotoxy(1,7+x);
      printf("|----------|----------------|--------|----------|--------|--------|--------|\n"); /*���������*/
      printf("Press any key congtinue...\n");
      getch();
   }
}
/*����Ӧ�����ʣ�����˰��*/
float fax(float x)
{
   float f1;
   if(x<1000)
      return 0;   /*С��1000������0*/
   switch((int)(x/1000))
   {
      case 1:f1=0.05;break; /*1000~1999������0.05*/
      case 2:f1=0.1;break; /*2000~2999������0.1*/
      case 3:f1=0.15;break; /*3000~3999������0.15*/
      case 4:f1=0.2;break; /*4000~4999������0.2*/
      default:f1=0.3;break; /*5000���ϣ�����0.3*/
   }
   return f1;
}
/*����Ʊ������*/
void computer()
{
   SALARY *p;
   int i=0,a[7]={0},t100,t50,t20,t10,t5,t2,t1;/*�����������ʱ��������Ʊ������*/
   float t;   /*���ɱ��������浱ǰʣ�๤��*/
   p=First;  /*�ƶ�ָ���ͷָ�뿪ʼ*/
   while(p!=NULL)  /*��p��Ϊ��ʱ*/
   {
      t100=(int)p->sfgz/100;  /*����100Ԫ����*/
      a[0]=a[0]+t100;   /*�ۼӱ���*/
      t=p->sfgz-t100*100; /*ʣ�๤��*/
      t50=(int)(t/50); /*����50Ԫ����*/
      a[1]=a[1]+t50; /*�ۼӱ���*/
      t=t-t50*50; /*ʣ�๤��*/
      t20=(int)(t/20);   /*����20Ԫ����*/
      a[2]=a[2]+t20; /*�ۼӱ���*/
      t=t-t20*20; /*ʣ�๤��*/
      t10=(int)(t/10); /*����10Ԫ����*/
      a[3]=a[3]+t10; /*�ۼӱ���*/
      t=t-t10*10; /*ʣ�๤��*/
      t5=(int)(t/5); /*����5Ԫ����*/
      a[4]=a[4]+t5; /*�ۼӱ���*/
      t=t-t5*5; /*ʣ�๤��*/
      t2=(int)(t/2); /*����2Ԫ����*/
      a[5]=a[5]+t2; /*�ۼӱ���*/
      t=t-t2*2; /*ʣ�๤��*/
      t1=(int)(t); /*����1Ԫ����*/
      a[6]=a[6]+t1; /*�ۼӱ���*/
      p=p->next;  /*ָ����ƣ�������һ����¼*/
   }
   clrscr();  /*����*/
   printf("\n\n\n***********ticket number***************\n"); /*�������*/
   printf("--100-----50------20-----10-----5------2------1------\n");
     /*�����������*/
   for(i=0;i<7;i++)
      printf("%5d  ",a[i]);/*������������*/
   printf("\n");/*����*/
}
/*����Ų��Ҽ�¼*/
SALARY *find(char *no)
{
   SALARY *p; /*�����ƶ�ָ��*/
   p=First;  /*��ͷָ�뿪ʼ*/
   while(p) /*��p��Ϊ��ʱ*/
   {
      if(!strcmp(no,p->no))
	 return p; /*�Ƚ���ȣ����ҳɹ�������ָ��*/
      p=p->next; /*�����ָ�����*/
   }
   printf("not found\n");  /*û���ҵ�*/
   return NULL; /*����Ϊ��ָ��*/
}
/*ɾ�����*/
void delete()
{
   SALARY *p; /*������ʱ������pָ��Ҫɾ���Ľ��p*/
   char s[11]; /*Ҫɾ�����ı��*/
   clrscr();  /*����*/
   printf("please deleted no\n"); /*��ʾ������Ϣ*/
   scanf("%s",s);   /*����Ҫɾ����ְ�����*/
   if((p=find(s))!=NULL) /*���ò��Һ���������ҵ�������Ĵ���*/
   {
      if(First==p)    /*����ǵ�һ�����*/
      {
	 First=p->next;  /*��ͷָ��ָ�����̽��*/
	 if(First)    /*��ͷָ�벻Ϊ��*/
	    First->prior=NULL;  /*��ͷ����ǰ����Ϊ��*/
	 else
	    Last=NULL;   /*����ͷΪ�գ�βҲΪ��*/
      }
      else   /*ɾ���Ĳ��ǵ�һ�����*/
      {
	 p->prior->next=p->next;  /*p��ǰ���ĺ��ָ��pԭ���ĺ��*/
	 if(p!=Last)   /*���p�����������*/
	    p->next->prior=p->prior;  /*p��̵�ǰ��ָ��pԭ����ǰ��*/
	 else
	    Last=p->prior;/*��p�����һ����㣬�޸�βָ��Ϊp��ǰ��*/
      }
      free(p);  /*ɾ�����󣬲�Ҫ�����ͷſռ�*/
      printf("\n have deleted %s SALARY\n",s);
      printf("Don't forget save\n");
   }
}
/*���������ҽ��*/
void search()
{
   SALARY *p;  /*�ƶ�ָ��*/
   char s[15];  /*��������*/
   clrscr(); /*����*/
   printf("please enter name for search\n"); /*��ʾ����*/
   scanf("%s",s);  /*����Ҫ�����˵�����*/
   p=First;  /*�ƶ�ָ���ͷָ�뿪ʼ*/
   while(strcmp(p->name,s)&&p!=NULL) /*���Ƚ��ж�*/
      p=p->next;   /*û�ҵ���ָ����Ƽ�������*/
   if(p==NULL)  /*ָ��Ϊ�գ�˵��û���ҵ�*/
      printf("\nlist no %s SALARY\n",s); /*��ʾû�ҵ���Ϣ*/
   else
   {
      printf("\n\n"); /*����*/
      print(p);  /*���ҳɹ�����������������ָ��p��ָ��¼*/
   }
}
/*������*/
void insert()
{
   SALARY *p,*info; /* infoΪ�½�㣬pΪ�½��ĺ��*/
   char s[11];  /*������ҵ�����*/
   float temp; /*��ʱ����*/
   clrscr(); /*����*/
   printf("please enter location  before the no\n"); /*��ʾ�������ĸ����֮ǰ����*/
   scanf("%s",s);  /*����ָ�����ı��*/
   printf("\nplease new record\n");  /*��ʾ�����¼*/
   clrscr();/*����*/
   gotoxy(1,3); /*��λ��꣬��������ʽ�����½����Ϣ*/
   printf("**************gongziguanli************");
   gotoxy(1,5);
   printf("|------------------------------------|");
   gotoxy(1,6);
   printf("|    no    |      name      |  jbgz  | ");
   gotoxy(1,7);
   printf("|----------|----------------|--------|");
   info=(SALARY *)malloc(sizeof(SALARY)); /*����ռ�*/
   if(!info)
   {
      printf("\nout of memory"); /*��û�еõ��ռ䣬�ڴ����*/
      exit(0);  /*�˳�����*/
   }
   info->next=NULL;  /*�½��ĺ��Ϊ��*/
   info->prior=NULL; /*�½���ǰ��Ϊ��*/
   gotoxy(1,8);printf("|");
   gotoxy(12,8);printf("|");
   gotoxy(29,8);printf("|");
   gotoxy(38,8);printf("|");
   gotoxy(2,8);
   inputs(info->no,10);  /*�����½��ı�ţ���У��*/
   gotoxy(13,8);
   inputs(info->name,14); /*�����½�����������У��*/
   gotoxy(30,8);
   scanf("%f",&temp);   /*�����������*/
   info->jbgz=temp;  /*�����������ֶθ�ֵ*/
   info->koukuan=0; /*�ۿ��ʼΪ0*/
   info->yfgz=0;  /*Ӧ�����ʳ�ʼΪ0*/
   info->shuijin=0; /*˰���ʼΪ0*/
   info->sfgz=0; /*ʵ�����ʳ�ʼΪ0*/
   printf("|----------|----------------|--------|");
   p=First;  /*�ƶ�ָ���ͷָ�뿪ʼ*/
   while(strcmp(p->no,s)&&p!=NULL) /*���ҽ��ȷ������λ��*/
      p=p->next;  /*ָ����ƣ���������*/
   if(p==NULL)  /*��pΪ��*/
      if(p==First) /*��pΪͷָ�룬˵������Ϊ��*/
      {
	 First=info;  /*�½��Ϊͷָ��*/
	 First->prior=NULL; /*ͷ����ǰ��Ϊ��*/
	 Last=First;  /*Ψһ��㣬βָ�����ͷָ��*/
      }
      else  /*�½�����β��*/
      {
	 Last->next=info;
	 info->prior=Last;
	 Last=info;  /*βָ��ָ���½��*/
      }
   else
      if(p==First)  /*p��Ϊ�գ���pΪͷָ�룬�½����ڵ�һ�����λ��*/
      {
	 info->prior=NULL; /*�½���ǰ��Ϊ��*/
	 info->next=p; /*�½��ĺ��Ϊp*/
	 p->prior=info; /*p��ǰ�����½��*/
	 First=info; /*�޸�ͷָ��ָ���½��*/
      }
      else /*�½��������м�ĳһ��λ��p֮ǰ*/
      {
	 info->next=p; /*�½��ĺ����p*/
	 info->prior= p->prior; /*�½���ǰ����p��ǰ��*/
	 p->prior->next=info; /*p��ǰ���ĺ�����½��*/
	 p->prior=info; /*p��ǰ�����½��*/
      }
   printf("\n\n\n ----have inserted %s SALARY----\n",info->name);
   printf("\n---Don't forget save---\n"); /*���ѵ��ñ����ļ�����*/
}
/*�����ļ�*/
void save()
{
   FILE *fp;  /*����ָ���ļ���ָ��*/
   SALARY *p; /*�����ƶ�ָ��*/
   char outfile[10]; /*��������ļ���*/
   clrscr();/*����*/
   printf("Enter outfile name,for example c:\\f1\\te.txt:\n");/*��ʾ�����ļ�����ʽ��Ϣ*/
   scanf("%s",outfile);
   if((fp=fopen(outfile,"wb"))==NULL) /*Ϊ�����һ���������ļ�*/
   {
      printf("can not open file\n");
      return; /*����*/
   }
   printf("\nSaving file......\n");
   p=First; /*�ƶ�ָ���ͷָ�뿪ʼ*/
   while(p!=NULL) /*��p��Ϊ��*/
   {
      fwrite(p,sizeof(SALARY),1,fp); /*д��һ����¼*/
      p=p->next;  /*ָ����ƣ�������һ����¼*/
   }
   fclose(fp); /*�ر��ļ�*/
   printf("-----save success!!-----\n");/*��ʾ����ɹ�*/
}
/*���ļ�*/
void load()
{
   SALARY *p,*q=NULL; /*�����¼ָ�����*/
   FILE *fp; /* ����ָ���ļ���ָ��*/
   char infile[10]; /*�����ļ���*/
   clrscr();/*����*/
   printf("Enter infile name,for example c:\\f1\\te.txt:\n");/*�����ļ�����ʽ*/
   scanf("%s",infile); /*�����ļ���*/
   if((fp=fopen(infile,"rb"))==NULL) /*��һ���������ļ���Ϊ����ʽ*/
   {
      printf("can not open file\n"); /*�粻�ܴ򿪣����������*/
      return; /*����*/
   }
   while(First) /*������Ϊ��ʱ���������*/
   {
      p=First; /*��ͷָ�뿪ʼ*/
      First=First->next; /*ɾ��ͷ���*/
      free(p); /*�ͷſռ�*/
   }
   printf("\n -----Loading file!-----\n"); /*��ʾ���ڶ��ļ�*/
   First=(SALARY *)malloc(sizeof(SALARY)); /*Ϊͷָ������ռ�*/
   if(!First) /*���û�еõ��ռ� ��ʾ�ڴ����*/
   {
      printf("out of memory!\n");
      return;  /*����*/
   }
   p=First; /*��ͷָ�븳ֵ��p*/
   while(!feof(fp)) /*���ļ�����ʱ*/
   {
      if(1!=fread(p,sizeof(SALARY),1,fp))break; /*����¼����p��ָ�Ľ��*/
      p->next=(SALARY *)malloc(sizeof(SALARY));/*Ϊp�ĺ������ռ�*/
      if(!p->next)  /*���û�еõ��ռ���ʾ�ڴ����*/
      {
	 printf("out of memory!\n");
	 return; /*����*/
      }
      p->prior=q; /*�õ��ռ䣬������ָ���ϵp��ǰ��Ϊq*/
      q=p;  /*����p��Ϊ�µ�p����ǰ��*/
      p=p->next;  /*pָ�����*/
   }
   q->next=NULL; /*q�ĺ��Ϊ��*/
   Last=q;  /*βָ��Ϊq*/
   First->prior=NULL; /*ͷָ���ǰ��Ϊ��*/
   fclose(fp); /*�ر��ļ�*/
   printf("---You have success read data from file!!!---\n"); /*��ʾ�ɹ������¼*/
}
/*׷�Ӽ�¼*/
void append()
{
   FILE *fp;   /*����ָ���ļ���ָ��*/
   SALARY *info; /*ָ��׷����Ϣ��ָ��*/
   char infile[10]; /*�����ļ���*/
   float temp; /*��ʱ����*/
   clrscr(); /*����*/
   gotoxy(1,3);
   printf("**************gongziguanli************");
   gotoxy(1,5);
   printf("|------------------------------------|");
   gotoxy(1,6);
   printf("|    no    |      name      |  jbgz  | ");
   gotoxy(1,7);
   printf("|----------|----------------|--------|");
   info=(SALARY *)malloc(sizeof(SALARY)); /*����׷����Ϣ�Ŀռ�*/
   if(!info)
   {
      printf("\nout of memory"); /*û�пռ䣬�ڴ����*/
      exit(0);  /*��������*/
   }
   info->next=NULL;  /*����Ϣ�ĺ��Ϊ��*/
   info->prior=NULL; /*����Ϣ��ǰ��Ϊ��*/
   gotoxy(1,8);printf("|");
   gotoxy(12,8);printf("|");
   gotoxy(29,8);printf("|");
   gotoxy(38,8);printf("|");
   gotoxy(2,8);
   inputs(info->no,10); /*�����Ų�У��*/
   gotoxy(13,8);
   inputs(info->name,14); /*����������У��*/
   gotoxy(30,8);
   scanf("%f",&temp); /*�����������*/
   info->jbgz=temp;
   info->koukuan=0; /*�ۿ�Ϊ0*/
   info->yfgz=0; /*Ӧ������Ϊ0*/
   info->shuijin=0; /*˰��Ϊ0*/
   info->sfgz=0; /*ʵ������Ϊ0*/
   printf("|----------|----------------|--------|");
   printf("\n\n\nEnter infile name,for example c:\\f1\\te.txt:\n"); /*��ʾ�����ļ���*/
   scanf("%s",infile);
   if((fp=fopen(infile,"ab"))==NULL) /*���ļ�*/
   {
      printf("can not open file\n");
      return;  /*����*/
   }
   printf("\n -----Appending record!-----\n");
   if(1!=fwrite(info,sizeof(SALARY),1,fp)) /*���ļ���׷�Ӽ�¼*/
   {
      printf("-----file write error!-----\n"); /*д��������ֵ��Ϊ1��˵��д�����*/
      return;  /*����*/
   }
   printf("-----append  sucess!!----\n");/*��ʾ׷�ӳɹ�*/
   fclose(fp);/*�ر��ļ�*/
}
/*�ļ�����*/
void copy()
{
   char outfile[10],infile[10];  /*����Դ�ļ�����Ŀ���ļ���*/
   FILE *sfp,*tfp;  /*����ָ��Դ�ļ���Ŀ���ļ���ָ��*/
   SALARY *p=NULL;  /*������ʱָ�룬�ݴ�����ļ�¼*/
   clrscr();
   printf("Enter infile name,for example c:\\f1\\te.txt:\n");
   scanf("%s",infile); /*����Դ�ļ���*/
   if((sfp=fopen(infile,"rb"))==NULL) /*�����ƶ���ʽ��Դ�ļ�*/
   {
      printf("can not open input file\n"); /*��ʾ���ܴ��ļ���Ϣ*/
      return; /*����*/
   }
   printf("Enter outfile name,for example c:\\f1\\te.txt:\n");  /*��ʾ����Ŀ���ļ���*/
   scanf("%s",outfile); /*����Ŀ���ļ���*/
   if((tfp=fopen(outfile,"wb"))==NULL) /*������д��ʽ��Ŀ���ļ�*/
   {
      printf("can not open output file \n");
      return;
   }
   while(!feof(sfp)) /*���ļ�ֱ���ļ�β*/
   {
      if(1!=fread(p,sizeof(SALARY),1,sfp))
	 break;/*���*/
      fwrite(p,sizeof(SALARY),1,tfp); /*��д*/
   }
   fclose(sfp); /*�ر�Դ�ļ�*/
   fclose(tfp); /*�ر�Ŀ���ļ�*/
   printf("you have success copy  file!!!\n");/*��ʾ�ɹ�����*/
}
/*����*/
void sort()
{
   SALARY *p0,*p00,*p1,*p11,*p2; /*������ʱָ��*/
   int i=1;  /*ͳ�Ƶ�ǰ�ŵڼ������*/
   clrscr(); /*����*/
   printf("\n\n start sort....\n"); /*��ʼ����*/
   p1=p11=p2=First;
   while(p1!=NULL) /*��p1��Ϊ��ʱ*/
   {
      p0=p1;    /*���ȽϽ���p1��ʼ*/
      p2=p1;    /*p2�ӵ�ǰ������λ�ÿ�ʼ*/
      while(p2->next!=NULL) /*p2�ĺ�̲�Ϊ��ʱ*/
      {
	 if(p0->sfgz>p2->next->sfgz)  /*�ҵ�ǰ��С���*/
	 {
	    p00=p2;   /*�ҵ���ǰ��С����¼��ǰ��ָ��*/
	    p0=p2->next;  /*��¼��ǰ��Сֵ����ָ��p0*/
	 }
	 p2=p2->next;  /*ָ�����*/
      }
      Last=p0;  /*��ǰ��С���Ϊβ���*/
      if(p1!=p0)  /*�ж���С����Ƿ��ǵ�ǰ����λ�ã���������ָ���޸�*/
      {
	 p00->next=p0->next;  /*��p00�ĺ��ָ��p0�ĺ��*/
	 p0->next->prior=p00; /*p0�ĺ�̵�ǰ��Ϊp00*/
	 p0->next=p1;  /*p0�ĺ��ָ���¸�����λ��p1*/
	 p1->prior=p0; /*p1��ǰ��Ϊp0*/
	 if(i==1)
	 {
	    First=p0;   /*��ͷָ��*/
	    First->prior=NULL; /*ͷָ���ǰ��Ϊ��*/
	 }
	 else   /*���ǵ�һ�����*/
	 {
	    p11->next=p0;   /*p11�ĺ��ָ��p0*/
	    p0->prior=p11; /*p0��ǰ��ָ��p11*/
	 }
	 p11=p1=p0; /*��p0���ź�������һ����㣬p1����һ����Ҫ�����λ�ã����Խ�p0��ֵ��p1��p11*/
      }
      i++;  /*�ź���Ľ������1*/
      p1=p1->next; /*p1����ָ����һ��������λ��*/
   }
   printf("sort sucess!!!\n");  /*�������ɹ�*/
}
/*����*/
void index()
{
   SALARY *p,*q,*t,*h1; /*������ʱָ��*/
   clrscr();/*����*/
   printf("\n\n start index.....\n\n"); /*��ʾ��ʼ����*/
   h1=First->next; /*��ԭ����ͷָ����ָ����һ�������ͷָ��*/
   First->next=NULL; /*��һ�����Ϊ�±���ͷ���*/
   Last=First; /*βָ�����ͷָ��*/
   while(h1!=NULL) /*��ԭ����Ϊ��ʱ����������*/
   {
      t=h1; /*ȡԭ����ͷ���*/
      h1=h1->next; /*ԭ��ͷ���ָ�����*/
      p=First; /*�趨�ƶ�ָ��p����ͷָ�뿪ʼ*/
      q=First; /*�趨�ƶ�ָ��q��Ϊp��ǰ������ֵΪͷָ��*/
      while(strcmp(t->no,p->no)>0&&p!=NULL) /*����űȽ�*/
      {
	 q=p; /*�������ֵ��Ӧ����壬�����±�ָ�����*/
	 p=p->next;
      }
      if(p==q)  /*p==q��˵���������ֵС��Ӧ������λ*/
      {
	 t->next=p; /*�������ĺ��Ϊp*/
	 p->prior=t; /*p��ǰ�����ָ���������*/
	 First=t; /*��ͷ���Ϊ�������*/
	 First->prior=NULL; /*ͷָ���ǰ��Ϊ��*/
      }
      else /*�������Ӧ�������м�ĳ��λ��q��p֮�䣬��pΪ������β��*/
      {
	 t->next=p; /*t�ĺ����p*/
	 t->prior=q; /*t��ǰ����q*/
	 q->next=t; /*q�ĺ����t*/
	 if(p==NULL) /*��p�Ƿ�Ϊ��*/
	    Last=t;   /*��t��ֵΪβָ��*/
	 else
	    p->prior=t; /*����t��ֵ��p��ǰ��*/
      }
   }
   printf("index sucess!!!\n");/*��������ɹ�*/
}
/*����ϼ�*/
void total()
{
   struct t1  /*����ṹ�屣��ϼ�����*/
   {
      char u[7];  /*����*/
      float s;   /*�ϼ�ʵ������*/
   }unit[20];   /*����һ����λ������������20��*/
   SALARY *p,*q;  /*������ʱָ�����*/
   char sno[7],qno[7],*ptr; /*���沿�ź�*/
   float s1;  /*����ϼ�ֵ*/
   int i,j;  /*���Ʊ���*/
   p=First;  /*��ָ�뿪ʼ*/
   i=0;  /*ͳ�Ʋ�����*/
   clrscr();/*����*/
   while(p!=NULL) /*��p��Ϊ��ʱ������Ĵ���*/
   {
      memcpy(sno,p->no,6); /*�ӱ����ȡ�����ź�*/
      sno[6]='\0'; /*���ַ����������*/
      q=p->next; /*��ָ��ָ����Ƚϵļ�¼*/
      s1=p->sfgz;  /*��ǰ���ŵ�ʵ�����ʳ�ֵ��Ϊ�ò��źϼ�ֵ�ĳ�ֵ*/
      while(q!=NULL) /*��ѭ����ʼ*/
      {
	 memcpy(qno,q->no,6); /*�����ź�*/
	 qno[6]='\0'; /*���ַ����������*/
	 if(strcmp(qno,sno)==0) /*�Ƚϲ��ź�*/
	 {
	    s1+=q->sfgz; /*�ۼ�ʵ������*/
	    q=q->next; /*ָ��ָ����һ����¼*/
	 }
	 else
	    break; /*����һ�����ŵĽ���������ѭ��*/
      }
      strcpy(unit[i].u,sno); /*�����ź�д�벿�Žṹ��������*/
      unit[i].s=s1;    /*��ʵ������д�벿�Žṹ��������*/
      i++;       /*�������ۼ�*/
      if(q==NULL)  /*���qΪ�գ�����ѭ��*/
	 break;
      else
	 p=q;   /*���򣬽�q��ֵ��p*/
   }
   printf("\n\n**************Total table**************\n");/*����ϼƱ���*/
   printf("\n--no-------unit------------total---------\n"); /*�����š����š��ϼ��ֶα���*/
   for(j=0;j<i;j++)
      printf("   %d        %s             %10.2f\n",j,unit[j].u,unit[j].s); /*����ϼ�ֵ*/
   printf("----------------------------------------\n");
}
