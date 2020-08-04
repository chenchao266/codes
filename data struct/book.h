/*13. book.c源程序*/
#include <dos.h>
#include <bios.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LEFT 0x4b00  /*左*/
#define RIGHT 0x4d00 /*右*/
#define DOWN 0x5000 /*下*/
#define UP 0x4800 /*上*/
#define SPACE 0x3920 /*空格*/
#define ESC 0x011b /* ESC键*/
#define ENTER 0x1c0d /*回车键*/
#define Backspace 0xe08 /*擦除键*/
#define ALT_B 12288 /*组合键ALT_B */
#define ALT_M 12800/*组合键ALT_M */
#define ALT_H 8960/*组合键ALT_H */
int key;/*按键变量*/
int textx,texty;/*光标坐标,x行,y列*/
struct menustruct/*菜单用的结构体*/
{
   char name[10];/*主菜单名*/
   char str[10][20];/*选项*/
   int n;/*选项数*/
}ml[3];/*使用了3个，可根据需要增删*/
typedef struct BookList/*书的结构体*/
{
   char num[20];/*图书编号*/
   char name[20];/*书名*/
   int price;/*书的价格*/
   char person[20];/*借阅人*/
   int yes;/*判断书是否存在或者已经借出,1存在,0借出*/
   struct BookList *next;
}Book;
typedef struct MemberList/*会员的结构体*/
{
   char name[20];/*会员的姓名*/
   char sex[2];/*会员的性别*/
   int age;/*会员的年龄*/
   struct MemberList *next;
}Member;
char save[4096];/*保存文本区域空间*/
/*char sav1[4096];*/
char c[4096];/*清屏专用空间*/
int i,j;/*常用变量*/
void Menu();/*初始化界面*/
void Selectitem();/*定义菜单*/
void DrawSelectitem();/*显示主菜单*/
void BlackText(int x,int y,char *z);/*选中菜单*/
void RedText(int x,int y,char *z);/*正常菜单*/
void Run();/*具体操作过程*/
void DrawMl(int n);/*显示下拉菜单*/
void MoveMl(int n,int x);/*菜单选项的控制*/
void Enter(int m,int n);/*菜单选项的具体功能*/
void BookAdd();/*添加图书*/
void BookConsult();/*图书查询*/
void BookDel();/*删除图书资料*/
void BookBorrow();/*借书*/
void BookReturn(); /*还书*/
void MemberAdd(); /*增加会员*/
void MemberConsult();/*查询会员*/
void MemberDel(); /*删除会员*/
void MemberBook(); /*查询会员借书信息*/
void Help(); /*帮助*/
void Ver(); /*版本信息*/
void ClrScr();/*自定义清屏函数*/
void DrawFrame(int left,int up,int right,int down,int textcolor,int backgroundcolor);/*画边框*/
/***主函数****/
void main(void)
{
   Menu();/*初始化界面*/
   Run();/*具体操作过程*/
}
/*初始化界面*/
void Menu()
{
   system("cls"); /*调用系统的清屏命令*/
   textbackground(BLUE);/*将背景设置为蓝色*/
   window(1,1,25,80);
   clrscr();
   textx=3;/*光标初始化位置*/
   texty=2;
   gotoxy(1,2);
   printf("%c",218);/*画左上角*/
   for(i=0;i<78;i++)
   printf("%c",196); /*画水平直线*/
   printf("%c",191);/*画右上角*/
   for(i=3;i<=23;i++)
   {
      gotoxy(1,i);
      printf("%c",179); /*画垂直线*/
      gotoxy(80,i);
      printf("%c",179);
   }
   printf("%c",192); /*画左下角*/
   for(i=0;i<78;i++)
      printf("%c",196);
   printf("%c",217); /*画右下角*/
   gotoxy(1,1);
   textcolor(7); /*设置灰色*/
   for(i=0;i<80;i++)
      cprintf("%c",219);/*用符号实现画主菜单的灰色背景区*/
   Selectitem();  /*调用选项函数*/
   DrawSelectitem(); /*画选项*/
   gettext(2,3,78,23,c); /*保存当前文本区域*/
}
/*定义菜单*/
void Selectitem()
{
   strcpy(ml[0].name,"Book");/*下面的具体选项补空格是为了各菜单黑色背景相同*/
   strcpy(ml[0].str[0],"Add                ");/*添加图书*/
   strcpy(ml[0].str[1],"Consult            ");/*查询图书*/
   strcpy(ml[0].str[2],"Del                ");/*删除图书资料*/
   strcpy(ml[0].str[3],"Borrow             ");/*借书*/
   strcpy(ml[0].str[4],"Return             ");/*还书*/
   strcpy(ml[0].str[5],"Exit               ");/*退出系统*/
   ml[0].n=6; /*保存菜单的项数*/
   strcpy(ml[1].name,"Member");
   strcpy(ml[1].str[0],"Add                ");/*添加新会员*/
   strcpy(ml[1].str[1],"Consult            ");/*查询会员资料*/
   strcpy(ml[1].str[2],"Del                ");/*删除会员资料*/
   strcpy(ml[1].str[3],"Member&book        ");
   ml[1].n=4;
   strcpy(ml[2].name,"Help");/*系统帮助*/
   strcpy(ml[2].str[0],"This System        ");
   strcpy(ml[2].str[1],"Ver           ");
   ml[2].n=2;
}
/*显示主单名*/
void DrawSelectitem()
{
   for(i=0;i<3;i++)
   RedText(i,1,ml[i].name); /*显示主菜单名，且首字母为红色*/
}
/*正常显示菜单*/
void RedText(int x,int y,char *z)
{
   textbackground(7); /*设置背景颜色为浅灰色*/
   gotoxy(3+x*20,y);
   for(j=0;z[j];j++)
   {
      if(j==0)
	 textcolor(RED);/*第一个字母显示红色*/
      else
	 textcolor(BLACK); /*设置黑色*/
      cprintf("%c",z[j]); /*输出菜单名*/
   }
}
/*显示选中菜单*/
void BlackText(int x,int y,char *z)
{
   textbackground(0); /*设置背景颜色为黑色*/
   textcolor(15); /*设置文本颜色为白色*/
   gotoxy(3+20*x,y);/*定位坐标*/
   cputs(z); /*输出菜单名字符串*/
}
/*按键操作过程*/
void Run()
{
   while(1)
   {
      gotoxy(texty,textx);
      key=bioskey(0);/*接收按键*/
      switch(key)
      {
	 case ALT_B:
	 case ESC: DrawMl(0);break; /*显示下拉菜单1*/
	 case ALT_M: DrawMl(1);break;/*显示下拉菜单2*/
	 case ALT_H: DrawMl(2);/*显示下拉菜单3*/
	 case UP:  /*上光标键的操作控制*/
	    {
	       if(textx==3)
		  textx=23;
	       textx--;
	       gotoxy(texty,textx);
	    }break;
	 case DOWN:  /*下光标键的操作控制*/
	    {
	       if(textx==23)
		  textx=3;
	       textx++;
	       gotoxy(texty,textx);
	    }break;
	 case LEFT: /*左光标键的操作控制*/
	    {
	       if(texty==2)
		  texty=79;
	       texty--;
	       gotoxy(texty,textx);
	    }break;
	 case Backspace: /*擦除键的设置*/
	    {
	       if(texty==2&&textx==3)
		  continue;
	       else
	       {
		  if(texty!=2)
		     texty--; /*擦除键的细节问题,先擦去东西,然后光标还要往后退一格*/
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
	 case RIGHT: /*右光标键的操作控制*/
	    {
	       if(texty==79)
		  texty=2;
	       texty++;
	       gotoxy(texty,textx);
	    }break;
	 case SPACE: /*空格键的操作*/
	    {
	       if(texty==79)
		  continue;
	       else
	       {
		  gotoxy(texty,textx); /*空格的细节操作*/
		  printf(" ");
		  texty++;
		  gotoxy(texty,textx);
	       }
	    }break;
	 case ENTER: /*回车的控制操作*/
	    {
	       if(textx==23)
		  continue;
	       textx++;
	       texty=2;
	       gotoxy(texty,textx);
	    }break;
	 default : /*非控制键的结果*/
	    {
	       if(texty==79&&textx==23)/*到达最后就不再输出*/
		  continue;
	       else
		  if(texty==79&&textx!=23) /*到行的最后*/
		  {
		     textx++;
		     texty=2;
		  }
	       gotoxy(texty,textx);/*输出结果*/
	       printf("%c",key);
	       if(texty==79) /*如果texty==79就不执行*/
		  continue;
	       else /*如果没到行尾就继续执行,使光标向前移动一位*/
		  texty++;
	 }
      }
   }/*大循环的大括号*/
}
/*画边框函数*/
void DrawFrame(int l,int u,int r,int d,int tcolor,int bcolor)
{
   textbackground(bcolor); /*背景颜色*/
   textcolor(bcolor); /*文本颜色*/
   for(i=l;i<=r;i++) /*输出背景区域*/
   {
      for(j=u;j<=d;j++)
      {
	 gotoxy(i,j);
	 printf("%c",219); /*输出背景字符*/
      }
   }
   textcolor(tcolor);/*边框颜色*/
   for(i=u+1;i<d;i++) /*在背景区域内输出边框线*/
   {
      gotoxy(l,i);
      cprintf("%c",179); /*垂直线*/
      gotoxy(r,i);
      cprintf("%c",179);
   }
   for(i=l+1;i<r;i++)
   {
      gotoxy(i,u);
      cprintf("%c",196); /*水平线*/
      gotoxy(i,d);
      cprintf("%c",196);
   }
   gotoxy(l,u);
   cprintf("%c",218);/*左上角*/
   gotoxy(r,u);
   cprintf("%c",191);/*右上角*/
   gotoxy(l,d);
   cprintf("%c",192);/*左下角*/
   gotoxy(r,d);
   cprintf("%c",217); /*右下角*/
/* gettext(l+1,u+1,r-1,d-1,save1);*//*保存边框内区域*/
}
/*显示具体下拉选择项目*/
void DrawMl(int n)
{
   gettext(1,1,80,25,save);/*保存被掩盖的地方*/
   BlackText(n,1,ml[n].name);/*反选显示主菜单*/
   DrawFrame(3+20*n-1,2,3+20*n+19,3+ml[n].n,0,7);/*下拉菜单的边框*/
   for(i=3;i<3+ml[n].n;i++)/*输出所选菜单各选项*/
   {
      if(i==3)
	 BlackText(n,i,ml[n].str[i-3]);/*默认选中第一项*/
      else
	 RedText(n,i,ml[n].str[i-3]);/*其余各项首字符红色显示*/
   }
   gotoxy(79,1);
   MoveMl(n,3);/*菜单选项的控制*/
}
/*菜单选项的控制，n决定水平项，x决定下拉的选项*/
void MoveMl(int n,int x)
{
   int flag=1;
   while(flag)
   {
      gotoxy(79,1);
      key=bioskey(0);/*接收按键*/
      gotoxy(79,1);
      switch(key)
      {
	 case ESC:/*退出循环*/
		  puttext(1,1,80,25,save);/*恢复打开菜单前的样子*/
		  flag=0;
		  break;
	 case LEFT:/*移到左边的选项*/
		  puttext(1,1,80,25,save);/*恢复打开菜单前的样子*/
		  if(n==0)/*往左移动越界的话移到最后一个选项*/
		     DrawMl(2);
		  else
		     DrawMl(n-1);
		  flag=0;
		  break;
	 case RIGHT:/*移动右边的选项*/
		  puttext(1,1,80,25,save);/*恢复打开菜单前的样子*/
		  if(n==2)/*往右移动越界的话移到第一个选项*/
		     DrawMl(0);
		  else
		     DrawMl(n+1);
		  flag=0;
		  break;
	 case UP:/*具体选项往上移动*/
		  RedText(n,x,ml[n].str[x-3]);/*输出红色字体*/
		  if(x==3)/*移到最上面再按上键，就移到最下面*/
		     x=3+ml[n].n-1;
		  else
		     x--;/*移动到新的要显示的内容*/
		  BlackText(n,x,ml[n].str[x-3]);/*输出黑色字体*/
		  flag=1;
		  break;
	 case DOWN:/*具体选项往下移动*/
		  RedText(n,x,ml[n].str[x-3]);
		  if(x==(3+ml[n].n-1))/*移动到最底下再按下键就移到最上面*/
		     x=3;
		  else
		     x++;/*移动到新的要显示的内容*/
		  BlackText(n,x,ml[n].str[x-3]);
		  flag=1;
		  break;
	 case ENTER:
		  puttext(1,1,80,25,save);/*恢复打开菜单前的样子*/
		  Enter(n,x-3);/*菜单选项的具体功能*/
		  flag=0;
		  break;
	 }
      gotoxy(79,1);
   }
}
 /*菜单选项的具体功能*/
void Enter(int m,int n)
{
   switch(m)
   {
      case 0:switch(n) /*选择了图书菜单选项*/
	     {
		case 0:BookAdd();break;/*添加图书*/
		case 1:BookConsult();break;/*图书查询*/
		case 2:BookDel();break;/*删除一本图书资料*/
		case 3:BookBorrow();break;/*借书*/
		case 4:BookReturn();break;/*还书*/
		case 5:exit(0);break;
	     } break;/*退出系统*/
      case 1: switch(n) /*选择了会员菜单选项*/
	      {
		case 0: MemberAdd();break;/*添加会员*/
		case 1:  MemberConsult();break;/*会员查询*/
		case 2:MemberDel();break;/*删除一个会员资料*/
		case 3:MemberBook();/*查询某个会员所借图书情况*/
	      }break;
      case 2:switch(n) /*选择了帮助菜单选项*/
	     {
		case 0:Help();break;
		case 1:Ver();
	     }
   }/*结束外switch*/
}
