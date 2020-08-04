/*11.3.2 Դ����*/
/***********xuesheng.c***********/
/******ͷ�ļ���.h��***********/
#include "stdio.h"    /*I/O����*/
#include "stdlib.h"   /*����˵��*/
#include "string.h"   /*�ַ�������*/
#include "conio.h"   /*��Ļ��������*/
#include "mem.h"   /*�ڴ��������*/
#include "ctype.h"   /*�ַ���������*/
#include "alloc.h"   /*��̬��ַ���亯��*/
#define N 3       /*���峣��*/
typedef struct z1   /*�������ݽṹ*/
{
   char no[11];
   char name[15];
   int score[N];
   float sum;
   float average;
   int order;
   struct z1 *next;
 }STUDENT;
/*�����Ǻ���ԭ��*/
STUDENT  *init();      /*��ʼ������*/
STUDENT *create();   /*��������*/
STUDENT *delete(STUDENT *h);   /*ɾ����¼*/
void print(STUDENT *h);   /* ��ʾ���м�¼*/
void search(STUDENT *h);    /*����*/
void save(STUDENT *h);     /*����*/
STUDENT *load();        /*�����¼*/
void computer(STUDENT *h);  /*�����ֺܷ;���*/
STUDENT *insert(STUDENT *h);   /*�����¼*/
void append();            /*׷�Ӽ�¼*/
void copy();            /*�����ļ�*/
STUDENT *sort(STUDENT *h);    /*����*/
STUDENT *index(STUDENT *h);  /*����*/
void total(STUDENT *h);       /*����ϼ�*/
int menu_select();            /*�˵�����*/
/******��������ʼ*******/
main()
{
   int i;
   STUDENT *head;     /*��������ͷָ��*/
   head=init();          /*��ʼ������*/
   clrscr();            /*����*/
   for(;;)             /*����ѭ��*/
   {
      switch(menu_select())     /*�������˵�����������ֵ������������������*/
      {                     /*ֵ��ͬ��ִ�еĺ�����ͬ��break ����ʡ��*/
	 case 0:head=init();break;   /*ִ�г�ʼ��*/
	 case 1:head=create();break; /*��������*/
	 case 2:head=delete(head);break; /*ɾ����¼*/
	 case 3:print(head);break;   /*��ʾȫ����¼*/
	 case 4:search(head);break;  /*���Ҽ�¼*/
	 case 5:save(head);break;  /*�����ļ�*/
	 case 6:head=load(); break;  /*���ļ�*/
	 case 7:computer(head);break;  /*�����ֺܷ;���*/
	 case 8:head=insert(head);  break; /*�����¼*/
	 case 9:copy();break;    /*�����ļ�*/
	 case 10:head=sort(head);break;  /*����*/
	 case 11:append();break;     /*׷�Ӽ�¼*/
	 case 12:head=index(head);break;  /*����*/
	 case 13:total(head);break;   /*����ϼ�*/
	 case 14:exit(0);       /*��˵�����ֵΪ14�������*/
      }
   }
}
/*�˵�����������ֵΪ����*/
menu_select()
{
   char *menu[]={"***************MENU***************",  /*����˵��ַ�������*/
   " 0. init list",    /*��ʼ��*/
   " 1. Enter list",   /*�����¼*/
   " 2. Delete a record from list",  /*�ӱ���ɾ����¼*/
   " 3. print list ",       /*��ʾ�����������м�¼*/
   " 4. Search record on name",   /*�����������Ҽ�¼*/
   " 5. Save the file",          /*���������м�¼���浽�ļ���*/
   " 6. Load the file",        /*���ļ��ж����¼*/
   " 7. compute the score",    /*��������ѧ�����ֺܷ;���*/
   " 8. insert record to list ",   /*�����¼������*/
   " 9. copy the file to new file",   /*�����ļ�*/
   " 10. sort to make new file",   /*����*/
   " 11. append  record to file",   /*׷�Ӽ�¼���ļ���*/
   " 12. index on nomber",       /*����*/
   " 13. total on nomber",      /*����ϼ�*/
   " 14. Quit"};              /*�˳�*/
   char s[3];       /*���ַ���ʽ����ѡ���*/
   int c,i;        /*�������α���*/
   gotoxy(1,25);        /*�ƶ����*/
   printf("press any key enter menu......\n");   /*ѹ��һ���������˵�*/
   getch();                       /*������һ��*/
   clrscr();                     /*����Ļ*/
   gotoxy(1,1);             /*�ƶ����*/
   textcolor(YELLOW);   /*�����ı���ʾ��ɫΪ��ɫ*/
   textbackground(BLUE);   /*���ñ�����ɫΪ��ɫ*/
   gotoxy(10,2);          /*�ƶ����*/
   putch(0xc9);        /*������ϽǱ߿�*/
   for(i=1;i<44;i++)
      putch(0xcd);    /*����ϱ߿�ˮƽ��*/
   putch(0xbb);            /*������ϽǱ߿�  ��*/
   for(i=3;i<20;i++)
   {
      gotoxy(10,i);putch(0xba);   /*�����ֱ��*/
      gotoxy(54,i);putch(0xba);
   }   /*����Ҵ�ֱ��*/
   gotoxy(10,20);putch(0xc8);  /*������ϽǱ߿�*/
   for(i=1;i<44;i++)
      putch(0xcd);      /*����±߿�ˮƽ��*/
   putch(0xbc);                 /*������½Ǳ߿�*/
   window(11,3,53,19);       /* ������ʾ�˵��Ĵ��ڣ���С���ݲ˵��������*/
   clrscr();              /*����*/
   for(i=0;i<16;i++)       /*������˵�����*/
   {
      gotoxy(10,i+1);
      cprintf("%s",menu[i]);
   }
   textbackground(BLACK);   /*���ñ�����ɫΪ��ɫ*/
   window(1,1,80,25);       /*�ָ�ԭ���ڴ�С*/
   gotoxy(10,21);       /*�ƶ����*/
   do{
      printf("\n	 Enter you choice(0~14):");    /*�ڲ˵���������ʾ��ʾ��Ϣ*/
      scanf("%s",s);           /*����ѡ����*/
      c=atoi(s);            /*��������ַ���ת��Ϊ������*/
   }while(c<0||c>14);    /*ѡ�����0~14֮������*/
   return c;              /*����ѡ�����������ݸ���������Ӧ�ĺ���*/
}
STUDENT *init()
{
   return NULL;
}

/*��������*/
STUDENT *create()
{
   int i; int s;
   STUDENT *h=NULL,*info;  /* STUDENTָ��ṹ���ָ��*/
   for(;;)
   {
      info=(STUDENT *)malloc(sizeof(STUDENT));  /*����ռ�*/
      if(!info)   /*���ָ��infoΪ��*/
      {
	 printf("\nout of memory");   /*����ڴ����*/
	 return NULL;           /*���ؿ�ָ��*/
      }
      inputs("enter no:",info->no,11);   /*����ѧ�Ų�У��*/
      if(info->no[0]=='@') break;    /*���ѧ�����ַ�Ϊ@���������*/
      inputs("enter name:",info->name,15); /*����������������У��*/
      printf("please input %d score \n",N);  /*��ʾ��ʼ����ɼ�*/
      s=0;         /*����ÿ��ѧ�����ܷ֣���ֵΪ0*/
      for(i=0;i<N;i++)    /*N�ſγ�ѭ��N��*/
      {
	 do{
	    printf("score%d:",i+1);    /*��ʾ����ڼ��ſγ�*/
	    scanf("%d",&info->score[i]);    /*����ɼ�*/
	    if(info->score[i]>100||info->score[i]<0) /*ȷ���ɼ���0~100֮��*/
	    printf("bad data,repeat input\n");  /*������ʾ��Ϣ*/
	 }while(info->score[i]>100||info->score[i]<0);
	 s=s+info->score[i];   /*�ۼӸ��ſγ̳ɼ�*/
      }
      info->sum=s;    /*���ֱܷ���*/
      info->average=(float)s/N;  /*���ƽ��ֵ*/
      info->order=0;    /*δ����ǰ��ֵΪ0*/
      info->next=h;   /*��ͷ�����Ϊ��������ĺ�̽��*/
      h=info;    /*��������Ϊ�µ�ͷ���*/
   }
   return(h);   /*����ͷָ��*/
}
/*�����ַ����������г�����֤*/
inputs(char *prompt, char *s, int count)
{
   char p[255];
   do{
      printf(prompt);  /*��ʾ��ʾ��Ϣ*/
      scanf("%s",p);  /*�����ַ���*/
      if(strlen(p)>count)printf("\n too long! \n"); /*���г���У�飬����countֵ������*/
   }while(strlen(p)>count);
   strcpy(s,p); /*��������ַ����������ַ���s��*/
}
/*��������н����Ϣ*/
void print(STUDENT *h)
{
   int i=0;     /* ͳ�Ƽ�¼����*/
   STUDENT *p;  /*�ƶ�ָ��*/
   clrscr();     /*����*/
   p=h;      /*��ֵΪͷָ��*/
   printf("\n\n\n****************************STUDENT********************************\n");
   printf("|rec|nO        |      name     | sc1| sc2| sc3|   sum  |  ave  |order|\n");
   printf("|---|----------|---------------|----|----|----|--------|-------|-----|\n");
   while(p!=NULL)
   {
       i++;
       printf("|%3d |%-10s|%-15s|%4d|%4d|%4d| %4.2f | %4.2f | %3d |\n", i, p->no,p->name,p->score[0],p->score[1],
p->score[2],p->sum,p->average,p->order);
       p=p->next;
   }
   printf("**********************************end*********************************\n");
}
/*ɾ����¼*/
STUDENT *delete(STUDENT *h)
{
   STUDENT *p,*q;  /*pΪ���ҵ�Ҫɾ���Ľ��ָ�룬qΪ��ǰ��ָ��*/
   char s[11];     /*���ѧ��*/
   clrscr();       /*����*/
   printf("please deleted no\n");      /*��ʾ��ʾ��Ϣ*/
   scanf("%s",s);   /*����Ҫɾ����¼��ѧ��*/
   q=p=h;    /*��q��p����ֵͷָ��*/
   while(strcmp(p->no,s)&&p!=NULL)   /*����¼��ѧ�Ų���Ҫ�ҵģ���ָ�벻Ϊ��ʱ*/
   {
      q=p;       /*��pָ��ֵ����q��Ϊp��ǰ��ָ��*/
      p=p->next;      /*��pָ��ָ����һ����¼*/
   }
   if(p==NULL)     /*���pΪ�գ�˵��������û�иý��*/
      printf("\nlist no %s student\n",s);
   else       /*p��Ϊ�գ���ʾ�ҵ��ļ�¼��Ϣ*/
   {
      printf("*****************************have found***************************\n");
      printf("|no        |      name     | sc1| sc2| sc3|   sum  |  ave  |order|\n");
      printf("|----------|---------------|----|----|----|--------|-------|-----|\n");
      printf("|%-10s|%-15s|%4d|%4d|%4d| %4.2f | %4.2f | %3d |\n", p->no,
       p->name,p->score[0],p->score[1],p->score[2],p->sum,
       p->average,p->order);
      printf("********************************end*******************************\n");
      getch();      /*ѹ��һ���󣬿�ʼɾ��*/
      if(p==h)    /*���p==h��˵����ɾ�����ͷ���*/
	 h=p->next;     /*�޸�ͷָ��ָ����һ����¼*/
      else
	 q->next=p->next; /*����ͷָ�룬��p�ĺ�̽����Ϊq�ĺ�̽��*/
      free(p);          /*�ͷ�p��ָ���ռ�*/
      printf("\n have deleted No %s student\n",s);
      printf("Don't forget save\n");/*��ʾɾ����Ҫ���Ǳ����ļ�*/
   }
   return(h);      /*����ͷָ��*/
}
/*���Ҽ�¼*/
void search(STUDENT *h)
{
   STUDENT *p;    /*  �ƶ�ָ��*/
   char s[15];    /*����������ַ�����*/
   clrscr();    /*����Ļ*/
   printf("please enter name for search\n");
   scanf("%s",s);    /*��������*/
   p=h;    /*��ͷָ�븳��p*/
   while(strcmp(p->name,s)&&p!=NULL)  /*����¼����������Ҫ�ҵģ���ָ�벻Ϊ��ʱ*/
   p=p->next;     /*�ƶ�ָ�룬ָ����һ���*/
   if(p==NULL)         /*���ָ��Ϊ��*/
      printf("\nlist no %s student\n",s);   /*��ʾû�и�ѧ��*/
   else           /*��ʾ�ҵ��ļ�¼��Ϣ*/
   {
      printf("\n\n*****************************havefound***************************\n");
      printf("|nO        |      name     | sc1| sc2| sc3|   sum  |  ave  |order|\n");
      printf("|----------|---------------|----|----|----|--------|-------|-----|\n");
      printf("|%-10s|%-15s|%4d|%4d|%4d| %4.2f | %4.2f | %3d |\n", p->no,
p->name,p->score[0],p->score[1],p->score[2],p->sum,p->average,p->order);
      printf("********************************end*******************************\n");
   }
}
/*�����¼*/
STUDENT  *insert(STUDENT *h)
{
   STUDENT *p,*q,*info; /*pָ�����λ�ã�q����ǰ����infoָ�²����¼*/
   char s[11];  /*��������λ�õ�ѧ��*/
   int s1,i;
   printf("please enter location  before the no\n");
   scanf("%s",s);   /*��������ѧ��*/
   printf("\nplease new record\n");      /*��ʾ�����¼��Ϣ*/
   info=(STUDENT *)malloc(sizeof(STUDENT));   /*����ռ�*/
   if(!info)
   {
      printf("\nout of memory");   /*��û�����뵽���ڴ����*/
      return NULL;             /*���ؿ�ָ��*/
   }
   inputs("enter no:",info->no,11); /*����ѧ��*/
   inputs("enter name:",info->name,15); /*��������*/
   printf("please input %d score \n",N);  /*��ʾ�������*/
   s1=0;    /*�����¼�¼���ܷ֣���ֵΪ0*/
   for(i=0;i<N;i++)    /*N�ſγ�ѭ��N������ɼ�*/
   {
      do{        /*�����ݽ�����֤����֤��0~100֮��*/
	 printf("score%d:",i+1);
	 scanf("%d",&info->score[i]);
	 if(info->score[i]>100||info->score[i]<0)
	    printf("bad data,repeat input\n");
      }while(info->score[i]>100||info->score[i]<0);
      s1=s1+info->score[i];    /*�����ܷ�*/
   }
   info->sum=s1;    /*���ִܷ����¼�¼��*/
   info->average=(float)s1/N;  /*�������*/
   info->order=0;         /*���θ�ֵ0*/
   info->next=NULL;     /*����ָ��Ϊ��*/
   p=h;               /*��ָ�븳ֵ��p*/
   q=h;             /*��ָ�븳ֵ��q*/
   while(strcmp(p->no,s)&&p!=NULL)    /*���Ҳ���λ��*/
   {
      q=p;                 /*����ָ��p����Ϊ��һ��p��ǰ��*/
      p=p->next;          /*��ָ��p����*/
   }
   if(p==NULL)      /*���pָ��Ϊ�գ�˵��û��ָ�����*/
      if(p==h)      /*ͬʱp����h��˵������Ϊ��*/
	 h=info;   /*�¼�¼��Ϊͷ���*/
      else
	 q->next=info;  /*pΪ�գ���p������h�����½����ڱ�β*/
   else
      if(p==h)     /*p��Ϊ�գ����ҵ���ָ�����*/
      {
	 info->next=p; /*���p����h�����½������ڵ�һ�����֮ǰ*/
	 h=info;    /*�½��Ϊ�µ�ͷ���*/
      }
      else
      {
	 info->next=p;   /*����ͷ��㣬�����м�ĳ��λ�ã��½��ĺ��Ϊp*/
	 q->next=info;  /*�½����Ϊq�ĺ�̽��*/
      }
   printf("\n ----have inserted %s student----\n",info->name);    printf("---Don't forget save---\n");     /*��ʾ����*/
   return(h);         /*����ͷָ��*/
}
/*�������ݵ��ļ�*/
void save(STUDENT *h)
{
   FILE *fp;         /*����ָ���ļ���ָ��*/
   STUDENT *p;    /* �����ƶ�ָ��*/
   char outfile[10];  /*��������ļ���*/
   printf("Enter outfile name,for example c:\\f1\\te.txt:\n"); /*��ʾ�ļ�����ʽ��Ϣ*/
   scanf("%s",outfile);
   if((fp=fopen(outfile,"wb"))==NULL) /*Ϊ�����һ���������ļ�����û������*/
   {
      printf("can not open file\n");
      exit(1);
   }
   printf("\nSaving file......\n");  /*���ļ�����ʾ���ڱ���*/
   p=h;                    /*�ƶ�ָ���ͷָ�뿪ʼ*/
   while(p!=NULL)        /*��p��Ϊ��*/
   {
      fwrite(p,sizeof(STUDENT),1,fp);/*д��һ����¼*/
      p=p->next;        /*ָ�����*/
   }
   fclose(fp);      /*�ر��ļ�*/
   printf("-----save success!!-----\n");  /*��ʾ����ɹ�*/
}
/* ���ļ�������*/
STUDENT *load()
{
   STUDENT *p,*q,*h=NULL;    /*�����¼ָ�����*/
   FILE *fp;            /* ����ָ���ļ���ָ��*/
   char infile[10];       /*�����ļ���*/
   printf("Enter infile name,for example c:\\f1\\te.txt:\n");   scanf("%s",infile);           /*�����ļ���*/
   if((fp=fopen(infile,"rb"))==NULL)   /*��һ���������ļ���Ϊ����ʽ*/
   {
      printf("can not open file\n");    /*�粻�ܴ򿪣����������*/
      exit(1);
   }
   printf("\n -----Loading file!-----\n");
   p=(STUDENT *)malloc(sizeof(STUDENT));   /*����ռ�*/
   if(!p)
   {
      printf("out of memory!\n");    /*��û�����뵽�����ڴ����*/
      return h;       /*���ؿ�ͷָ��*/
   }
   h=p;         /*���뵽�ռ䣬������Ϊͷָ��*/
   while(!feof(fp))  /*ѭ��������ֱ���ļ�β����*/
   {
      if(1!=fread(p,sizeof(STUDENT),1,fp))
	 break;  /*���û�������ݣ�����ѭ��*/
      p->next=(STUDENT *)malloc(sizeof(STUDENT));  /*Ϊ��һ���������ռ�*/
      if(!p->next)
      {
	 printf("out of memory!\n");  /*��û�����뵽�����ڴ����*/
	 return h;
      }
      q=p;   /*���浱ǰ����ָ�룬��Ϊ��һ����ǰ��*/
      p=p->next;   /*ָ����ƣ��¶�������������ǰ��β*/
   }
   q->next=NULL;   /*���һ�����ĺ��ָ��Ϊ��*/
   fclose(fp);       /*�ر��ļ�*/
   printf("---You have success read data from file!!!---\n");
   return h;   /*����ͷָ��*/
}
/*׷�Ӽ�¼���ļ�*/
void append()
{
   FILE *fp;      /*����ָ���ļ���ָ��*/
   STUDENT *info;    /*�¼�¼ָ��*/
   int s1,i;
   char infile[10];    /*�����ļ���*/
   printf("\nplease new record\n");
   info=(STUDENT *)malloc(sizeof(STUDENT));   /*����ռ�*/
   if(!info)
   {
      printf("\nout of memory");   /*û�����뵽���ڴ��������������*/
      return ;
   }
   inputs("enter no:",info->no,11);    /*����inputs����ѧ��*/
   inputs("enter name:",info->name,15); /*����inputs��������*/
   printf("please input %d score \n",N);   /*��ʾ����ɼ�*/
   s1=0;
   for(i=0;i<N;i++)
   {
      do{
	 printf("score%d:",i+1);
	 scanf("%d",&info->score[i]);  /*����ɼ�*/
	 if(info->score[i]>100||info->score[i]<0)printf("bad data,repeat input\n");
      }while(info->score[i]>100||info->score[i]<0); /*�ɼ�������֤*/
      s1=s1+info->score[i];      /*���ܷ�*/
   }
   info->sum=s1;      /*�����ܷ�*/
   info->average=(float)s1/N;  /*�����*/
   info->order=0;      /*���γ�ʼֵΪ0*/
   info->next=NULL;  /*���¼�¼���ָ�븳ֵΪ��*/
   printf("Enter infile name,for example c:\\f1\\te.txt:\n");    scanf("%s",infile);         /*�����ļ���*/
   if((fp=fopen(infile,"ab"))==NULL)  /*��������ļ�β�������ݷ�ʽ���ļ�*/
   {
      printf("can not open file\n");   /*��ʾ���ܴ�*/
      exit(1);             /*�˳�����*/
   }
   printf("\n -----Appending record!-----\n");
   if(1!=fwrite(info,sizeof(STUDENT),1,fp))   /*д�ļ�����*/
   {
      printf("-----file write error!-----\n");
      return;              /*����*/
   }
   printf("-----append  sucess!!----\n");
   fclose(fp);       /*�ر��ļ�*/
}
/*�ļ�����*/
void copy()
{
   char outfile[10],infile[10];
   FILE *sfp,*tfp;        /*Դ��Ŀ���ļ�ָ��*/
   STUDENT *p=NULL;   /*�ƶ�ָ��*/
   clrscr();    /*����*/
   printf("Enter infile name,for example c:\\f1\\te.txt:\n");
   scanf("%s",infile);         /*����Դ�ļ���*/
   if((sfp=fopen(infile,"rb"))==NULL)   /*�����ƶ���ʽ��Դ�ļ�*/
   {
      printf("can not open input file\n");
      exit(0);
   }
   printf("Enter outfile name,for example c:\\f1\\te.txt:\n");   /*��ʾ����Ŀ���ļ���*/
   scanf("%s",outfile);  /*����Ŀ���ļ���*/
   if((tfp=fopen(outfile,"wb"))==NULL)  /*������д��ʽ��Ŀ���ļ�*/
   {
      printf("can not open output file \n");
      exit(0);
   }
   while(!feof(sfp))   /*���ļ�ֱ���ļ�β*/
   {
      if(1!=fread(p,sizeof(STUDENT),1,sfp))
	 break; /*���*/
      fwrite(p,sizeof(STUDENT),1,tfp);   /*��д*/
   }
   fclose(sfp);    /*�ر�Դ�ļ�*/
   fclose(tfp);   /*�ر�Ŀ���ļ�*/
   printf("you have success copy  file!!!\n");   /*��ʾ�ɹ�����*/
}
/*����*/
STUDENT *sort(STUDENT *h)
{
   int i=0;                  /*��������*/
   STUDENT *p,*q,*t,*h1;   /*������ʱָ��*/
   h1=h->next;           /*��ԭ����ͷָ����ָ����һ�������ͷָ��*/
   h->next=NULL;        /*��һ�����Ϊ�±���ͷ���*/
   while(h1!=NULL)    /*��ԭ����Ϊ��ʱ����������*/
   {
      t=h1;            /*ȡԭ����ͷ���*/
      h1=h1->next;     /*ԭ��ͷ���ָ�����*/
      p=h;           /*�趨�ƶ�ָ��p����ͷָ�뿪ʼ*/
      q=h;         /*�趨�ƶ�ָ��q��Ϊp��ǰ������ֵΪͷָ��*/
      while(t->sum<p->sum&&p!=NULL)  /*���ֱܷȽ�*/
      {
	 q=p;            /*�������ֵС�����±�ָ�����*/
	 p=p->next;
      }
      if(p==q)      /*p==q��˵���������ֵ��Ӧ������λ*/
      {
	 t->next=p;     /*�������ĺ��Ϊp*/
	 h=t;         /*��ͷ���Ϊ�������*/
      }
      else    /*�������Ӧ�������м�ĳ��λ��q��p֮�䣬��pΪ������β��*/
      {
	 t->next=p;   /*t�ĺ����p*/
	 q->next=t;    /*q�ĺ����t*/
      }
   }
   p=h;               /*���ź����ͷָ�븳��p��׼����д����*/
   while(p!=NULL)  /*��p��Ϊ��ʱ���������в���*/
   {
      i++;       /*������*/
      p->order=i;   /*�����θ�ֵ*/
      p=p->next;   /*ָ�����*/
   }
   printf("sort sucess!!!\n");   /*����ɹ�*/
   return h;      /*����ͷָ��*/
}
/*�����ֺܷ;�ֵ*/
void computer(STUDENT *h)
{
   STUDENT *p;   /*�����ƶ�ָ��*/
   int i=0;  /*�����¼������ֵΪ0*/
   long s=0;   /*�ֳܷ�ֵΪ0*/
   float average=0;  /*���ֳ�ֵΪ0*/
   p=h;        /*��ͷָ�뿪ʼ*/
   while(p!=NULL)    /*��p��Ϊ��ʱ����*/
   {
      s+=p->sum;    /*�ۼ��ܷ�*/
      i++;         /*ͳ�Ƽ�¼����*/
      p=p->next;   /*ָ�����*/
   }
   average=(float)s/i;/* ����֣�����Ϊ���������ܷ�Ϊ����������������ת��*/
   printf("\n--All students sum score is:%ld  average is %5.2f\n",s,average);
}
/*����*/
STUDENT *index(STUDENT *h)
{
   STUDENT *p,*q,*t,*h1;  /*������ʱָ��*/
   h1=h->next;    /*��ԭ����ͷָ����ָ����һ�������ͷָ��*/
   h->next=NULL;   /*��һ�����Ϊ�±���ͷ���*/
   while(h1!=NULL)  /*��ԭ����Ϊ��ʱ����������*/
   {
      t=h1;        /*ȡԭ����ͷ���*/
      h1=h1->next;   /*ԭ��ͷ���ָ�����*/
      p=h;   /*�趨�ƶ�ָ��p����ͷָ�뿪ʼ*/
      q=h;   /*�趨�ƶ�ָ��q��Ϊp��ǰ������ֵΪͷָ��*/
      while(strcmp(t->no,p->no)>0&&p!=NULL)  /*��ѧ�űȽ�*/
      {
	 q=p;           /*�������ֵ��Ӧ����壬�����±�ָ�����*/
	 p=p->next;
      }
      if(p==q)   /*p==q��˵���������ֵС��Ӧ������λ*/
      {
	 t->next=p;  /*�������ĺ��Ϊp*/
	 h=t;    /*��ͷ���Ϊ�������*/
      }
      else     /*�������Ӧ�������м�ĳ��λ��q��p֮�䣬��pΪ������β��*/
      {
	 t->next=p;    /*t�ĺ����p*/
	 q->next=t;     /*q�ĺ����t*/
      }
   }
   printf("index sucess!!!\n");  /*��������ɹ�*/
   return h;    /*����ͷָ��*/
}
/*����ϼ�*/
void total(STUDENT *h)
{
   STUDENT *p,*q;   /*������ʱָ�����*/
   char sno[9],qno[9],*ptr;    /*����༶�ŵ�*/
   float s1,ave;        /*�����ֺܷ;���*/
   int i;       /*����༶����*/
   clrscr();    /*����*/
   printf("\n\n  *******************Total*****************\n");
   printf("---class---------sum--------------average----\n");
   p=h;       /*��ͷָ�뿪ʼ*/
   while(p!=NULL)  /*��p��Ϊ��ʱ������Ĵ���*/
   {
      memcpy(sno,p->no,8);  /*��ѧ����ȡ���༶��*/
      sno[8]='\0';          /*���ַ����������*/
      q=p->next;        /*��ָ��ָ����Ƚϵļ�¼*/
      s1=p->sum;      /*��ǰ�༶���ֳܷ�ֵΪ�ð༶�ĵ�һ����¼�ܷ�*/
      ave=p->average;  /*��ǰ�༶�ľ��ֳ�ֵΪ�ð༶�ĵ�һ����¼����*/
      i=1;          /*ͳ�Ƶ�ǰ�༶����*/
      while(q!=NULL)   /*��ѭ����ʼ*/
      {
	 memcpy(qno,q->no,8);   /*��ȡ�༶��*/
	 qno[8]='\0';     /*���ַ����������*/
	 if(strcmp(qno,sno)==0)  /*�Ƚϰ༶��*/
	 {
	    s1+=q->sum;     /*�ۼ��ܷ�*/
	    ave+=q->average; /*�ۼӾ���*/
	    i++;         /*�ۼӰ༶����*/
	    q=q->next;   /*ָ��ָ����һ����¼*/
	 }
	 else
	    break;  /*����һ���༶�Ľ���������ѭ��*/
      }
      printf("%s     %10.2f           %5.2f\n",sno,s1,ave/i);
      if(q==NULL)
	 break;   /*�����ǰָ��Ϊ�գ���ѭ���������������*/
      else
	 p=q;   /*���򣬽���ǰ��¼��Ϊ�µİ༶�ĵ�һ����¼��ʼ�µıȽ�*/
   }
   printf("---------------------------------------------\n");
}
