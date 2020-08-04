


/*学生成绩*/
#include<STDIO.H>
#include<CONIO.H>
#include<STDLIB.H>
#include<BIOS.H>
#include<IO.H>
#define CR 13
#define ESC 27
#define UP 72
#define DOWN 80
#include<string.h>
#include<stdio.h>
#define CRNUM 200
#define STNUM 40
#define TERMS 6
typedef struct message
{
	int num,grade,mcode;
	struct message *ll,*r1,*next;
}MPOINT;
typedef struct coursechain
{
	int ccode,cterm;
	MPOINT *mp;
	struct coursechain *next;
}CPOINT;
typedef struct directory
{
	int dclass;
	CPOINT *cp;
	struct student
	{
		char dname[20];
		MPOINT *mp[TERMS];
	}stu[STNUM];
	struct directory *next;
}DPOINT;
char coc[CRNUM][20];
DPOINT *dphead;
char nulnam='\0';
DPOINT *searchdp();
CPOINT *searchcp();
void registerstudent();
MPOINT *insbtree();
void markregister();
void modify();
void printreport();
void inorder();
void countaverage();
void segmentprint();
void region();
int searchcode();
long int key;
long int f1=0;
long int f2=0;
int true=1;
int false=0; /* */
union inkey
{
	char ch[2];
	int sc;
}buff; /* */
int arrow;
int s;
void main()
{
	dphead=NULL;
	for(arrow=0;arrow<CRNUM;arrow++) coc[arrow][0]=nulnam;
	do{
		char floag;
		arrow=2;
		false=0;
		do{
			textbackground(WHITE);
			textcolor(RED);
			clrscr();
			printf("\n\n\n\n\n\n\t\t\t山 东 理 工 大 学\n\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\t\t\t\t计科四班\n\n\n");
			printf("\t\t注册学生\n");
			printf("\t\t登录学生成绩\n");
			printf("\t\t修改学生成绩\n");
			printf("\t\t打印某班学生单科成绩\n");
			printf("\t\t学期某班各科平均成绩\n");
			printf("\t\t打印某科成处在指定分数段内学生名单\n");
			printf("\t\t退出系统\n\n\n\n");
			printf("\t\t\t\t 制作人:世外高人\n");
			printf("\t\t\t\t 制作日期:二００四年六月二十五号\n");
			textcolor(BLUE);
			switch(arrow)
			{
			case 1:gotoxy(17,13); cprintf("注册学生"); break;
			case 2:gotoxy(17,14); cprintf("登录学生成绩"); break;
			case 3:gotoxy(17,15); cprintf("修改学生成绩"); break;
			case 4:gotoxy(17,16); cprintf("打印某班学生单科成绩"); break;
			case 5:gotoxy(17,17); cprintf("学期某班各科平均成绩"); break;
			case 6:gotoxy(17,18); cprintf("打印某科成绩处在指定分数段内学生名单"); break;
			case 7:gotoxy(17,19); cprintf("退出系统"); break;
			}
loop: key=bioskey(0);
      f1=key&255;
      f2=(key&65280)/256;
      if(f1!=0)
      {
		  switch(f1)
		  {
		  case CR:false=1; true=1; break;
		  case ESC:false=1; true=0; break;
		  default:goto loop;
		  }
      }
      else
      {
		  switch(f2)
		  {
		  case DOWN:arrow++; break;
		  case UP:arrow--; break;
		  default:goto loop;
		  }
		  if(arrow>7) arrow=1;
		  if(arrow<1) arrow=7;
      }
		}while(false!=1);
		if(true==1)
			switch(arrow)
		{
      case 1:registerstudent(); break;
      case 2:markregister(); break;
      case 3:modify(); break;
      case 4:printreport(); break;
      case 5:countaverage(); break;
      case 6:segmentprint(); break;
      case 7:true=1; break;
		}
	}while(true!=1);
	clrscr();
	textbackground(BLACK);
	textcolor(WHITE);
	clrscr();
}
/*班级登记*/
DPOINT *searchdp(int class)

{
	DPOINT *dp;
	dp=dphead;
	while((dp!=NULL)&&(dp->dclass!=class)) dp=dp->next;
	return(dp);
}
CPOINT *searchcp(DPOINT *dp,int term,int *code)
{
	int number;
	char cournam[20];
	CPOINT *cp;
	*code=-1;
	gotoxy(5,22);
	cprintf(" ");
	gotoxy(5,22);
	cprintf("请输入课程名称:");
	gets(cournam);
	strupr(cournam);
	if(cournam);
	{
		*code=searchcode(cournam);
		cp=dp->cp;
		while(cp!=NULL&&!(cp->ccode==*code&&cp->cterm==term)) cp=cp->next;
		return(cp);
	}
}
void registerstudent()
{
	int class,num,i,j,h=1;
	char name[20]={"TOM"};
	DPOINT *dp;
	class=1;
	do{
		/*班级*/ clrscr();
		gotoxy(5,24);
		textbackground(WHITE);
		textcolor(BLUE);
		cprintf(" ");
		gotoxy(5,24);
		cprintf("请录入信息! 模块退出标志:班级号小于或等于0");
		gotoxy(5,3);
		textcolor(BLUE);
		cprintf("第%d班",class);
		gotoxy(15,10);
		cprintf("班级(数字):");
		gotoxy(15,14);
		cprintf("学号(数字):");
		gotoxy(15,18);
		cprintf("姓名(英文):");
		gotoxy(30,10);
		cprintf(" ");
		gotoxy(30,10);
		scanf("%d",&class);
		getchar();
		dp=searchdp(class);
		if(dp!=NULL)
		{
			if(dp->dclass==class);
			gotoxy(5,22);
			cprintf("%d班已注册!",h);
		}
		else
		{
			dp=(DPOINT*)malloc(sizeof(DPOINT));
			dp->dclass=class;
			dp->cp=NULL;
			for(i=0;i<STNUM;i++)
			{
				dp->stu[i].dname[0]=nulnam;
				for(j=0;j<TERMS;j++) dp->stu[i].mp[j]=NULL;
			}
			dp->next=dphead;
			dphead=dp;
			gotoxy(5,24);
			cprintf(" ");
			gotoxy(5,24);
			cprintf("请输入信息! 学生结束标志:学号不大于0");
			do{
				gotoxy(30,14);
				cprintf(" ");
				gotoxy(30,14);
				scanf("%d",&num);
				getchar();
				if(num>0&&num<=STNUM)
				{
					gotoxy(30,18);
					cprintf(" ");
					gotoxy(30,18);
					gets(name);
					strupr(name);
					strcpy(dp->stu[num-1].dname,name);
				}
				if(num>=STNUM)
				{
					gotoxy(5,22);
					cprintf(" ");
					gotoxy(5,22);
					cprintf("输入号%3d不正确!",num);
				}
			}while(num>0);
		}
		if(h!=class) h=class;
	}while(class>0);
}
/*登记函数*/
MPOINT *insbtree(MPOINT* mp,MPOINT* mph)

{
	if(mph==NULL) mph=mp;
	else
		if(mp->grade>=mph->grade) mph->ll=insbtree(mp,mph->ll);
		else mph->r1=insbtree(mp,mph->r1);
		return(mph);
}
int searchcode(cname)
char cname[];
{
	int i,found,code;
	found=0;
	i=(cname[0]*(cname[2]+cname[4]))%CRNUM;
	code=i;
	i=(i-1)%CRNUM;
	while(!(found||code==i))
	{
		if(coc[code][0]==nulnam)
		{
			strcpy(coc[code],cname);
			found=1;
		}
		else
			if(!strcmp(coc[code],cname)) found=1;
			else code=(code+1)%CRNUM;
	}
	return(code);
}
void markregister()
{
	int class,term,number,code,mark;
	char cournam[20];
	DPOINT *dp;
	CPOINT *cp;
	MPOINT *mp;
	clrscr();
	textbackground(WHITE);
	textcolor(BLUE);
	gotoxy(15,8);
	cprintf("班级号:");
	gotoxy(15,11);
	cprintf("学期号:");
	gotoxy(15,14);
	cprintf("学号:");
	gotoxy(15,17);
	cprintf("成绩:");
	gotoxy(5,22);
	cprintf(" ");
	gotoxy(5,22);
	cprintf("请输入班级号及几学期(1~%ld):",TERMS);
	gotoxy(25,8);
	cprintf(" ");
	gotoxy(25,8);
	scanf("%d",&class);
	if(class!=0)
	{
		gotoxy(25,11);
		cprintf(" ");
		gotoxy(25,11);
		scanf("%d",&term);
		getchar();
	}
	dp=searchdp(class);
	if(dp==NULL)
	{
		gotoxy(5,22);
		cprintf(" ");
		gotoxy(5,22);
		cprintf("你必须先进行\"1.班级,学生登记注册\"!");
	}
	else
	{
		cp=searchcp(dp,term,&code);
		if(code==-1) return;
		if(cp!=NULL)
		{
			gotoxy(5,22);
			cprintf("第%ld学期的课程已经登记过部分学生!",cp->cterm,coc[code]);
		}
		else
		{
			cp=(CPOINT *)malloc(sizeof(CPOINT));
			cp->ccode=code;
			cp->cterm=term;
			cp->mp=NULL;
			cp->next=dp->cp;
			dp->cp=cp;
		}
		do{
			gotoxy(5,24);
			cprintf("请输入学生的学号及成绩(学号0表示成绩登记结束):");
			gotoxy(25,14);
			cprintf(" ");
			gotoxy(25,14);
			scanf("%d",&number);
			if(number!=0)
			{
				gotoxy(25,17);
				cprintf(" ");
				gotoxy(25,17);
				scanf("%d",&mark);
				getchar();
			}
			if((number>0)&&(number<=STNUM))
			{
				if(dp->stu[number-1].dname[0]!=nulnam)
				{
					mp=dp->stu[number-1].mp[term=1];
					while((mp!=NULL)&&(mp->mcode!=code)) mp=mp->next;
					if(mp==NULL)
					{
						mp=(MPOINT *)malloc(sizeof(MPOINT));
						mp->num=number;
						mp->grade=mark;
						mp->mcode=code;
						mp->ll=mp->r1=NULL;
						mp->next=dp->stu[number-1].mp[term-1];
						cp->mp=insbtree(mp,cp->mp);
						dp->stu[number-1].mp[term-1]=mp;
					}
					else
					{
						gotoxy(5,22);
						cprintf("你必须使用\"3.学生各科成绩修改\"修改已登记的成绩!");
						return;
					}
				}
				else if(number!=0)
				{
					gotoxy(5,22);
					cprintf("第%d号学生没有登记注册!",number);
				}
			}
		}while(number!=0);
	}
      }
      void modify()
      {
		  int class,term,number,code,mark;
		  char cournam[20],t;
		  DPOINT *dp;
		  CPOINT *cp;
		  MPOINT *mp;
		  clrscr();
		  textbackground(WHITE);
		  textcolor(BLUE);
		  clrscr();
		  gotoxy(15,8);
		  cprintf("班级号:");
		  gotoxy(15,11);
		  cprintf("学期号:");
		  gotoxy(15,14);
		  cprintf("学号:");
		  gotoxy(15,17);
		  cprintf("成绩:");
		  gotoxy(5,22);
		  cprintf("请输入班级号及几学期(1~%ld):",TERMS);
		  gotoxy(25,8);
		  cprintf(" ");
		  gotoxy(25,8);
		  cprintf(" ");
		  gotoxy(25,8);
		  scanf("%d",&class);
		  if(class!=0)
		  {
			  gotoxy(25,11);
			  cprintf(" ");
			  gotoxy(25,11);
			  scanf("%d",&term);
			  getchar();
		  }
		  if(term<1||term>TERMS)
		  {
			  gotoxy(5,22);
			  cprintf("学期号%d不正确!",term);
		  }
		  dp=searchdp(class);
		  if(dp==NULL)
		  {
			  gotoxy(5,22);
			  cprintf("%4d班级没有登记注册!",&class);
			  return;
		  }
		  else
		  {
			  gotoxy(5,22);
			  cprintf(" ");
			  gotoxy(5,22);
			  cprintf("请输入课程名称:");
			  gets(cournam);
			  strupr(cournam);
			  if(cournam[0]!=nulnam) code=searchcode(cournam);
			  else return;
		  }
		  do{
			  gotoxy(5,24);
			  cprintf("请输入学生的学号及成绩(学号0表示结束修改):");
			  gotoxy(25,14);
			  cprintf(" ");
			  gotoxy(25,14);
			  scanf("%d",&number);
			  if(number!=0)
			  {
				  gotoxy(25,17);
				  cprintf(" ");
				  gotoxy(25,17);
				  scanf("%d",&mark);}
			  getchar();
			  if(number>0&&number<=STNUM)
			  {
				  mp=dp->stu[number-1].mp[term-1];
				  if(dp->stu[number-1].dname[0]!=nulnam)
				  {
					  while(mp!=NULL&&mp->mcode!=code) mp=mp->next;
					  if(mp!=NULL)
					  {
						  gotoxy(5,22);
						  cprintf("是这一位学生吗?");
						  gotoxy(5,24);
						  cprintf(" ");
						  gotoxy(5,24);
						  cprintf("学号:%2d 姓名:%-16s 原成绩%3d课程代号:%5d(Y/N)",mp->num,dp->stu[number-1].dname,mp->grade,mp->mcode);
							  scanf("%c",&t);
						  getchar();
						  while(t!='Y'&&t!='y'&&t!='N'&&t!='n')
						  {
							  gotoxy(5,24);
							  cprintf(" ");
							  gotoxy(5,24);
							  cprintf(" ");
							  gotoxy(5,24);
							  cprintf("请输入Y键或N键:");
							  scanf("%c",&t);
							  getchar();
						  }
						  if(t=='y'||t=='Y') mp->grade=mark;
						  else
						  {
							  gotoxy(5,24);
							  cprintf("学号%-3d的学生成绩没有修改! ",number);
						  }
					  }
					  else
					  {
						  gotoxy(5,24);
						  cprintf("学号%-3d的学生课程成绩未登记! ",number);
					  }
				  }
				  else
				  {
					  gotoxy(5,24);
					  cprintf("第%d的学生没有登记注册! ",number);
				  }
			  }
			  else if(number!=0)
			  {
				  gotoxy(5,24);
				  cprintf(" ");
				  gotoxy(5,24);
				  cprintf("输入学号%3d不正确!",number);
			  }
		  }while(number!=0);
      }
      void countaverage()
      {
		  int class,term,i,n,code;
		  float aver;
		  DPOINT *dp;
		  MPOINT *mp;
		  clrscr();
		  gotoxy(5,24);
		  cprintf("请输入班级号及第几学期(1--%ld):",TERMS);
		  gotoxy(15,10);
		  cprintf("班级");
		  gotoxy(15,15);
		  cprintf("学期");
		  gotoxy(25,10);
		  cprintf(" ");
		  gotoxy(25,15);
		  scanf("%d",&class);
		  if(class!=0)
		  {
			  gotoxy(25,15);
			  cprintf(" ");
			  gotoxy(25,15);
			  scanf("%d",&term);
			  getchar();
		  }
		  dp=searchdp(class);
		  if(dp==NULL)
		  {
			  gotoxy(5,24);
			  cprintf("%4d班级没有登记注册!",class);
		  }
		  else
		  {
			  if(term<1||term>TERMS)
			  {
				  gotoxy(5,24);
				  cprintf("学期%ld不正确!",term);
				  return;
			  }
			  gotoxy(5,5);
			  cprintf("第%ld学期各科平均成绩表\n",term);
			  cprintf("============================================================\n");
			  cprintf("\n");
			  cprintf(" 学号 姓名 平均成绩\n");
			  cprintf("\n");
			  for(i=0;i<STNUM;i++)
				  if(dp->stu[i].dname[0]!=nulnam)
				  {
					  n=0;
					  aver=0;
					  mp=dp->stu[i].mp[term-1];
					  while(mp!=NULL)
					  {
						  aver=aver+mp->grade;
						  n+=1;
						  mp=mp->next;
					  }
					  if(n!=0) aver=aver/n;
					  {
						  gotoxy(5,24);
						  cprintf("%19d%20s%23.1f",i+1,dp->stu[i].dname,aver);
					  }
				  }
				  gotoxy(5,24);
				  cprintf("按回车键继续");
				  getchar();
		  }
      }
      void printreport()
      {
		  int class,term,code,i;
		  float aver;
		  int count[10];
		  char cournam[20];
		  DPOINT *dp;
		  CPOINT *cp;
		  MPOINT *mp;
		  clrscr();
		  clrscr();
		  textbackground(WHITE);
		  textcolor(BLUE);
		  clrscr();
		  gotoxy(5,24);
		  cprintf("请输入班级号及第几学期(1--%d):",TERMS);
		  gotoxy(15,15);
		  cprintf("学期");
		  gotoxy(15,10);
		  cprintf("班级");
		  gotoxy(25,10);
		  cprintf(" ");
		  gotoxy(25,10);
		  scanf("%d",&class);
		  if(class!=0)
		  {
			  gotoxy(25,15);
			  cprintf(" ");
			  gotoxy(25,15);
			  scanf("%d",&term);}
		  getchar();
		  if(term<1||term>TERMS)
		  {
			  gotoxy(5,24);
			  cprintf("学期号%ld不正确!",term);
			  return;
		  }
		  dp=searchdp(class);
		  if(dp==NULL)
		  {
			  gotoxy(5,24);
			  cprintf("%4d班级没有登记注册!",class);
		  }
		  else
		  {
			  cp=searchcp(dp,term,&code);
			  if(code==-1) return;
			  if(code==-1) return;
			  if(cp==NULL)
			  {
				  gotoxy(5,24);
				  cprintf("%第ld学期%s课程没有登记!",term,coc[code]);
			  }
			  else
			  {
				  for(i=0;i<=9;i++) count[i]=0;
				  aver=0;
				  clrscr();
				  gotoxy(5,5);
				  cprintf(" 单科成绩\n");
				  gotoxy(5,6);
				  cprintf("=====================================================\n");
				  gotoxy(5,7);
				  cprintf("课程名称:%s\n",coc[code]);
				  cprintf("\n");
				  gotoxy(5,8);
				  inorder(count,cp->mp,dp);
				  cprintf("按回车键继续\n");
				  getchar();
				  aver=count[1];
				  if(count[0]!=0) aver=aver/count[0];
				  gotoxy(15,10);
				  cprintf("班级平均成绩:%13.1f\n",aver);
				  cprintf("等级 0-59 60-69 70-79 80-89 90-100\n");
				  cprintf("人数");
				  for(i=5;i<=9;i++)
					  cprintf("%7d",count[i]);
				  cprintf("\n");
			  }
		  }
      }
      void inorder(int count[],MPOINT *t,DPOINT *dp)
	  {
		  int m,n=0,aver=0,top;
		  int uu;
		  MPOINT *s[STNUM];
		  top=-1;
		  uu=8;
		  while(t!=NULL||top!=-1)
		  {
			  while(t!=NULL)
			  {
				  s[++top]=t;
				  t=t->ll;
			  }
			  if(top>=0)
			  {
				  t=s[top--];
				  m=(t->grade)/10;
				  aver+=t->grade;
				  switch(m)
				  {
				  case 0:
				  case 1:
				  case 2:
				  case 3:
				  case 4:
				  case 5:m=5;break;
				  case 6:m=6;break;
				  case 7:m=7;break;
				  case 8:m=8;break;
				  case 9:
				  case 10:m=9;break;
				  };
				  n++;
				  count[m]++;
			  }
			  m=t->num;
			  uu++;
			  gotoxy(5,uu);
			  cprintf("%7d%20s%4d\n",m,dp->stu[m-1].dname,t->grade);
			  t=t->r1;
		  }
		  count[0]=n;
		  count[1]=aver;
      }
      void segmentprint()
      {
		  int class,term,code,min,max;
		  char cournam[20];
		  DPOINT *dp;
		  CPOINT *cp;
		  MPOINT *mp;
		  clrscr();
		  textbackground(WHITE);
		  textcolor(BLUE);
		  clrscr();
		  gotoxy(5,24);
		  cprintf("请输入班级号几学期(1--%ld):",TERMS);
		  gotoxy(15,10);
		  cprintf("班级");
		  gotoxy(15,15);
		  cprintf("学期");
		  gotoxy(25,10);
		  cprintf(" ");
		  gotoxy(25,10);
		  scanf("%d",&class);
		  if(class!=0)
		  {
			  gotoxy(25,15);
			  cprintf(" ");
			  gotoxy(25,15);
			  scanf("%d",&term);
		  }
		  getchar();
		  if(term<1||term>TERMS)
		  {
			  gotoxy(5,24);
			  cprintf("学期号%ld不正确!",term);
			  return;
		  }
		  dp=searchdp(class);
		  if(dp==NULL)
		  {
			  gotoxy(5,24);
			  cprintf("%4d班级没有登记注册1",class);
		  }
		  else
		  {
			  cp=searchcp(dp,term,&code);
			  if(code==-1) return;
			  if(cp==NULL)
			  {
				  gotoxy(5,24);
				  cprintf("第%ld学期%s课程没有登记!",term,coc[code]);
			  }
			  else
			  {
				  gotoxy(15,20);
				  cprintf("请输入分数段的下界和上界:\n");
				  gotoxy(25,20);
				  cprintf(" ");
				  gotoxy(25,20);
				  scanf("%d%d",&min,&max);
				  getchar();
				  gotoxy(5,5);
				  cprintf("分数段报告表\n");
				  gotoxy(5,6);
				  cprintf("======================================================");
				  cprintf("\n");
				  gotoxy(5,8);
				  cprintf("学号 姓名 分数\n");
				  cprintf("\n");
				  region(cp->mp,dp,class,min,max);
				  cprintf("按回车键继续\n");
				  getchar();
			  }
		  }
      }
      void region(MPOINT *mp,DPOINT *dp,int class,int min, int max)
	  {
		  int m,mark;
		  int o=8;
		  if(mp!=NULL)
		  {
			  mark=mp->grade;
			  if(mark<=max) region(mp->ll,dp,class,min,max);
			  if(mark<=max&&mark>=min)
			  {
				  m=mp->num;
				  o++;
				  cprintf("%8d%13s%14d\n",m,dp->stu[m-1].dname,mark);
			  }
			  if(mark>min) region(mp->r1,dp,class,min,max);
		  }
      }
		  
		  