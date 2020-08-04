/*12. bookfunction.cԴ����*/
/*** bookfunction.c***/
/*���ͼ��*/
#include "book.h"
void BookAdd()
{
   FILE *fp;
   Book consultbook;
   fp=fopen("book.txt","rb");/*����ʽ���ļ�*/
   if(fp==NULL)
      fp=fopen("book.txt","wb"); /*д��ʽ���ļ�*/
   else/*�����������*/
   {
      fclose(fp);
      fp=fopen("book.txt","ab");/*׷�ӷ�ʽ���ļ�*/
   }
   ClrScr();/*����*/
   printf("Intput new book num: ");/*���������ͼ�������*/
   gets(consultbook.num);
   gotoxy(2,4);
   printf("Input new book name: ");
   gets(consultbook.name);
   gotoxy(2,5);
   printf("Please input new book's price: ");
   scanf("%d%*c",&consultbook.price);
   strcpy(consultbook.person,"");
   consultbook.yes=1;/*��ʾ�����δ���*/
   consultbook.next=NULL;
   fwrite(&consultbook,sizeof(Book),1,fp);/*��д*/
   fclose(fp); /*�ر��ļ�*/
   ClrScr();
}
/*ͼ���ѯ*/
void BookConsult()
{
   FILE *fp;
   Book consultbook;
   char bookname[20]; /*����*/
   int flag=0,i=0;
   ClrScr();
   gotoxy(2,3);
   printf("Input the book name: ");
   gets(bookname); /*����Ҫ��ѯ��ͼ����*/
   fp=fopen("book.txt","rb");
   if(fp==NULL)/*û�ҵ��ļ�*/
   {
      ClrScr();
      gotoxy(2,3);
      printf("Error!");
      fclose(fp);
      return;
   }
   else
      while(!feof(fp))/*��ѯͼ������*/
      {
	 fread(&consultbook,sizeof(Book),1,fp);
	 if(strcmp(consultbook.name,bookname)==0)/*������ҵ�*/
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
	    flag=1; /*�������ݱ�־*/
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
/*ɾ��ͼ������*/
void BookDel()
{
   FILE *fp;
   Book *head,*p,*q;
   char bookname[20];
   fp=fopen("book.txt","rb");/*����ʽ���ļ�*/
   if(fp==NULL)/*û�ҵ��ļ�*/
   {
      gotoxy(2,3);
      printf("Error!");
      fclose(fp);
      return;
   }
   head=p=q=(Book*)malloc(sizeof(Book));/*���ļ����ݷ�������*/
   fread(p,sizeof(Book),1,fp);	/*����һ����¼*/
   while(!feof(fp))
   {
      q=p;
      p=(Book*)malloc(sizeof(Book));
      fread(p,sizeof(Book),1,fp); /*����¼*/
      q->next=p;/*�¼�¼���뵽�����β*/
   }
   p->next=NULL;/*���һ�����ĺ��Ϊ��*/
   fclose(fp);
   ClrScr();
   printf("Input the book name: ");
   gets(bookname); /*����ͼ����*/
   p=head;
   while(p!=NULL)/*�����ֲ���Ҫɾ����ͼ��*/
   {
      if(strcmp(p->name,bookname)==0)/*�ҵ�Ҫɾ����ͼ��*/
      {
	 if(p==head) /*����ǵ�һ�����*/
	    head=head->next;
	 else
	    q->next=p->next;/*���ǵ�һ�����*/
	 break;
      }
      q=p;  /*ָ�����*/
      p=p->next;
   }
   fp=fopen("book.txt","wb");/*ɾ����������ͷ��ʼ���±���*/
   while(head!=NULL)
   {
      fwrite(head,sizeof(Book),1,fp);/*��д*/
      head=head->next;
   }
   fclose(fp);
   ClrScr();
}
 /*����*/
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
   gets(membername); /*�����Ա��*/
   if((fp=fopen("member.txt","rb"))==NULL)/*û�ҵ��ļ�*/
   {
      gotoxy(3,3);
      printf("Cannot open file\n!");
      return;
   }
   else
      while(!feof(fp))/*��ѯ��Ա����*/
      {
	 fread(&consultmember,sizeof(Member),1,fp);
	 if(strcmp(consultmember.name,membername)==0)
	 {
	    mflag=1; /*�ǻ�Ա��־*/
	    break;
	 }
      }
   fclose(fp);
   if(mflag==0)
   {
      gotoxy(2,4);
      printf("You not is a member,pleas register!");/*���ǻ�Ա���ܽ���*/
      return;
   }
   gotoxy(3,5);
   printf("Input the book name: ");
   gets(bookname); /*����ͼ����*/
   ClrScr();
   if((fp=fopen("book.txt","rb+"))==NULL)/*û�ҵ��ļ�*/
   {
      gotoxy(2,3);
      printf("Cannot open file\n!");
      return;
   }
   while(!feof(fp))/*��ѯͼ������*/
   {
      fread(&consultbook,sizeof(Book),1,fp);
      if(strcmp(consultbook.name,bookname)==0)
      {
	 if(consultbook.yes==0)/*ͼ���Ѿ�������ж�*/
	 {
	    gotoxy(2,3);
	    printf("This book has borrowed\n!");
	    break;
	 }
	 else
	 {
	    consultbook.yes=0; /*�������־*/
	    strcpy(consultbook.person,membername);/*�Ǽǽ����Ա*/
	    fseek(fp,-1L*sizeof(Book),1);/*�ӵ�ǰλ��ǰ��һ����¼ָ��*/
	    fwrite(&consultbook,sizeof(Book),1,fp);/*д��¼*/
	    gotoxy(2,3);
	    printf("Borrowed succ!");
	    flag=1;/*������־*/
	    break;
	 }
      }
   }
   if(flag!=1)
   {
      ClrScr();
      gotoxy(2,3);
      printf("Borrowed fail!\n");/*����ʧ��*/
   }
   fclose(fp);
}
/*����*/
void BookReturn()
{
   FILE *fp;
   Book consultbook;
   char bookname[20];
   int flag=0;
   ClrScr();
   if((fp=fopen("book.txt","rb+"))==NULL)/*û�ҵ��ļ�*/
   {
      gotoxy(2,3);
      printf("cannot open file\n!");
      return;
   }
   printf("Input the book name: ");
   gets(bookname); /*����ͼ����*/
   ClrScr();
   while(!feof(fp))/*��ѯͼ������*/
   {
      fread(&consultbook,sizeof(Book),1,fp);
      if(strcmp(consultbook.name,bookname)==0)
      {
	 if(consultbook.yes==0)/*ͼ���Ѿ�������ж�*/
	 {
	    consultbook.yes=1;/*��ͼ��δ�����־����ʾ�ѻ���*/
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
/*�����Ļ*/
void ClrScr()
{
   int i,j;
   puttext(2,3,78,23,c);/*�տ�ʼ�Ѿ���gettext����ɫ��һ��ط���������,�����ٻ�ԭ*/
   gotoxy(2,3);
}
/*��Աע��*/
void MemberAdd()
{
   FILE *fp;
   Member consultmember;
   fp=fopen("member.txt","rb");
   if(fp==NULL)/*���û��Ա���½�*/
      fp=fopen("member.txt","wb");
   else/*����л�Ա�����*/
   {
      fclose(fp);
      fp=fopen("member.txt","ab");/*׷�ӷ�ʽ��*/
   }
   ClrScr();
   gotoxy(2,3);
   printf("Intput new member name: ");/*��������Ա������*/
   gets(consultmember.name);
   gotoxy(2,4);
   printf("Input member sex: ");
   gets(consultmember.sex);
   gotoxy(2,5);
   printf("Please input member age: ");
   scanf("%d%*c",&consultmember.age);
   fwrite(&consultmember,sizeof(Member),1,fp);/*д���¼*/
   fclose(fp);
   ClrScr();
}
/*��ѯ��Ա*/
void MemberConsult()
{
   FILE *fp;
   Member consultmember;
   char membername[20];
   int flag=0;
   ClrScr();
   gotoxy(2,3);
   printf("Input the member name: ");/*����Ҫ��ѯ�Ļ�Ա��*/
   gets(membername);
   fp=fopen("member.txt","rb");
   if(fp==NULL)/*û�ҵ��ļ�*/
   {
      ClrScr();
      gotoxy(2,3);
      printf("Error!");
      fclose(fp);
      return;
   }
   while(!feof(fp))/*��ѯ��Ա����*/
   {
      fread(&consultmember,sizeof(Member),1,fp);
      if(strcmp(consultmember.name,membername)==0)/*�Ƚ�*/
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
/*ɾ����Ա����*/
void MemberDel()
{
   FILE *fp;
   Member *head,*p,*q;
   char membername[20];
   fp=fopen("member.txt","rb");
   if(fp==NULL)/*û�ҵ��ļ�*/
   {
      gotoxy(2,3);
      printf("Error!");
      fclose(fp);
      return;
   }
   head=p=q=(Member*)malloc(sizeof(Member));/*���ļ����ݷ�������*/
   fread(p,sizeof(Member),1,fp);
   while(!feof(fp))
   {
      q=p;
      p=(Member*)malloc(sizeof(Member));
      fread(p,sizeof(Member),1,fp);/*����¼*/
      q->next=p;/*���뵽�����β��*/
   }
   p->next=NULL;
   fclose(fp);
   ClrScr();
   printf("Input the member name: ");/*�����Ա��*/
   gets(membername);
   p=head;
   while(p!=NULL)/*�����ֲ���Ҫɾ������*/
   {
      if(strcmp(p->name,membername)==0)/*���һ�Ա*/
      {
	 if(p==head)
	    head=head->next;/*����ǵ�һ����¼*/
	 else
	    q->next=p->next;/*���ǵ�һ����¼*/
	 break;
      }
      q=p;  /*û�ҵ�����ָ����Ƽ�����*/
      p=p->next;
   }
   fp=fopen("member.txt","wb");/*ɾ�������±���*/
   while(head!=NULL)
   {
      fwrite(head,sizeof(Member),1,fp);/*��д*/
      head=head->next;
   }
   fclose(fp);
   ClrScr();
}
/*��ѯ��Ա�����ͼ��*/
void MemberBook()
{
   FILE *fp;
   Book consultbook;
   Member  consultmember;
   char bookname[20],membername[20];
   int flag=0,mflag=0,i=0;
   ClrScr();
   gotoxy(2,3);
   printf("Input the mermber name: ");/*�����Ա��*/
   gets(membername);
   if((fp=fopen("member.txt","rb"))==NULL)/*û�ҵ��ļ�*/
   {
      gotoxy(3,3);
      printf("Cannot open file\n!");
      return;
   }
   else
      while(!feof(fp))/*��ѯ��Ա���ϣ�ȷ�ϻ�Ա*/
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
   if((fp=fopen("book.txt","rb+"))==NULL)/*û�ҵ��ļ�*/
   {
      gotoxy(2,3);
      printf("Cannot open file\n!");
      return;
   }
   while(!feof(fp))/*��ѯͼ������*/
   {
      fread(&consultbook,sizeof(Book),1,fp);
      gotoxy(2,3);
      printf("This member %s has borrowed book  as follow:",membername);
      gotoxy(2,4);
      printf("********************************************");
      if(strcmp(consultbook.person,membername)==0)/*�����˱Ƚ�*/
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
      printf("******total is %d*********",i);/*��ʾ����*/
   }
   fclose(fp);
}
/*�����ļ�*/
void Help()
{
   ClrScr();
   DrawFrame(10,5,50,8,0,7); /*���߿�*/
   gotoxy(15,6);
   cprintf("click menu to run each funtion");/*����ַ���*/
   getch();
   ClrScr();
}
/***�汾��Ϣ***/
void Ver()
{
   ClrScr();
   DrawFrame(10,5,50,8,0,7); /*���߿�*/
   gotoxy(15,6);
   cprintf("Ver 1.0 finished by zhuyun");/*����ַ���*/
   getch();
   ClrScr();
}
