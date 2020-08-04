/*13. teacherfun.c源程序*/
/*** teacherfun.c ***/
#include "teacher.h"  /*根据实际存放位置修改此路径*/
/*初始化双链表*/
void init()
{
   First=(TEACHER *)malloc(sizeof(TEACHER)); /*为头结点申请空间*/
   Last=First; /*将尾指针指向头结点*/
   First->prior=Last; /*设置头结点的前驱指针*/
   Last->next=First; /*设置头结点的后继指针*/
   p=First; /*设置当前记录指针为头结点*/
}

/*创建教师信息循环双链表*/
void create()
{
   int unit,flag=0;
   float temp;
   TEACHER *info; /*新增结点*/
   init();
   for(;;)
   {
      if(flag==1)break; /*标志为1，不再输入*/
      clrscr(); /*清屏*/
      printf("Please enter teacher infomation\n");
      printf("input @ end enter\n");
      info=(TEACHER *)malloc(sizeof(TEACHER));/*为新增结点申请空间*/
      if(!info) /*没有空间出错处理*/
      {
	 printf("\nout of memory");
	 exit(0);
      }
      printf("No:"); /*开始提示输入*/
      scanf("%s",info->no);
      if(info->no[0]=='@')/*输入@结束输入*/
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
	 if(strcmp(info->profess,"prof"))unit=25; /*教授*/
	 if(strcmp(info->profess,"aprof"))unit=20;/*副教授*/
	 if(strcmp(info->profess,"lect"))unit=15;/*讲师*/
	 if(strcmp(info->profess,"ass"))unit=10;/*助教*/
	 info->lessonf=unit*info->workload;/*根据职称计算代课费*/
	 info->next=Last->next;/*新插入结点插在表末尾*/
	 info->prior=Last; /*新结点的前驱为原来的尾结点*/
	 Last->next=info; /*原来尾结点的后继为新结点*/
	 Last=info; /*新的尾结点为新结点*/
	 First->prior=Last;/*头结点的前驱为尾指针*/
      }
   return;
}
/*显示第一条记录*/
void firstr()
{
   if(First==Last)return;
   clear();
   p=First->next;
   print(p);
}
/*显示最后一条记录*/
void lastr()
{
   if(First==Last)return;
      clear();
   p=Last;
   print(p);
}
/*显示前一条记录*/
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
/*显示下一条记录*/
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
/*从文件读数据*/
void load()
{
   TEACHER *p1;
   FILE *fp;
   if((fp=fopen("data.txt","rb"))==NULL)
   {
      printf("can not open file\n");
      return;
   }
   while(First->next!=First) /*如果当前表不空，删除当前表*/
   {
      p1=First->next;
      First->next=p1->next;
      free(p1);
   }
   free(First);
   First=(TEACHER*)malloc(sizeof(TEACHER)); /*创建头结点*/
   if(!First)
   {
      printf("out of memory!\n");
      return;
   }
   Last=First;
   First->prior=Last;
   Last->next=First;
   p=First;
   while(!feof(fp)) /*当文件不为空时读数据*/
   {
      p1=(TEACHER*)malloc(sizeof(TEACHER));
      if(!p1)
      {
	 printf("out of memory!\n");
	 return;
      }
      if(1!=fread(p1,sizeof(TEACHER),1,fp))
	 break; /*读数据*/
      p1->next=Last->next; /*将新读出的数据链在当前表尾*/
      p1->prior=Last;
      Last->next=p1;
      Last=Last->next;
      First->prior=Last;
   }
   fclose(fp); /*关闭文件*/
}
/*保存数据到磁盘文件*/
void save()
{
   FILE *fp;  /*定义指向文件的指针*/
   TEACHER *p1; /* 定义移动指针*/
   if((fp=fopen("data.txt","wb"))==NULL) /*为输出打开一个文本文件，如没有则建立*/
   {
      printf("can not open file\n"); /*如不能打开文件，显示提示信息，结束程序*/
      return;  /*返回*/
   }
   p1=First; /*移动指针从头指针开始*/
   while(p1->next!=First) /*如p1不为空*/
   {
      fwrite(p1->next,sizeof(TEACHER),1,fp); /*写入一条记录*/
      p1=p1->next;  /*指针后移，处理下一条记录*/
   }
   fclose(fp); /*关闭文件*/
}
/*删除记录*/
void delete()
{
   TEACHER *p1;
   if(First==Last)
      return;/*表为空*/
   if(p==First) /*p为头结点*/
      p=First->next;
   if(p==Last)/*p为尾结点*/
      Last=p->prior;
   p1=p; /*一般情况*/
   p=p->next;
   p1->prior->next=p1->next;
   p1->next->prior=p1->prior;
   free(p1);
}
/*输出记录*/
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
/*****清除界面显示信息******/
void clear()
{
   int x1=70,y1=100,m,n;
   for(m=0;m<4;m++)
      for(n=0;n<2;n++)
      {
	 setfillstyle(1,WHITE);/*白色覆盖原有信息*/
	 bar(x1+n*250+100,y1+50+m*30+20,x1+n*250+200,y1+50+m*30+40);
      }
}
/*sort排序函数*/
void sort()
{
   TEACHER *p0,*p00,*p1,*p11,*templast;
   if(First->next==First||First->next->next==First)return;
   p00=First; /*作排好序表的表头和第一个结点*/
   p0=First->next;
   p1=p0->next;
   First->prior=p0;
   p0->next=First;
   templast=p0;
   while(p1!=First) /*当p1没有转回到表头时*/
   {
      p11=p1;   /*将p11作为待插入结点*/
      p1=p1->next; /*p1指向下一个待排序结点*/
      p00=First; /*从头结点开始寻找插入位置*/
      p0=p00->next; /*p0是p00的后继*/
      while(p0!=First&&p11->workload>p0->workload)
      {
	 p00=p0;/*当新插入结点比当前表结点大时，指针后移*/
	 p0=p0->next;
      }
      if( p0==First)/*如果p0移到了头结点*/
      {
	 p11->next=p00->next;
	 p11->prior=p00;
	 p00->next=p11;
	 p0->prior=p11;
	 templast=p11;
      }
      else  /*新插入结点介于p00和p0之间*/
      {
	 p11->next=p0;
	 p11->prior=p00;
	 p0->prior=p11;
	 p00->next=p11;
      }
   }
   Last=templast; /*设置尾指针*/
   p=First; /*设置当前记录指针*/
}
