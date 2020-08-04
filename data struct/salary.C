/*12.3.2 源程序*/
/******头文件（.h）***********/
#include "stdio.h"    /*I/O函数*/
# include "bios.h"  /*ROM基本输入输出函数*/
#include "dos.h"    /*dos接口函数*/
#include "conio.h"   /*屏幕操作函数*/
#include "stdlib.h"   /*其它说明*/
#include "string.h"  /*字符串函数*/
#include "mem.h"  /*内存操作函数*/
#include "ctype.h" /*字符操作函数*/
#include "alloc.h"  /*动态地址分配函数*/
/****变量定义*******/
typedef struct z1    /*定义数据结构*/
{
   char no[11];     /*编号*/
   char name[15];  /*姓名*/
   float jbgz;     /*基本工资*/
   float koukuan; /*扣款*/
   float yfgz;  /*应发工资*/
   float shuijin;  /*税金*/
   float sfgz;   /*实发工资*/
   struct z1 *prior;/*前驱指针*/
   struct z1 *next;/*后继指针*/
}SALARY;     /*结构体类型名*/
struct z1 *First;   /*双链表头指针*/
struct z1 *Last;   /*双链表尾指针*/
/******函数原型*********/
void init();    /*初始化*/
void create();  /*创建链表*/
void calc();   /*计算应发工资*/
void delete(); /*删除*/
void search(); /*查找*/
void save();  /*保存文件*/
void load();  /*读取文件*/
void computer(); /*计算所需各种票面张数*/
void insert();  /*插入*/
void append(); /*追加*/
void copy(); /*复制文件*/
void sort();  /*排序*/
void index();  /*索引*/
void total();  /*分类合计*/
void list(); /*显示所有数据*/
void print(SALARY *p);  /*输出单条记录*/
void display();  /*随意逐条显示*/
float fax(float x);  /*计算税金*/
int menu_select();  /*主菜单*/
/*******主函数开始**********/
main()
{
   int i;
   clrscr();
   for(;;)
   {
      switch(menu_select())   /*调用菜单函数返回一个整数值*/
      {
	 case 0:init();break;    /*初始化*/
	 case 1:create();break; /*输入数据创建双链表*/
	 case 2:list();break;  /*显示所有数据*/
	 case 3:display();break;  /*单条显示*/
	 case 4:calc();break;  /*计算实发工资*/
	 case 5:search();break;  /*查找职工信息数据*/
	 case 6:delete();break;  /*删除记录*/
	 case 7:insert();break;  /*插入记录*/
	 case 8:append();break;  /*追加记录*/
	 case 9:save();break;  /*保存文件*/
	 case 10:load(); break;  /*读取文件*/
	 case 11:copy();break;  /*复制文件*/
	 case 12:sort();break;  /*按实发工资升序排序*/
	 case 13:computer();break; /*计算所需票面数*/
	 case 14:index();break;  /*按职工号索引*/
	 case 15:total();break;  /*按部门求合计*/
	 case 16:exit(0);    /*退出*/
      }
   }
}
/*菜单函数，函数返回值为整型，代表所选的菜单项*/
menu_select()
{
   char *f[]= {     /*定义菜单字符串数组*/
   "**************MENU*************", /*菜单的标题行*/
   " 0. init list", /*初始化双链表*/
   " 1. Enter list", /*输入数据，创建双链表*/
   " 2. List all \n",  /*显示所有记录*/
   " 3. Display record by step", /*显示单条记录*/
   " 4. Calc the salary", /*计算实发工资*/
   " 5. Search record on name", /*查找记录*/
   " 6. Delete a record", /*删除记录*/
   " 7. Insert record to list", /*插入记录*/
   " 8. Append  record to file", /*追加记录*/
   " 9. Save data to the file",  /*保存文件*/
   " 10. Load data from the file",/*读取文件*/
   " 11. Copy the file to new file", /*复制文件*/
   " 12. Sort on sfgz",  /*排序*/
   " 13. Computer ticket number", /*计算所需票面数*/
   " 14. Index on number",  /*索引*/
   " 15. Total on number",  /*分类合计*/
   " 16. Quit" };  /*退出*/
   char s[80];
   int  i;
   int key=0; /*记录所压键值*/
   int c=0;
   gotoxy(1,25); /*移动光标*/
   printf("press any key enter menu......\n");/*压任意键进入主菜单*/
   getch();
   clrscr();  /*清屏*/
   textcolor(YELLOW); /*设置文本颜色为黄色*/
   textbackground(BLUE);  /*设置背景颜色为兰色*/
   gotoxy(10,2);
   putch(0xda); /*输出左上角边框┏*/
   for(i=1;i<44;i++)
   putch(0xc4); /*输出上边框水平线*/
   putch(0xbf);  /*输出右上角边框  ┓*/
   for(i=3;i<22;i++)/*输出左右两边的垂直线*/
   {
      gotoxy(10,i);putch(0xb3);
      gotoxy(54,i);putch(0xb3);
   }
   gotoxy(10,22);putch(0xc0); /*输出左上角边框┗*/
   for(i=1;i<44;i++)
      putch(0xc4);  /*输出下边框水平线*/
   putch(0xd9);  /*输出右下角边框┛*/
   window(11,3,53,21); /* 制作显示菜单的窗口，大小根据菜单条数设计*/
   clrscr();   /*清屏*/
   for(i=0;i<18;i++)
   {
      gotoxy(10,i+1);
      cprintf("%s",f[i]); /*输出菜单项数组*/
   }
   i=1;
   gotoxy(10,2);  /*设置默认选项在第一项*/
   textbackground(LIGHTGREEN);/*设置背景颜色为浅绿*/
   cprintf("%s",f[1]);  /*输出菜单项，表示选中*/
   gotoxy(10,2);  /*移动光标到菜单的第一项*/
   while(key!=13)   /*所压键不是回车键时*/
   {
      while(bioskey(1)==0);  /*查询是否压下了一个键*/
      key=bioskey(0);   /*返回下一个在键盘压下的键*/
      key=key&0xff?key&0xff:key>>8; /*对所压的键进行判断*/
      gotoxy(10,i+1);
      textbackground(BLUE);/*设置背景颜色为蓝色*/
      cprintf("%s",f[i]);  /*输出菜单项*/
      if(key==72) i=i==1?17:i-1; /*如压向上光标键↑，i减1，如已到第一行再上移，则到最后一行*/
      if(key==80)i=i==17?1:i+1; /*如压向下光标键↓，i加1，如已到最后一行再下移，则到第一行*/
      gotoxy(10,i+1); /*光标移动i的下一项*/
      textbackground(LIGHTGREEN); /*将背景颜色设为浅绿*/
      cprintf("%s",f[i]);  /*输出菜单项*/
      c=i-1;  /*给代表菜单选项的整数赋值*/
   }
   textbackground(BLACK);  /*设置背景颜色为黑色*/
   window(1,1,80,25);  /*恢复原窗口大小*/
   return c;  /*返回代表菜单选项的整数值*/
}
/*初始化函数*/
void init()
{
   First=NULL;
   Last=NULL;
}
/*输入数据，创建双链表*/
void create()
{
   int x; /*记录行坐标*/
   int i; /*记录输入记录数*/
   int flag=0; /*做结束标记*/
   float temp;   /*定义临时变量*/
   SALARY *info,*p; /*定义临时变量*/
   if(First!=NULL)
   init();/*如果头指针为空，调用初始化函数*/
   p=First; /*从头指针开始*/
   for(;;)
   {
      if(flag==1)
	 break; /*如果flag=1，结束输入*/
      i=0;
      x=0;   /*确定移动的行坐标*/
      clrscr(); /*清屏*/
      gotoxy(1,3);
      printf("*************gongziguanli*************");/*输出标题*/
      gotoxy(1,4);
      printf("                      --Enter @ end--");/*提示输入@结束*/
      gotoxy(1,5);
      printf("|------------------------------------|");/*输出表格的起始线*/
      gotoxy(1,6);
      printf("|    no    |      name      |  jbgz  |");/*输出字段标题，注意空格数*/
      for(;;)
      {
	 gotoxy(1,7+x);
	 printf("|----------|----------------|--------|");/*输出表格的水平线*/
	 info=(SALARY *)malloc(sizeof(SALARY));/*申请一个记录空间*/
	 if(!info)
	 {
	    printf("\nout of memory");/*如没有得到空间，输出内存溢出信息*/
	    exit(0);/*退出程序*/
	 }
	 info->next=NULL;/*新结点的后继为空*/
	 info->prior=NULL;/*新结点的前驱为空*/
	 gotoxy(1,8+x);printf("|"); /*输出数据间的分割线*/
	 gotoxy(12,8+x);printf("|");
	 gotoxy(29,8+x);printf("|");
	 gotoxy(38,8+x);printf("|");
	 gotoxy(2,8+x);/*光标到输入编号位置*/
	 inputs(info->no,10);/*输入编号，并验证长度不超过10*/
	 if(info->no[0]=='@')
	 {
	    flag=1;
	    break;
	 } /*编号首字符为@结束输入*/
	 gotoxy(13,8+x);  /*光标到输入姓名位置*/
	 inputs(info->name,14); /*输入姓名，并验证长度不超过14*/
	 gotoxy(30,8+x); /*光标到输入基本工资位置*/
	 scanf("%f",&temp); /*输入基本工资到临时变量*/
	 info->jbgz=temp; /*基本工资赋值*/
	 info->koukuan=0; /*初始扣款为0，待计算工资时输入*/
	 info->sfgz=0; /*初始实发工资为0，待计算工资时计算*/
	 info->shuijin=0; /*初始税金为0，待计算工资时计算*/
	 info->yfgz=0; /*初始应发工资为0，待计算工资时计算*/
	 if(p==NULL)/*如果p为空，说明输入的是第一个结点*/
	 {
	    First=Last=info;/*头指针和尾指针*/
	    First->prior=NULL; /*头指针的前驱是空*/
	    Last->next=NULL; /*尾指针的后继是空*/
	 }
	 else /*插入的结点不是第一结点，则插入在头结点之前*/
	 {
	    info->next=p;/*新结点的后继指向原来的头结点*/
	    info->prior=p->prior;/*新结点的前驱指向原来的头结点的前驱*/
	    p->prior=info;/*原来结点的前驱指向新结点*/
	 }
	 p=info;  /*新结点变为p结点，暂时的头结点*/
	 First=info; /*新结点变为头结点*/
	 x+=2;   /*因为水平线，将光标下移两行*/
	 gotoxy(1,8+x);
	 i++;   /*输入记录数加1*/
	 if(i%9==0)
	    break; /*输入9个记录，换一页输入*/
      }
      gotoxy(1,8+x);
   }
   printf("|----------|----------------|--------|");/*输出结尾水平线*/
}
/*字符串输入和验证函数*/
inputs(char *s, int count)
{
   char p[255];
   do{
      scanf("%s",p);/*输入字符串*/
      if(strlen(p)>count)printf("\n too long! \n");/*进行长度校验，超过count值重输入*/
   }while(strlen(p)>count);
   strcpy(s,p); /*将输入的字符串拷贝到字符串s中*/
}
/*显示链表*/
void list()
{
   int i=0,n;  /*记录输出行数*/
   SALARY *p; /*定义移动指针*/
   clrscr();  /*清屏*/
   p=First;  /*从头指针开始*/
   while(i%10==0&&p!=NULL)  /*外循环控制换页*/
   {
      i=0;       /*记录一页的输出行数*/
      clrscr();   /*清屏*/
      printf("\n\n\n"); /*换行三次*/
      printf("*************************************SALARY***********************************\n");   /*输出大标题*/
      printf("|nO        |      name     |   jbgz  | koukuan |   yfgz  | shuijin |   sfgz  |\n");  /*输出字段标题*/
      printf("|----------|---------------|---------|---------|---------|---------|---------|\n");  /*输出水平线*/
      while(p!=NULL)  /*当p不为空时*/
      {
	 printf("|%-10s|%-15s|%9.2f|%9.2f|%9.2f|%9.2f|%9.2f|\n",
	 p->no,p->name,p->jbgz,p->koukuan,p->yfgz,p->shuijin,p->sfgz);
	 p=p->next;  /*指针后移*/
	 i++;   /*输出行数加1 */
	 if(i%10==0)
	    break;  /*如果满10行，跳出内循环*/
      }
      printf("****************************************end***********************************\n");  /*输出本页的结束行*/
      printf("Press any key congtinue...\n");
      getch(); /*输出一页暂停一下，压任意键继续*/
   }  /*返回到外循环，准备下一页的输出*/
}
/*单条显示记录*/
void display()
{
   int ch;   /*菜单选项字符*/
   SALARY *p;  /*定义临时指针*/
   clrscr();  /*清屏*/
   p=First;  /*从头指针开始*/
   if(p==NULL)  /*指针为空，即链表为空，返回主函数*/
   {
      printf("record is empty!\n");
      return;
   }
   else
   {
      print(p);   /*调用输出函数显示头结点信息*/
      for(;;)  /*循环开始*/
      {
	 printf("\n\n0. Quit 1. Prior      2.  Next      3.  First        4.Last \n\n");
/*菜单项Quit退出 Prior前驱 Next后继  First第一条 Last最后一条*/
	 printf("\nplease select 0~4:"); /*提示输入菜单选项0~4*/
	 scanf("%d",&ch); /*输入菜单选项0~4*/
	 if(ch==0) break;  /*如果选项为0，退出*/
	 switch(ch)   /*开关语句根据选项字符判断*/
	 {
	    case 1:p=p->prior;if(p==NULL)p=First;break;  /*指向前驱，没有前驱指向第一个*/
	    case 2:p=p->next;if(p==NULL) p=Last;break; /*指向后继，没有后继指向最后一个*/
	    case 3:p=First;break;  /*指向第一个结点*/
	    case 4:p=Last;break; /*指向最后一个结点*/
	 }
	 print(p);  /*调用输出函数显示指针所指结点信息*/
      }
   }
}
/*输出指针所指结点信息*/
void print(SALARY *p)
{
   if(p)   /*如果指针不为空，则显示信息*/
   {
      clrscr();  /*清屏*/
      printf("\n\n\n");  /*换行*/
      printf("************************************SALARY************************************\n");  /*输出标题信息*/
      printf("|no        |      name     |   jbgz  | koukuan|   yfgz  | shuijin |   sfgz   |\n"); /*输出字段标题*/
      printf("|----------|---------------|---------|---------|---------|---------|---------|\n"); /*输出分割线*/
      printf("|%-10s|%-15s|%9.2f|%9.2f|%9.2f|%9.2f|%9.2f|\n", p->no,p->name,
      p->jbgz,p->koukuan,p->yfgz,p->shuijin,p->sfgz);/*输出结点信息*/
      printf("***************************************end***************************** *******\n");  /*输出结束标记*/
   }
}
/*计算实发工资*/
void calc()
{
   int x; /*行坐标*/
   int i=0; /*记录数*/
   float temp; /*临时变量扣款*/
   SALARY *p;  /*移动指针*/
   clrscr();   /*清屏*/
   p=First;  /*从头指针开始*/
   while(p!=NULL)  /*当p不为空时，也就是有记录时，外循环控制换页*/
   {
      x=0;
      clrscr();
      gotoxy(1,3);
      printf("***************************gongziguanli************************************");   /*输出标题*/
      gotoxy(1,5);
      printf("|--------------------------------------------------------------------------|");
      gotoxy(1,6);/*下面输出字段标题*/
      printf("|    no    |      name      |  jbgz  |  koukuan |  yfgz  |shuijin |  sfgz  |");
      while(p!=NULL) /*当p不为空时，也就是有记录时，内循环控制处理9条*/
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
	 printf("%-10s",p->no); /*输出编号*/
	 gotoxy(13,8+x);
	 printf("%-15s",p->name); /*输出姓名*/
	 gotoxy(30,8+x);
	 printf("%.2f",p->jbgz);  /*输出基本工资*/
	 gotoxy(39,8+x);
	 scanf("%f",&temp);  /*输入扣款*/
	 p->koukuan=temp; /*扣款字段赋值*/
	 p->yfgz=p->jbgz-p->koukuan; /*计算应发工资=基本工资-扣款*/
	 gotoxy(50,8+x);
	 printf("%.2f",p->yfgz);  /*输出应发工资*/
	 gotoxy(59,8+x);
	 p->shuijin=(p->yfgz-1000)*fax(p->yfgz); /*否则按税率计算税金*/
	 printf("%.2f",p->shuijin);  /*输出税金*/
	 gotoxy(68,8+x);
	 p->sfgz=p->yfgz-p->shuijin;  /*计算实发工资*/
	 printf("%-8.2f",p->sfgz);  /*输出实发工资*/
	 x+=2;   /*行坐标下移两行*/
	 i++;   /*记录数加1*/
	 if(i%9==0)break;  /*记录数超过9条，跳出内循环*/
	 p=p->next;   /*指针后移，处理下一条*/
      }
      gotoxy(1,7+x);
      printf("|----------|----------------|--------|----------|--------|--------|--------|\n"); /*输出结束行*/
      printf("Press any key congtinue...\n");
      getch();
   }
}
/*根据应发工资，计算税率*/
float fax(float x)
{
   float f1;
   if(x<1000)
      return 0;   /*小于1000，返回0*/
   switch((int)(x/1000))
   {
      case 1:f1=0.05;break; /*1000~1999，返回0.05*/
      case 2:f1=0.1;break; /*2000~2999，返回0.1*/
      case 3:f1=0.15;break; /*3000~3999，返回0.15*/
      case 4:f1=0.2;break; /*4000~4999，返回0.2*/
      default:f1=0.3;break; /*5000以上，返回0.3*/
   }
   return f1;
}
/*计算票面张数*/
void computer()
{
   SALARY *p;
   int i=0,a[7]={0},t100,t50,t20,t10,t5,t2,t1;/*设置数组和临时变量保存票面张数*/
   float t;   /*过渡变量，保存当前剩余工资*/
   p=First;  /*移动指针从头指针开始*/
   while(p!=NULL)  /*当p不为空时*/
   {
      t100=(int)p->sfgz/100;  /*计算100元张数*/
      a[0]=a[0]+t100;   /*累加保存*/
      t=p->sfgz-t100*100; /*剩余工资*/
      t50=(int)(t/50); /*计算50元张数*/
      a[1]=a[1]+t50; /*累加保存*/
      t=t-t50*50; /*剩余工资*/
      t20=(int)(t/20);   /*计算20元张数*/
      a[2]=a[2]+t20; /*累加保存*/
      t=t-t20*20; /*剩余工资*/
      t10=(int)(t/10); /*计算10元张数*/
      a[3]=a[3]+t10; /*累加保存*/
      t=t-t10*10; /*剩余工资*/
      t5=(int)(t/5); /*计算5元张数*/
      a[4]=a[4]+t5; /*累加保存*/
      t=t-t5*5; /*剩余工资*/
      t2=(int)(t/2); /*计算2元张数*/
      a[5]=a[5]+t2; /*累加保存*/
      t=t-t2*2; /*剩余工资*/
      t1=(int)(t); /*计算1元张数*/
      a[6]=a[6]+t1; /*累加保存*/
      p=p->next;  /*指针后移，处理下一条记录*/
   }
   clrscr();  /*清屏*/
   printf("\n\n\n***********ticket number***************\n"); /*输出标题*/
   printf("--100-----50------20-----10-----5------2------1------\n");
     /*输出张数标题*/
   for(i=0;i<7;i++)
      printf("%5d  ",a[i]);/*输出所需的张数*/
   printf("\n");/*换行*/
}
/*按编号查找记录*/
SALARY *find(char *no)
{
   SALARY *p; /*定义移动指针*/
   p=First;  /*从头指针开始*/
   while(p) /*当p不为空时*/
   {
      if(!strcmp(no,p->no))
	 return p; /*比较相等，查找成功，返回指针*/
      p=p->next; /*不相等指针后移*/
   }
   printf("not found\n");  /*没有找到*/
   return NULL; /*返回为空指针*/
}
/*删除结点*/
void delete()
{
   SALARY *p; /*定义临时变量，p指向要删除的结点p*/
   char s[11]; /*要删除结点的编号*/
   clrscr();  /*清屏*/
   printf("please deleted no\n"); /*提示输入信息*/
   scanf("%s",s);   /*输入要删除的职工编号*/
   if((p=find(s))!=NULL) /*调用查找函数，如果找到做下面的处理*/
   {
      if(First==p)    /*如果是第一个结点*/
      {
	 First=p->next;  /*将头指针指向其后继结点*/
	 if(First)    /*如头指针不为空*/
	    First->prior=NULL;  /*将头结点的前驱设为空*/
	 else
	    Last=NULL;   /*否则头为空，尾也为空*/
      }
      else   /*删除的不是第一个结点*/
      {
	 p->prior->next=p->next;  /*p的前驱的后继指向p原来的后继*/
	 if(p!=Last)   /*如果p不是最后个结点*/
	    p->next->prior=p->prior;  /*p后继的前驱指向p原来的前驱*/
	 else
	    Last=p->prior;/*如p是最后一个结点，修改尾指针为p的前驱*/
      }
      free(p);  /*删除结点后，不要忘记释放空间*/
      printf("\n have deleted %s SALARY\n",s);
      printf("Don't forget save\n");
   }
}
/*按姓名查找结点*/
void search()
{
   SALARY *p;  /*移动指针*/
   char s[15];  /*保存姓名*/
   clrscr(); /*清屏*/
   printf("please enter name for search\n"); /*提示输入*/
   scanf("%s",s);  /*输入要查找人的姓名*/
   p=First;  /*移动指针从头指针开始*/
   while(strcmp(p->name,s)&&p!=NULL) /*做比较判断*/
      p=p->next;   /*没找到，指针后移继续查找*/
   if(p==NULL)  /*指针为空，说明没有找到*/
      printf("\nlist no %s SALARY\n",s); /*显示没找到信息*/
   else
   {
      printf("\n\n"); /*换行*/
      print(p);  /*查找成功，调用输出函数输出指针p所指记录*/
   }
}
/*插入结点*/
void insert()
{
   SALARY *p,*info; /* info为新结点，p为新结点的后继*/
   char s[11];  /*保存查找的姓名*/
   float temp; /*临时变量*/
   clrscr(); /*清屏*/
   printf("please enter location  before the no\n"); /*提示输入在哪个结点之前插入*/
   scanf("%s",s);  /*输入指定结点的编号*/
   printf("\nplease new record\n");  /*提示输入记录*/
   clrscr();/*清屏*/
   gotoxy(1,3); /*定位光标，按表格形式输入新结点信息*/
   printf("**************gongziguanli************");
   gotoxy(1,5);
   printf("|------------------------------------|");
   gotoxy(1,6);
   printf("|    no    |      name      |  jbgz  | ");
   gotoxy(1,7);
   printf("|----------|----------------|--------|");
   info=(SALARY *)malloc(sizeof(SALARY)); /*申请空间*/
   if(!info)
   {
      printf("\nout of memory"); /*如没有得到空间，内存溢出*/
      exit(0);  /*退出程序*/
   }
   info->next=NULL;  /*新结点的后继为空*/
   info->prior=NULL; /*新结点的前驱为空*/
   gotoxy(1,8);printf("|");
   gotoxy(12,8);printf("|");
   gotoxy(29,8);printf("|");
   gotoxy(38,8);printf("|");
   gotoxy(2,8);
   inputs(info->no,10);  /*输入新结点的编号，并校验*/
   gotoxy(13,8);
   inputs(info->name,14); /*输入新结点的姓名，并校验*/
   gotoxy(30,8);
   scanf("%f",&temp);   /*输入基本工资*/
   info->jbgz=temp;  /*给基本工资字段赋值*/
   info->koukuan=0; /*扣款初始为0*/
   info->yfgz=0;  /*应发工资初始为0*/
   info->shuijin=0; /*税金初始为0*/
   info->sfgz=0; /*实发工资初始为0*/
   printf("|----------|----------------|--------|");
   p=First;  /*移动指针从头指针开始*/
   while(strcmp(p->no,s)&&p!=NULL) /*查找结点确定插入位置*/
      p=p->next;  /*指针后移，继续查找*/
   if(p==NULL)  /*如p为空*/
      if(p==First) /*如p为头指针，说明链表为空*/
      {
	 First=info;  /*新结点为头指针*/
	 First->prior=NULL; /*头结点的前驱为空*/
	 Last=First;  /*唯一结点，尾指针等于头指针*/
      }
      else  /*新结点插在尾部*/
      {
	 Last->next=info;
	 info->prior=Last;
	 Last=info;  /*尾指针指向新结点*/
      }
   else
      if(p==First)  /*p不为空，但p为头指针，新结点插在第一个结点位置*/
      {
	 info->prior=NULL; /*新结点的前驱为空*/
	 info->next=p; /*新结点的后继为p*/
	 p->prior=info; /*p的前驱是新结点*/
	 First=info; /*修改头指针指向新结点*/
      }
      else /*新结点插入在中间某一个位置p之前*/
      {
	 info->next=p; /*新结点的后继是p*/
	 info->prior= p->prior; /*新结点的前驱是p的前驱*/
	 p->prior->next=info; /*p的前驱的后继是新结点*/
	 p->prior=info; /*p的前驱是新结点*/
      }
   printf("\n\n\n ----have inserted %s SALARY----\n",info->name);
   printf("\n---Don't forget save---\n"); /*提醒调用保存文件存盘*/
}
/*保存文件*/
void save()
{
   FILE *fp;  /*定义指向文件的指针*/
   SALARY *p; /*定义移动指针*/
   char outfile[10]; /*保存输出文件名*/
   clrscr();/*清屏*/
   printf("Enter outfile name,for example c:\\f1\\te.txt:\n");/*提示输入文件名格式信息*/
   scanf("%s",outfile);
   if((fp=fopen(outfile,"wb"))==NULL) /*为输出打开一个二进制文件*/
   {
      printf("can not open file\n");
      return; /*返回*/
   }
   printf("\nSaving file......\n");
   p=First; /*移动指针从头指针开始*/
   while(p!=NULL) /*如p不为空*/
   {
      fwrite(p,sizeof(SALARY),1,fp); /*写入一条记录*/
      p=p->next;  /*指针后移，处理下一条记录*/
   }
   fclose(fp); /*关闭文件*/
   printf("-----save success!!-----\n");/*显示保存成功*/
}
/*读文件*/
void load()
{
   SALARY *p,*q=NULL; /*定义记录指针变量*/
   FILE *fp; /* 定义指向文件的指针*/
   char infile[10]; /*保存文件名*/
   clrscr();/*清屏*/
   printf("Enter infile name,for example c:\\f1\\te.txt:\n");/*输入文件名格式*/
   scanf("%s",infile); /*输入文件名*/
   if((fp=fopen(infile,"rb"))==NULL) /*打开一个二进制文件，为读方式*/
   {
      printf("can not open file\n"); /*如不能打开，则结束程序*/
      return; /*返回*/
   }
   while(First) /*当表不为空时，清空链表*/
   {
      p=First; /*从头指针开始*/
      First=First->next; /*删除头结点*/
      free(p); /*释放空间*/
   }
   printf("\n -----Loading file!-----\n"); /*显示正在读文件*/
   First=(SALARY *)malloc(sizeof(SALARY)); /*为头指针申请空间*/
   if(!First) /*如果没有得到空间 显示内存溢出*/
   {
      printf("out of memory!\n");
      return;  /*返回*/
   }
   p=First; /*将头指针赋值给p*/
   while(!feof(fp)) /*当文件不空时*/
   {
      if(1!=fread(p,sizeof(SALARY),1,fp))break; /*将记录读到p所指的结点*/
      p->next=(SALARY *)malloc(sizeof(SALARY));/*为p的后继申请空间*/
      if(!p->next)  /*如果没有得到空间显示内存溢出*/
      {
	 printf("out of memory!\n");
	 return; /*返回*/
      }
      p->prior=q; /*得到空间，则链接指针关系p的前驱为q*/
      q=p;  /*保存p做为新的p结点的前驱*/
      p=p->next;  /*p指针后移*/
   }
   q->next=NULL; /*q的后继为空*/
   Last=q;  /*尾指针为q*/
   First->prior=NULL; /*头指针的前驱为空*/
   fclose(fp); /*关闭文件*/
   printf("---You have success read data from file!!!---\n"); /*显示成功读入记录*/
}
/*追加记录*/
void append()
{
   FILE *fp;   /*定义指向文件的指针*/
   SALARY *info; /*指向追加信息的指针*/
   char infile[10]; /*保存文件名*/
   float temp; /*临时变量*/
   clrscr(); /*清屏*/
   gotoxy(1,3);
   printf("**************gongziguanli************");
   gotoxy(1,5);
   printf("|------------------------------------|");
   gotoxy(1,6);
   printf("|    no    |      name      |  jbgz  | ");
   gotoxy(1,7);
   printf("|----------|----------------|--------|");
   info=(SALARY *)malloc(sizeof(SALARY)); /*申请追加信息的空间*/
   if(!info)
   {
      printf("\nout of memory"); /*没有空间，内存溢出*/
      exit(0);  /*结束程序*/
   }
   info->next=NULL;  /*新信息的后继为空*/
   info->prior=NULL; /*新信息的前驱为空*/
   gotoxy(1,8);printf("|");
   gotoxy(12,8);printf("|");
   gotoxy(29,8);printf("|");
   gotoxy(38,8);printf("|");
   gotoxy(2,8);
   inputs(info->no,10); /*输入编号并校验*/
   gotoxy(13,8);
   inputs(info->name,14); /*输入姓名并校验*/
   gotoxy(30,8);
   scanf("%f",&temp); /*输入基本工资*/
   info->jbgz=temp;
   info->koukuan=0; /*扣款为0*/
   info->yfgz=0; /*应发工资为0*/
   info->shuijin=0; /*税金为0*/
   info->sfgz=0; /*实发工资为0*/
   printf("|----------|----------------|--------|");
   printf("\n\n\nEnter infile name,for example c:\\f1\\te.txt:\n"); /*提示输入文件名*/
   scanf("%s",infile);
   if((fp=fopen(infile,"ab"))==NULL) /*打开文件*/
   {
      printf("can not open file\n");
      return;  /*返回*/
   }
   printf("\n -----Appending record!-----\n");
   if(1!=fwrite(info,sizeof(SALARY),1,fp)) /*向文件中追加记录*/
   {
      printf("-----file write error!-----\n"); /*写函数返回值不为1，说明写入错误*/
      return;  /*返回*/
   }
   printf("-----append  sucess!!----\n");/*显示追加成功*/
   fclose(fp);/*关闭文件*/
}
/*文件复制*/
void copy()
{
   char outfile[10],infile[10];  /*保存源文件名和目标文件名*/
   FILE *sfp,*tfp;  /*定义指向源文件和目标文件的指针*/
   SALARY *p=NULL;  /*定义临时指针，暂存读出的记录*/
   clrscr();
   printf("Enter infile name,for example c:\\f1\\te.txt:\n");
   scanf("%s",infile); /*输入源文件名*/
   if((sfp=fopen(infile,"rb"))==NULL) /*二进制读方式打开源文件*/
   {
      printf("can not open input file\n"); /*显示不能打开文件信息*/
      return; /*返回*/
   }
   printf("Enter outfile name,for example c:\\f1\\te.txt:\n");  /*提示输入目标文件名*/
   scanf("%s",outfile); /*输入目标文件名*/
   if((tfp=fopen(outfile,"wb"))==NULL) /*二进制写方式打开目标文件*/
   {
      printf("can not open output file \n");
      return;
   }
   while(!feof(sfp)) /*读文件直到文件尾*/
   {
      if(1!=fread(p,sizeof(SALARY),1,sfp))
	 break;/*块读*/
      fwrite(p,sizeof(SALARY),1,tfp); /*块写*/
   }
   fclose(sfp); /*关闭源文件*/
   fclose(tfp); /*关闭目标文件*/
   printf("you have success copy  file!!!\n");/*显示成功拷贝*/
}
/*排序*/
void sort()
{
   SALARY *p0,*p00,*p1,*p11,*p2; /*定义临时指针*/
   int i=1;  /*统计当前排第几个结点*/
   clrscr(); /*清屏*/
   printf("\n\n start sort....\n"); /*开始排序*/
   p1=p11=p2=First;
   while(p1!=NULL) /*当p1不为空时*/
   {
      p0=p1;    /*待比较结点从p1开始*/
      p2=p1;    /*p2从当前待排序位置开始*/
      while(p2->next!=NULL) /*p2的后继不为空时*/
      {
	 if(p0->sfgz>p2->next->sfgz)  /*找当前最小结点*/
	 {
	    p00=p2;   /*找到当前最小结点记录其前驱指针*/
	    p0=p2->next;  /*记录当前最小值结点的指针p0*/
	 }
	 p2=p2->next;  /*指针后移*/
      }
      Last=p0;  /*当前最小结点为尾结点*/
      if(p1!=p0)  /*判断最小结点是否是当前排序位置，不是则作指针修改*/
      {
	 p00->next=p0->next;  /*将p00的后继指向p0的后继*/
	 p0->next->prior=p00; /*p0的后继的前驱为p00*/
	 p0->next=p1;  /*p0的后继指向下个排序位置p1*/
	 p1->prior=p0; /*p1的前驱为p0*/
	 if(i==1)
	 {
	    First=p0;   /*作头指针*/
	    First->prior=NULL; /*头指针的前驱为空*/
	 }
	 else   /*不是第一个结点*/
	 {
	    p11->next=p0;   /*p11的后继指向p0*/
	    p0->prior=p11; /*p0的前驱指向p11*/
	 }
	 p11=p1=p0; /*因p0是排好序的最后一个结点，p1是下一个将要排序的位置，所以将p0赋值给p1和p11*/
      }
      i++;  /*排好序的结点数加1*/
      p1=p1->next; /*p1后移指向下一个待排序位置*/
   }
   printf("sort sucess!!!\n");  /*输出排序成功*/
}
/*索引*/
void index()
{
   SALARY *p,*q,*t,*h1; /*定义临时指针*/
   clrscr();/*清屏*/
   printf("\n\n start index.....\n\n"); /*显示开始索引*/
   h1=First->next; /*将原表的头指针所指的下一个结点作头指针*/
   First->next=NULL; /*第一个结点为新表的头结点*/
   Last=First; /*尾指针等于头指针*/
   while(h1!=NULL) /*当原表不为空时，进行排序*/
   {
      t=h1; /*取原表的头结点*/
      h1=h1->next; /*原表头结点指针后移*/
      p=First; /*设定移动指针p，从头指针开始*/
      q=First; /*设定移动指针q做为p的前驱，初值为头指针*/
      while(strcmp(t->no,p->no)>0&&p!=NULL) /*作编号比较*/
      {
	 q=p; /*待排序点值大，应往后插，所以新表指针后移*/
	 p=p->next;
      }
      if(p==q)  /*p==q，说明待排序点值小，应排在首位*/
      {
	 t->next=p; /*待排序点的后继为p*/
	 p->prior=t; /*p的前驱结点指向待排序结点*/
	 First=t; /*新头结点为待排序点*/
	 First->prior=NULL; /*头指针的前驱为空*/
      }
      else /*待排序点应插入在中间某个位置q和p之间，如p为空则是尾部*/
      {
	 t->next=p; /*t的后继是p*/
	 t->prior=q; /*t的前驱是q*/
	 q->next=t; /*q的后继是t*/
	 if(p==NULL) /*判p是否为空*/
	    Last=t;   /*将t赋值为尾指针*/
	 else
	    p->prior=t; /*否则将t赋值给p的前驱*/
      }
   }
   printf("index sucess!!!\n");/*索引排序成功*/
}
/*分类合计*/
void total()
{
   struct t1  /*定义结构体保存合计数据*/
   {
      char u[7];  /*部门*/
      float s;   /*合计实发工资*/
   }unit[20];   /*定义一个单位部门数不超过20个*/
   SALARY *p,*q;  /*定义临时指针变量*/
   char sno[7],qno[7],*ptr; /*保存部门号*/
   float s1;  /*保存合计值*/
   int i,j;  /*控制变量*/
   p=First;  /*从指针开始*/
   i=0;  /*统计部门数*/
   clrscr();/*清屏*/
   while(p!=NULL) /*当p不为空时做下面的处理*/
   {
      memcpy(sno,p->no,6); /*从编号中取出部门号*/
      sno[6]='\0'; /*做字符串结束标记*/
      q=p->next; /*将指针指向待比较的记录*/
      s1=p->sfgz;  /*当前部门的实发工资初值作为该部门合计值的初值*/
      while(q!=NULL) /*内循环开始*/
      {
	 memcpy(qno,q->no,6); /*读部门号*/
	 qno[6]='\0'; /*做字符串结束标记*/
	 if(strcmp(qno,sno)==0) /*比较部门号*/
	 {
	    s1+=q->sfgz; /*累加实发工资*/
	    q=q->next; /*指针指向下一条记录*/
	 }
	 else
	    break; /*不是一个部门的结束本次内循环*/
      }
      strcpy(unit[i].u,sno); /*将部门号写入部门结构体数组中*/
      unit[i].s=s1;    /*将实发工资写入部门结构体数组中*/
      i++;       /*部门数累加*/
      if(q==NULL)  /*如果q为空，跳出循环*/
	 break;
      else
	 p=q;   /*否则，将q赋值给p*/
   }
   printf("\n\n**************Total table**************\n");/*输出合计标题*/
   printf("\n--no-------unit------------total---------\n"); /*输出序号、部门、合计字段标题*/
   for(j=0;j<i;j++)
      printf("   %d        %s             %10.2f\n",j,unit[j].u,unit[j].s); /*输出合计值*/
   printf("----------------------------------------\n");
}
