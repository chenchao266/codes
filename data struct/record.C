/*10.3.2源程序*/
/******头文件（.h）***********/
#include "stdio.h"   /*I/O函数*/
#include "stdlib.h" /*标准库函数*/
#include "string.h"/*字符串函数*/
#include "ctype.h" /*字符操作函数*/
#define M 50  /*定义常数表示记录数*/
typedef struct /*定义数据结构*/
{
   char name[20]; /*姓名*/
   char units[30];  /*单位*/
   char tele[10];  /*电话*/
}ADDRESS;
/******以下是函数原型*******/
int enter(ADDRESS t[]); /*输入记录*/
void list(ADDRESS t[],int n); /*显示记录*/
void search(ADDRESS t[],int n); /*按姓名查找显示记录*/
int delete(ADDRESS t[],int n); /*删除记录*/
int  add(ADDRESS t[],int n); /*插入记录*/
void save(ADDRESS t[],int n); /*记录保存为文件*/
int load(ADDRESS t[]);  /*从文件中读记录*/
void display(ADDRESS t[]); /*按序号查找显示记录*/
void sort(ADDRESS t[],int n); /*按姓名排序*/
void qseek(ADDRESS t[],int n); /*快速查找记录*/
void copy();  /*文件复制*/
void print(ADDRESS temp); /*显示单条记录*/
int find(ADDRESS t[],int n,char *s) ; /*查找函数*/
int menu_select();  /*主菜单函数*/
/******主函数开始*******/
main()
{
   int i;
   ADDRESS adr[M];  /*定义结构体数组*/
   int length;  /*保存记录长度*/
   clrscr();  /*清屏*/
   for(;;)/*无限循环*/
   {
      switch(menu_select())   /*调用主菜单函数，返回值整数作开关语句的条件*/
      {
	 case 0:length=enter(adr);break;/*输入记录*/
	 case 1:list(adr,length);break; /*显示全部记录*/
	 case 2:search(adr,length);break; /*查找记录*/
	 case 3:length=delete(adr,length);break; /*删除记录*/
	 case 4:length=add(adr,length);  break;   /*插入记录*/
	 case 5:save(adr,length);break; /*保存文件*/
	 case 6:length=load(adr); break; /*读文件*/
	 case 7:display(adr);break;  /*按序号显示记录*/
	 case 8:sort(adr,length);break; /*按姓名排序*/
	 case 9:qseek(adr,length);break; /*快速查找记录*/
	 case 10:copy();break; /*复制文件*/
	 case 11:exit(0); /*如返回值为11则程序结束*/
      }
   }
}
/*菜单函数，函数返回值为整数，代表所选的菜单项*/
menu_select()
{
   char s[80];
   int c;
   gotoxy(1,25);/*将光标定为在第25行，第1列*/
   printf("press any key enter menu......\n");/*提示压任意键继续*/
   getch(); /*读入任意字符*/
   clrscr(); /*清屏*/
   gotoxy(1,1);
   printf("********************MENU*********************\n\n");
   printf("		0. Enter record\n");
   printf("		1. List the file\n");
   printf("		2. Search record on name\n");
   printf("		3. Delete a record\n");
   printf("		4. add record \n");
   printf("		5. Save the file\n");
   printf("		6. Load the file\n");
   printf("		7. display record on order\n");
   printf("	        8. sort to make new file\n");
   printf("	        9. Quick seek record\n");
   printf("		10. copy the file to new file\n");
   printf("	        11. Quit\n");
   printf("***********************************************\n");
   do{
      printf("\n	 Enter you choice(0~11):"); /*提示输入选项*/
      scanf("%s",s); /*输入选择项*/
      c=atoi(s); /*将输入的字符串转化为整型数*/
   }while(c<0||c>11); /*选择项不在0~11之间重输*/
   return c; /*返回选择项，主程序根据该数调用相应的函数*/
}
/***输入记录，形参为结构体数组，函数值返回类型为整型表示记录长度*/
int  enter(ADDRESS t[])
{
   int i,n;
   char *s;
   clrscr(); /*清屏*/
   printf("\nplease input num \n"); /*提示信息*/
   scanf("%d",&n); /*输入记录数*/
   printf("please input record \n"); /*提示输入记录*/
   printf("name             unit                     telephone\n");
   printf("------------------------------------------------\n");
   for(i=0;i<n;i++)
   {
      scanf("%s%s%s",t[i].name,t[i].units,t[i].tele);  /*输入记录*/
      printf("----------------------------------------------\n");
   }
   return n;  /*返回记录条数*/
}
/*显示记录，参数为记录数组和记录条数*/
void list(ADDRESS t[],int n)
{
   int i;
   clrscr();
   printf("\n\n*******************ADDRESS******************\n");
   printf("name               unit                     telephone\n");
   printf("------------------------------------------------\n");
   for(i=0;i<n;i++)
   printf("%-20s%-30s%-10s\n",t[i].name,t[i].units,t[i].tele);
   if((i+1)%10==0)   /*判断输出是否达到10条记录*/
   {
      printf("Press any key continue...\n"); /*提示信息*/
      getch();  /*压任意键继续*/
   }
   printf("************************end*******************\n");
}
/*查找记录*/
void search(ADDRESS t[],int n)
{
   char s[20];   /*保存待查找姓名字符串*/
   int i;   /*保存查找到结点的序号*/
   clrscr();   /*清屏*/
   printf("please search name\n");
   scanf("%s",s); /*输入待查找姓名*/
   i=find(t,n,s); /*调用find函数，得到一个整数*/
   if(i>n-1)  /*如果整数i值大于n-1，说明没找到*/
      printf("not found\n");
   else
      print(t[i]);  /*找到，调用显示函数显示记录*/
}
/*显示指定的一条记录*/
void print(ADDRESS temp)
{
   clrscr();
   printf("\n\n********************************************\n");
   printf("name                unit                      telephone\n");
   printf("------------------------------------------------\n");
   printf("%-20s%-30s%-10s\n",temp.name,temp.units,temp.tele);
   printf("**********************end***********************\n");
}
/*查找函数，参数为记录数组和记录条数以及姓名s */
int find(ADDRESS t[],int n,char *s)
{
   int i;
   for(i=0;i<n;i++)/*从第一条记录开始，直到最后一条*/
   {
      if(strcmp(s,t[i].name)==0)  /*记录中的姓名和待比较的姓名是否相等*/
      return i;   /*相等，则返回该记录的下标号，程序提前结结束*/
   }
   return i;  /*返回i值*/
}
/*删除函数，参数为记录数组和记录条数*/
int delete(ADDRESS t[],int n)
{
   char s[20];  /*要删除记录的姓名*/
   int ch=0;
   int i,j;
   printf("please deleted name\n"); /*提示信息*/
   scanf("%s",s);/*输入姓名*/
   i=find(t,n,s); /*调用find函数*/
   if(i>n-1)  /*如果i>n-1超过了数组的长度*/
      printf("no found not deleted\n"); /*显示没找到要删除的记录*/
   else
   {
      print(t[i]); /*调用输出函数显示该条记录信息*/
      printf("Are you sure delete it(1/0)\n");  /*确认是否要删除*/
      scanf("%d",&ch);  /*输入一个整数0或1*/
      if(ch==1)  /*如果确认删除整数为1*/
      {
	 for(j=i+1;j<n;j++)  /*删除该记录，实际后续记录前移*/
	 {
	    strcpy(t[j-1].name,t[j].name); /*将后一条记录的姓名拷贝到前一条*/
	    strcpy(t[j-1].units,t[j].units); /*将后一条记录的单位拷贝到前一条*/
	    strcpy(t[j-1].tele,t[j].tele); /*将后一条记录的电话拷贝到前一条*/
	 }
	 n--;  /*记录数减1*/
      }
   }
   return n;  /*返回记录数*/
}
/*插入记录函数，参数为结构体数组和记录数*/
int add(ADDRESS t[],int n)/*插入函数，参数为结构体数组和记录数*/
{
   ADDRESS temp;  /*新插入记录信息*/
   int i,j;
   char s[20]; /*确定插入在哪个记录之前*/
   printf("please input record\n");
   printf("************************************************\n");
   printf("name                unit                      telephone\n");
   printf("--------------------------------------------------\n");
   scanf("%s%s%s",temp.name,temp.units,temp.tele); /*输入插入信息*/
   printf("------------------------------------------------\n");
   printf("please input locate name \n");
   scanf("%s",s); /*输入插入位置的姓名*/
   i=find(t,n,s);  /*调用find，确定插入位置*/
   for(j=n-1;j>=i;j--)   /*从最后一个结点开始向后移动一条*/
   {
      strcpy(t[j+1].name,t[j].name); /*当前记录的姓名拷贝到后一条*/
      strcpy(t[j+1].units,t[j].units); /*当前记录的单位拷贝到后一条*/
      strcpy(t[j+1].tele,t[j].tele); /*当前记录的电话拷贝到后一条*/
   }
   strcpy(t[i].name,temp.name); /*将新插入记录的姓名拷贝到第i个位置*/
   strcpy(t[i].units,temp.units); /*将新插入记录的单位拷贝到第i个位置*/
   strcpy(t[i].tele,temp.tele); /*将新插入记录的电话拷贝到第i个位置*/
   n++;   /*记录数加1*/
   return n; /*返回记录数*/
}
/*保存函数，参数为结构体数组和记录数*/
void save(ADDRESS t[],int n)
{
   int i;
   FILE *fp;  /*指向文件的指针*/
   if((fp=fopen("record.txt","wb"))==NULL)  /*打开文件，并判断打开是否正常*/
   {
      printf("can not open file\n");/*没打开*/
      exit(1);  /*退出*/
   }
   printf("\nSaving file\n"); /*输出提示信息*/
   fprintf(fp,"%d",n);  /*将记录数写入文件*/
   fprintf(fp,"\r\n");  /*将换行符号写入文件*/
   for(i=0;i<n;i++)
   {
      fprintf(fp,"%-20s%-30s%-10s",t[i].name,t[i].units,t[i].tele);/*格式写入记录*/
      fprintf(fp,"\r\n"); /*将换行符号写入文件*/
   }
   fclose(fp);/*关闭文件*/
   printf("****save success***\n"); /*显示保存成功*/
}
/*读入函数，参数为结构体数组*/
int load(ADDRESS t[])
{
   int i,n;
   FILE *fp; /*指向文件的指针*/
   if((fp=fopen("record.txt","rb"))==NULL)/*打开文件*/
   {
      printf("can not open file\n");  /*不能打开*/
      exit(1);  /*退出*/
   }
   fscanf(fp,"%d",&n); /*读入记录数*/
   for(i=0;i<n;i++)
      fscanf(fp,"%20s%30s%10s",t[i].name,t[i].units,t[i].tele); /*按格式读入记录*/
   fclose(fp);  /*关闭文件*/
   printf("You have success read data from file!!!\n"); /*显示保存成功*/
   return n; /*返回记录数*/
}
/*按序号显示记录函数*/
void display(ADDRESS t[])
{
   int id,n;
   FILE *fp; /*指向文件的指针*/
   if((fp=fopen("record.txt","rb"))==NULL) /*打开文件*/
   {
      printf("can not open file\n"); /*不能打开文件*/
      exit(1);  /*退出*/
   }
   printf("Enter order number...\n"); /*显示信息*/
   scanf("%d",&id);  /*输入序号*/
   fscanf(fp,"%d",&n); /*从文件读入记录数*/
   if(id>=0&&id<n) /*判断序号是否在记录范围内*/
   {
      fseek(fp,(id-1)*sizeof(ADDRESS),1); /*移动文件指针到该记录位置*/
      print(t[id]); /*调用输出函数显示该记录*/
      printf("\r\n");
   }
   else
      printf("no %d number record!!!\n ",id); /*如果序号不合理显示信息*/
   fclose(fp);  /*关闭文件*/
}
/*排序函数，参数为结构体数组和记录数*/
void sort(ADDRESS t[],int n)
{
   int i,j,flag;
   ADDRESS temp; /*临时变量做交换数据用*/
   for(i=0;i<n;i++)
   {
      flag=0;  /*设标志判断是否发生过交换*/
      for(j=0;j<n-1;j++)
      if((strcmp(t[j].name,t[j+1].name))>0) /*比较大小*/
      {
	 flag=1;
	 strcpy(temp.name,t[j].name);  /*交换记录*/
	 strcpy(temp.units,t[j].units);
	 strcpy(temp.tele,t[j].tele);
	 strcpy(t[j].name,t[j+1].name);
	 strcpy(t[j].units,t[j+1].units);
	 strcpy(t[j].tele,t[j+1].tele);
	 strcpy(t[j+1].name,temp.name);
	 strcpy(t[j+1].units,temp.units);
	 strcpy(t[j+1].tele,temp.tele);
      }
      if(flag==0)break;  /*如果标志为0，说明没有发生过交换循环结束*/
   }
   printf("sort sucess!!!\n"); /*显示排序成功*/
}
/*快速查找，参数为结构体数组和记录数*/
void qseek(ADDRESS t[],int n)
{
   char s[20];
   int l,r,m;
   printf("\nPlease  sort before qseek!\n"); /*提示确认在查找之前，记录是否已排序*/
   printf("please enter  name for qseek\n"); /*提示输入*/
   scanf("%s",s); /*输入待查找的姓名*/
   l=0;r=n-1;  /*设置左边界与右边界的初值*/
   while(l<=r) /*当左边界<=右边界时*/
   {
      m=(l+r)/2; /*计算中间位置*/
      if(strcmp(t[m].name,s)==0) /*与中间结点姓名字段做比较判是否相等*/
      {
	 print(t[m]); /*如果相等，则调用print函数显示记录信息*/
	 return ; /*返回*/
      }
      if(strcmp(t[m].name,s)<0)  /*如果中间结点小*/
	 l=m+1;  /*修改左边界*/
      else
	 r=m-1; /*否则，中间结点大，修改右边界*/
   }
   if(l>r)   /*如果左边界大于右边界时*/
      printf("not found\n"); /*显示没找到*/
}
/*复制文件*/
void copy()
{
   char outfile[20]; /*目标文件名*/
   int i,n;
   ADDRESS temp[M];  /*定义临时变量*/
   FILE *sfp,*tfp; /*定义指向文件的指针*/
   clrscr();/*清屏*/
   if((sfp=fopen("record.txt","rb"))==NULL) /*打开记录文件*/
   {
      printf("can not open file\n"); /*显示不能打开文件信息*/
      exit(1); /*退出*/
   }
   printf("Enter outfile name,for example c:\\f1\\te.txt:\n"); /*提示信息*/
   scanf("%s",outfile); /*输入目标文件名*/
   if((tfp=fopen(outfile,"wb"))==NULL) /*打开目标文件*/
   {
      printf("can not open file\n"); /*显示不能打开文件信息*/
      exit(1); /*退出*/
   }
   fscanf(sfp,"%d",&n); /*读出文件记录数*/
   fprintf(tfp,"%d",n);/*写入目标文件数*/
   fprintf(tfp,"\r\n"); /*写入换行符*/
   for(i=0;i<n;i++)
   {
      fscanf(sfp,"%20s%30s%10s\n",temp[i].name,temp[i].units,
	temp[i].tele); /*读入记录*/
      fprintf(tfp,"%-20s%-30s%-10s\n",temp[i].name,
	temp[i].units,temp[i].tele); /*写入记录*/
      fprintf(tfp,"\r\n"); /*写入换行符*/
   }
   fclose(sfp); /*关闭源文件*/
   fclose(tfp); /*关闭目标文件*/
   printf("you have success copy  file!!!\n"); /*显示复制成功*/
}
