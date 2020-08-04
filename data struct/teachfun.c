/*13. teacherfun.cԴ����*/
/*** teacherfun.c ***/
#include "teacher.h"  /*����ʵ�ʴ��λ���޸Ĵ�·��*/
/*��ʼ��˫����*/
void init()
{
   First=(TEACHER *)malloc(sizeof(TEACHER)); /*Ϊͷ�������ռ�*/
   Last=First; /*��βָ��ָ��ͷ���*/
   First->prior=Last; /*����ͷ����ǰ��ָ��*/
   Last->next=First; /*����ͷ���ĺ��ָ��*/
   p=First; /*���õ�ǰ��¼ָ��Ϊͷ���*/
}

/*������ʦ��Ϣѭ��˫����*/
void create()
{
   int unit,flag=0;
   float temp;
   TEACHER *info; /*�������*/
   init();
   for(;;)
   {
      if(flag==1)break; /*��־Ϊ1����������*/
      clrscr(); /*����*/
      printf("Please enter teacher infomation\n");
      printf("input @ end enter\n");
      info=(TEACHER *)malloc(sizeof(TEACHER));/*Ϊ�����������ռ�*/
      if(!info) /*û�пռ������*/
      {
	 printf("\nout of memory");
	 exit(0);
      }
      printf("No:"); /*��ʼ��ʾ����*/
      scanf("%s",info->no);
      if(info->no[0]=='@')/*����@��������*/
      {
	 flag=1;break;}
	 printf("Name:");
	 scanf("%s",info->name);
	 printf("Sex:");
	 scanf("%s",info->sex);
	 printf("Profess:");
	 scanf("%s",info->profess);
	 printf("Dept:");
	 scanf("%s",info->dept);
	 printf("Class:");
	 scanf("%s",info->class);
	 printf("Workload:");
	 scanf("%f",&temp);
	 info->workload=temp;
	 if(strcmp(info->profess,"prof"))unit=25; /*����*/
	 if(strcmp(info->profess,"aprof"))unit=20;/*������*/
	 if(strcmp(info->profess,"lect"))unit=15;/*��ʦ*/
	 if(strcmp(info->profess,"ass"))unit=10;/*����*/
	 info->lessonf=unit*info->workload;/*����ְ�Ƽ�����η�*/
	 info->next=Last->next;/*�²�������ڱ�ĩβ*/
	 info->prior=Last; /*�½���ǰ��Ϊԭ����β���*/
	 Last->next=info; /*ԭ��β���ĺ��Ϊ�½��*/
	 Last=info; /*�µ�β���Ϊ�½��*/
	 First->prior=Last;/*ͷ����ǰ��Ϊβָ��*/
      }
   return;
}
/*��ʾ��һ����¼*/
void firstr()
{
   if(First==Last)return;
   clear();
   p=First->next;
   print(p);
}
/*��ʾ���һ����¼*/
void lastr()
{
   if(First==Last)return;
      clear();
   p=Last;
   print(p);
}
/*��ʾǰһ����¼*/
void priorr()
{
   if(First==Last)
      return;
   if(p->prior!=First)
      p=p->prior;
   else
      p=Last;
   clear();
   print(p);
}
/*��ʾ��һ����¼*/
void nextr()
{
   if(First==Last)
      return;
   if(p==Last)
      p=First->next;
   else
      p=p->next;
   clear();
   print(p);
}
/*���ļ�������*/
void load()
{
   TEACHER *p1;
   FILE *fp;
   if((fp=fopen("data.txt","rb"))==NULL)
   {
      printf("can not open file\n");
      return;
   }
   while(First->next!=First) /*�����ǰ���գ�ɾ����ǰ��*/
   {
      p1=First->next;
      First->next=p1->next;
      free(p1);
   }
   free(First);
   First=(TEACHER*)malloc(sizeof(TEACHER)); /*����ͷ���*/
   if(!First)
   {
      printf("out of memory!\n");
      return;
   }
   Last=First;
   First->prior=Last;
   Last->next=First;
   p=First;
   while(!feof(fp)) /*���ļ���Ϊ��ʱ������*/
   {
      p1=(TEACHER*)malloc(sizeof(TEACHER));
      if(!p1)
      {
	 printf("out of memory!\n");
	 return;
      }
      if(1!=fread(p1,sizeof(TEACHER),1,fp))
	 break; /*������*/
      p1->next=Last->next; /*���¶������������ڵ�ǰ��β*/
      p1->prior=Last;
      Last->next=p1;
      Last=Last->next;
      First->prior=Last;
   }
   fclose(fp); /*�ر��ļ�*/
}
/*�������ݵ������ļ�*/
void save()
{
   FILE *fp;  /*����ָ���ļ���ָ��*/
   TEACHER *p1; /* �����ƶ�ָ��*/
   if((fp=fopen("data.txt","wb"))==NULL) /*Ϊ�����һ���ı��ļ�����û������*/
   {
      printf("can not open file\n"); /*�粻�ܴ��ļ�����ʾ��ʾ��Ϣ����������*/
      return;  /*����*/
   }
   p1=First; /*�ƶ�ָ���ͷָ�뿪ʼ*/
   while(p1->next!=First) /*��p1��Ϊ��*/
   {
      fwrite(p1->next,sizeof(TEACHER),1,fp); /*д��һ����¼*/
      p1=p1->next;  /*ָ����ƣ�������һ����¼*/
   }
   fclose(fp); /*�ر��ļ�*/
}
/*ɾ����¼*/
void delete()
{
   TEACHER *p1;
   if(First==Last)
      return;/*��Ϊ��*/
   if(p==First) /*pΪͷ���*/
      p=First->next;
   if(p==Last)/*pΪβ���*/
      Last=p->prior;
   p1=p; /*һ�����*/
   p=p->next;
   p1->prior->next=p1->next;
   p1->next->prior=p1->prior;
   free(p1);
}
/*�����¼*/
void print(TEACHER *p)
{
   int x1=70,y1=100;
   char str[20];
   outtextxy(x1+110,y1+75, p->no);
   outtextxy(x1+360,y1+75,p->name);
   outtextxy(x1+110,y1+105,p->sex);
   outtextxy(x1+360,y1+105,p->profess);
   outtextxy(x1+110,y1+135,p->dept);
   outtextxy(x1+360,y1+135,p->class);
   sprintf(str,"%f",p->workload);
   outtextxy(x1+110,y1+165,str);
   sprintf(str,"%f",p->lessonf);
   outtextxy(x1+360,y1+165,str);
}
/*****���������ʾ��Ϣ******/
void clear()
{
   int x1=70,y1=100,m,n;
   for(m=0;m<4;m++)
      for(n=0;n<2;n++)
      {
	 setfillstyle(1,WHITE);/*��ɫ����ԭ����Ϣ*/
	 bar(x1+n*250+100,y1+50+m*30+20,x1+n*250+200,y1+50+m*30+40);
      }
}
/*sort������*/
void sort()
{
   TEACHER *p0,*p00,*p1,*p11,*templast;
   if(First->next==First||First->next->next==First)return;
   p00=First; /*���ź����ı�ͷ�͵�һ�����*/
   p0=First->next;
   p1=p0->next;
   First->prior=p0;
   p0->next=First;
   templast=p0;
   while(p1!=First) /*��p1û��ת�ص���ͷʱ*/
   {
      p11=p1;   /*��p11��Ϊ��������*/
      p1=p1->next; /*p1ָ����һ����������*/
      p00=First; /*��ͷ��㿪ʼѰ�Ҳ���λ��*/
      p0=p00->next; /*p0��p00�ĺ��*/
      while(p0!=First&&p11->workload>p0->workload)
      {
	 p00=p0;/*���²�����ȵ�ǰ�����ʱ��ָ�����*/
	 p0=p0->next;
      }
      if( p0==First)/*���p0�Ƶ���ͷ���*/
      {
	 p11->next=p00->next;
	 p11->prior=p00;
	 p00->next=p11;
	 p0->prior=p11;
	 templast=p11;
      }
      else  /*�²��������p00��p0֮��*/
      {
	 p11->next=p0;
	 p11->prior=p00;
	 p0->prior=p11;
	 p00->next=p11;
      }
   }
   Last=templast; /*����βָ��*/
   p=First; /*���õ�ǰ��¼ָ��*/
}
