/*12. bookfunction.c源程序*/
/*** bookfunction.c***/
/*添加图书*/
#include "book.h"
void BookAdd()
{
   FILE *fp;
   Book consultbook;
   fp=fopen("book.txt","rb");/*读方式打开文件*/
   if(fp==NULL)
      fp=fopen("book.txt","wb"); /*写方式打开文件*/
   else/*如果有书就添加*/
   {
      fclose(fp);
      fp=fopen("book.txt","ab");/*追加方式打开文件*/
   }
   ClrScr();/*清屏*/
   printf("Intput new book num: ");/*逐个输入新图书的资料*/
   gets(consultbook.num);
   gotoxy(2,4);
   printf("Input new book name: ");
   gets(consultbook.name);
   gotoxy(2,5);
   printf("Please input new book's price: ");
   scanf("%d%*c",&consultbook.price);
   strcpy(consultbook.person,"");
   consultbook.yes=1;/*表示书存在未借出*/
   consultbook.next=NULL;
   fwrite(&consultbook,sizeof(Book),1,fp);/*块写*/
   fclose(fp); /*关闭文件*/
   ClrScr();
}
/*图书查询*/
void BookConsult()
{
   FILE *fp;
   Book consultbook;
   char bookname[20]; /*书名*/
   int flag=0,i=0;
   ClrScr();
   gotoxy(2,3);
   printf("Input the book name: ");
   gets(bookname); /*输入要查询的图书名*/
   fp=fopen("book.txt","rb");
   if(fp==NULL)/*没找到文件*/
   {
      ClrScr();
      gotoxy(2,3);
      printf("Error!");
      fclose(fp);
      return;
   }
   else
      while(!feof(fp))/*查询图书资料*/
      {
	 fread(&consultbook,sizeof(Book),1,fp);
	 if(strcmp(consultbook.name,bookname)==0)/*如果查找到*/
	 {
	    ClrScr();
	    gotoxy(2,3+i*5);
	    printf("This book %s information as follow:",consultbook.name);
	    gotoxy(2,4+i*5);
	    printf("This book num is %s",consultbook.num);
	    gotoxy(2,5+i*5);
	    printf("This book's price is %d",consultbook.price);
	    gotoxy(2,6+i*5);
	    printf("This book's  is browwed by %s",consultbook.person);
	    gotoxy(2,7+i*5);
	    printf("This book's state is %d",consultbook.yes);
	    i++;
	    flag=1; /*查找数据标志*/
	 }
      }
   if(flag==0)
   {
      ClrScr();
      gotoxy(2,3);
      printf("No found this book");
   }
   fclose(fp);
}
/*删除图书资料*/
void BookDel()
{
   FILE *fp;
   Book *head,*p,*q;
   char bookname[20];
   fp=fopen("book.txt","rb");/*读方式打开文件*/
   if(fp==NULL)/*没找到文件*/
   {
      gotoxy(2,3);
      printf("Error!");
      fclose(fp);
      return;
   }
   head=p=q=(Book*)malloc(sizeof(Book));/*将文件内容放入链表*/
   fread(p,sizeof(Book),1,fp);	/*读第一条记录*/
   while(!feof(fp))
   {
      q=p;
      p=(Book*)malloc(sizeof(Book));
      fread(p,sizeof(Book),1,fp); /*读记录*/
      q->next=p;/*新记录插入到链表的尾*/
   }
   p->next=NULL;/*最后一个结点的后继为空*/
   fclose(fp);
   ClrScr();
   printf("Input the book name: ");
   gets(bookname); /*输入图书名*/
   p=head;
   while(p!=NULL)/*按名字查找要删除的图书*/
   {
      if(strcmp(p->name,bookname)==0)/*找到要删除的图书*/
      {
	 if(p==head) /*如果是第一个结点*/
	    head=head->next;
	 else
	    q->next=p->next;/*不是第一个结点*/
	 break;
      }
      q=p;  /*指针后移*/
      p=p->next;
   }
   fp=fopen("book.txt","wb");/*删除后从链表的头开始重新保存*/
   while(head!=NULL)
   {
      fwrite(head,sizeof(Book),1,fp);/*块写*/
      head=head->next;
   }
   fclose(fp);
   ClrScr();
}
 /*借书*/
void BookBorrow()
{
   FILE *fp;
   Book consultbook;
   Member  consultmember;
   char bookname[20],membername[20];
   int flag=0,mflag=0;
   ClrScr();
   gotoxy(2,3);
   printf("Input the mermber name: ");
   gets(membername); /*输入会员名*/
   if((fp=fopen("member.txt","rb"))==NULL)/*没找到文件*/
   {
      gotoxy(3,3);
      printf("Cannot open file\n!");
      return;
   }
   else
      while(!feof(fp))/*查询会员资料*/
      {
	 fread(&consultmember,sizeof(Member),1,fp);
	 if(strcmp(consultmember.name,membername)==0)
	 {
	    mflag=1; /*是会员标志*/
	    break;
	 }
      }
   fclose(fp);
   if(mflag==0)
   {
      gotoxy(2,4);
      printf("You not is a member,pleas register!");/*不是会员不能借书*/
      return;
   }
   gotoxy(3,5);
   printf("Input the book name: ");
   gets(bookname); /*输入图书名*/
   ClrScr();
   if((fp=fopen("book.txt","rb+"))==NULL)/*没找到文件*/
   {
      gotoxy(2,3);
      printf("Cannot open file\n!");
      return;
   }
   while(!feof(fp))/*查询图书资料*/
   {
      fread(&consultbook,sizeof(Book),1,fp);
      if(strcmp(consultbook.name,bookname)==0)
      {
	 if(consultbook.yes==0)/*图书已经借出的判断*/
	 {
	    gotoxy(2,3);
	    printf("This book has borrowed\n!");
	    break;
	 }
	 else
	 {
	    consultbook.yes=0; /*作借出标志*/
	    strcpy(consultbook.person,membername);/*登记借书会员*/
	    fseek(fp,-1L*sizeof(Book),1);/*从当前位置前移一条记录指针*/
	    fwrite(&consultbook,sizeof(Book),1,fp);/*写记录*/
	    gotoxy(2,3);
	    printf("Borrowed succ!");
	    flag=1;/*借出书标志*/
	    break;
	 }
      }
   }
   if(flag!=1)
   {
      ClrScr();
      gotoxy(2,3);
      printf("Borrowed fail!\n");/*借书失败*/
   }
   fclose(fp);
}
/*还书*/
void BookReturn()
{
   FILE *fp;
   Book consultbook;
   char bookname[20];
   int flag=0;
   ClrScr();
   if((fp=fopen("book.txt","rb+"))==NULL)/*没找到文件*/
   {
      gotoxy(2,3);
      printf("cannot open file\n!");
      return;
   }
   printf("Input the book name: ");
   gets(bookname); /*输入图书名*/
   ClrScr();
   while(!feof(fp))/*查询图书资料*/
   {
      fread(&consultbook,sizeof(Book),1,fp);
      if(strcmp(consultbook.name,bookname)==0)
      {
	 if(consultbook.yes==0)/*图书已经借出的判断*/
	 {
	    consultbook.yes=1;/*作图书未借书标志，表示已还书*/
	    strcpy(consultbook.person,"");
	    fseek(fp,-1L*sizeof(Book),1);
	    fwrite(&consultbook,sizeof(Book),1,fp);
	    gotoxy(2,3);
	    printf("Return book succ!");
	    flag=1;
	    break;
	 }
      }
   }
   if(flag!=1)
      printf("Return fail!\n");
   fclose(fp);
}
/*清除屏幕*/
void ClrScr()
{
   int i,j;
   puttext(2,3,78,23,c);/*刚开始已经用gettext把蓝色的一块地方保存下来,现在再还原*/
   gotoxy(2,3);
}
/*会员注册*/
void MemberAdd()
{
   FILE *fp;
   Member consultmember;
   fp=fopen("member.txt","rb");
   if(fp==NULL)/*如果没会员就新建*/
      fp=fopen("member.txt","wb");
   else/*如果有会员就添加*/
   {
      fclose(fp);
      fp=fopen("member.txt","ab");/*追加方式打开*/
   }
   ClrScr();
   gotoxy(2,3);
   printf("Intput new member name: ");/*逐个输入会员的资料*/
   gets(consultmember.name);
   gotoxy(2,4);
   printf("Input member sex: ");
   gets(consultmember.sex);
   gotoxy(2,5);
   printf("Please input member age: ");
   scanf("%d%*c",&consultmember.age);
   fwrite(&consultmember,sizeof(Member),1,fp);/*写入记录*/
   fclose(fp);
   ClrScr();
}
/*查询会员*/
void MemberConsult()
{
   FILE *fp;
   Member consultmember;
   char membername[20];
   int flag=0;
   ClrScr();
   gotoxy(2,3);
   printf("Input the member name: ");/*输入要查询的会员名*/
   gets(membername);
   fp=fopen("member.txt","rb");
   if(fp==NULL)/*没找到文件*/
   {
      ClrScr();
      gotoxy(2,3);
      printf("Error!");
      fclose(fp);
      return;
   }
   while(!feof(fp))/*查询会员资料*/
   {
      fread(&consultmember,sizeof(Member),1,fp);
      if(strcmp(consultmember.name,membername)==0)/*比较*/
      {
	 ClrScr();
	 gotoxy(2,3);
	 printf("This member %s information as follow:",consultmember.name);
	 gotoxy(2,4);
	 printf("This member'sex  is %s",consultmember.sex);
	 gotoxy(2,5);
	 printf("This member 's age is %d",consultmember.age);
	 flag=1;
	 break;
      }
   }
   if(flag==0)
   {
      ClrScr();
      gotoxy(2,3);
      printf("No found this member!");
   }
   fclose(fp);
}
/*删除会员资料*/
void MemberDel()
{
   FILE *fp;
   Member *head,*p,*q;
   char membername[20];
   fp=fopen("member.txt","rb");
   if(fp==NULL)/*没找到文件*/
   {
      gotoxy(2,3);
      printf("Error!");
      fclose(fp);
      return;
   }
   head=p=q=(Member*)malloc(sizeof(Member));/*将文件内容放入链表*/
   fread(p,sizeof(Member),1,fp);
   while(!feof(fp))
   {
      q=p;
      p=(Member*)malloc(sizeof(Member));
      fread(p,sizeof(Member),1,fp);/*读记录*/
      q->next=p;/*链入到链表的尾部*/
   }
   p->next=NULL;
   fclose(fp);
   ClrScr();
   printf("Input the member name: ");/*输入会员名*/
   gets(membername);
   p=head;
   while(p!=NULL)/*按名字查找要删除的书*/
   {
      if(strcmp(p->name,membername)==0)/*查找会员*/
      {
	 if(p==head)
	    head=head->next;/*如果是第一条记录*/
	 else
	    q->next=p->next;/*不是第一条记录*/
	 break;
      }
      q=p;  /*没找到，则指针后移继续找*/
      p=p->next;
   }
   fp=fopen("member.txt","wb");/*删除后重新保存*/
   while(head!=NULL)
   {
      fwrite(head,sizeof(Member),1,fp);/*块写*/
      head=head->next;
   }
   fclose(fp);
   ClrScr();
}
/*查询会员所借的图书*/
void MemberBook()
{
   FILE *fp;
   Book consultbook;
   Member  consultmember;
   char bookname[20],membername[20];
   int flag=0,mflag=0,i=0;
   ClrScr();
   gotoxy(2,3);
   printf("Input the mermber name: ");/*输入会员名*/
   gets(membername);
   if((fp=fopen("member.txt","rb"))==NULL)/*没找到文件*/
   {
      gotoxy(3,3);
      printf("Cannot open file\n!");
      return;
   }
   else
      while(!feof(fp))/*查询会员资料，确认会员*/
      {
	 fread(&consultmember,sizeof(Member),1,fp);
	 if(strcmp(consultmember.name,membername)==0)
	 {
	    mflag=1;
	    break;
	 }
      }
   fclose(fp);
   if(mflag==0)
   {
      gotoxy(2,4);
      printf("Not the member!");
      return;
   }
   ClrScr();
   if((fp=fopen("book.txt","rb+"))==NULL)/*没找到文件*/
   {
      gotoxy(2,3);
      printf("Cannot open file\n!");
      return;
   }
   while(!feof(fp))/*查询图书资料*/
   {
      fread(&consultbook,sizeof(Book),1,fp);
      gotoxy(2,3);
      printf("This member %s has borrowed book  as follow:",membername);
      gotoxy(2,4);
      printf("********************************************");
      if(strcmp(consultbook.person,membername)==0)/*借阅人比较*/
      {
	 gotoxy(2,5+i*4);
	 printf("Book name is %s",consultbook.name);
	 gotoxy(2,6+i*4);
	 printf("This book num is %s",consultbook.num);
	 gotoxy(2,7+i*4);
	 printf("This book's price is %d",consultbook.price);
	 printf("\n");
	 i++;
	 flag=1;
      }
   }
   if(flag!=1)
   {
      ClrScr();
      gotoxy(2,3);
      printf("no Borrow book!\n");
   }
   else
   {
      gotoxy(2,i*4+6);
      printf("******total is %d*********",i);/*显示总数*/
   }
   fclose(fp);
}
/*帮助文件*/
void Help()
{
   ClrScr();
   DrawFrame(10,5,50,8,0,7); /*画边框*/
   gotoxy(15,6);
   cprintf("click menu to run each funtion");/*输出字符串*/
   getch();
   ClrScr();
}
/***版本信息***/
void Ver()
{
   ClrScr();
   DrawFrame(10,5,50,8,0,7); /*画边框*/
   gotoxy(15,6);
   cprintf("Ver 1.0 finished by zhuyun");/*输出字符串*/
   getch();
   ClrScr();
}
