/*9.3.2 Դ����*/
/***pcb.c***/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX 32767
typedef struct node   /*���÷���������*/
{
   int address,size;
   struct node *next;
}RECT;
/*����ԭ��*/
RECT *assignment(RECT *head,int application);
void acceptment1(RECT *head,RECT *back1);
void acceptment2(RECT *head,RECT *back1) ;
int backcheck(RECT *head,RECT *back1);
void print(RECT *head);
/*��������*/
RECT *head,*back,*assign1,*p;
int application1,maxblocknum;
char way;
/*������*/
main()
{
   char choose[10];
   int check;
   head=malloc(sizeof(RECT)); /*���������������ĳ�ʼ״̬*/
   p=malloc(sizeof(RECT));
   head->size=MAX;
   head->address=0;
   head->next=p;
   maxblocknum=1;
   p->size=MAX;
   p->address=0;
   p->next=NULL;
   print(head);  /*��������ñ���ʼ״̬*/
   printf("Enter the way(best or first(b/f)\n");/*ѡ����Ӧ����*/
   scanf("%c",&way);
   do{
      printf("Enter the assign or accept(as/ac)\n");
      scanf("%s",choose); /*ѡ���������*/
      if(strcmp(choose,"as")==0) /*asΪ����*/
      {
	 printf("Input application:\n");
	 scanf("%d",&application1);/*��������ռ��С*/
	 assign1=assignment(head,application1);/*���÷��亯��*/
	 if(assign1->address==-1)/*���䲻�ɹ�*/
	    printf("Too large application!,assign fails!!\n\n");
	 else
	    printf("Success!!ADDRESS=%5d\n",assign1->address); /*����ɹ�*/
	 print(head); /*���*/
      }
      else
	 if(strcmp(choose,"ac")==0) /*����*/
	 {
	    back=malloc(sizeof(RECT));
	    printf("Input Adress and Size!!\n");
	    scanf("%d%d",&back->address,&back->size);/*������յ�ַ�ʹ�С*/
	    check=backcheck(head,back); /*���*/
	    if(check==1)
	    {
	       if(tolower(way)=='f')/*������Ӧ�㷨*/
		  acceptment1(head,back); /*������Ӧ*/
	       else
		  acceptment2(head,back);/*�����Ӧ*/
	       print(head);
	    }
	 }
   }while(!strcmp(choose,"as")||!strcmp(choose,"ac"));
}
/*���亯��*/
RECT *assignment(RECT *head,int application)
{
   RECT *after,*before,*assign;
   assign=malloc(sizeof(RECT)); /*��������ռ�*/
   assign->size=application;
   assign->next=NULL;
   if(application>head->size||application<=0)
      assign->address=-1; /*������Ч*/
   else
   {
      before=head;
      after=head->next;
      while(after->size<application)/*������Ӧ�Ľ��*/
      {
	 before=before->next;
	 after=after->next;
      }
      if(after->size==application) /*����С���������С����ȫ����*/
      {
	 if(after->size==head->size)
	    maxblocknum--;
	 before->next=after->next;
	 assign->address=after->address;
	 free(after);
      }
      else
      {
	 if(after->size==head->size) maxblocknum--;
	 after->size=after->size-application; /*��������ռ����ȡ��Ӧ��С����*/
	 assign->address=after->address+after->size;
	 if(tolower(way)=='b')/*����������Ӧ������ȡ��ʣ�������»��յ�����λ��*/
	 {
	    before->next=after->next;
	    back=after;
	    acceptment2(head,back);
	 }
      }
      if(maxblocknum==0) /*�޸��������ͷ���ֵ*/
      {
	 before=head;
	 head->size=0;
	 maxblocknum=1;
	 while(before!=NULL)
	 {
	    if(before->size>head->size)
	    {
	       head->size=before->size;
	       maxblocknum=1;
	    }
	    else
	       if(before->size==head->size)
		  maxblocknum++;
	    before=before->next;
	 }
      }
   }
   assign1=assign;
   return assign1; /*���ط�����û��ĵ�ַ*/
}
void acceptment1(RECT *head,RECT *back1)/*������Ӧ*/
{
   RECT *before,*after;
   int insert;
   before=head;
   after=head->next;
   insert=0;
   while(!insert) /*�������������������*/
   {
      if((after==NULL)||
	 ((back1->address<=after->address)&&
	   (back1->address>=before->address)))
      {
	 before->next=back1;
	 back1->next=after;
	 insert=1;
      }
      else
      {
	 before=before->next;
	 after=after->next;
      }
   }
   if(back1->address==before->address+before->size)/*����һ��ϲ�*/
   {
      before->size=before->size+back1->size;
      before->next=back1->next;
      free(back1);
      back1=before;
   }
   if(after!=NULL&&(after->address==back1->address+back1->size))
   {  /*����һ��ϲ�*/
      back1->size=back1->size+after->size;
      back1->next=after->next;
      free(after);
   }
   if(head->size<back1->size) /*�޸�����ֵ���������*/
   {
      head->size=back1->size;
      maxblocknum=1;
   }
   else
      if(head->size==back1->size)
	 maxblocknum++;
}
/*�����Ӧ��back1Ϊ���ս��ĵ�ַ*/
void acceptment2(RECT *head,RECT *back1)
{
   RECT *before,*after;
   int insert ;
   insert=0;
   before=head;
   after=head->next;
   if(head->next==NULL) /*�������������Ϊ��*/
   {
      head->size=back1->size;
      head->next=back1;
      maxblocknum++;
      back1->next=NULL;
   }
   else
   {
      while(after!=NULL) /*����һ��ϲ�*/
      if(back1->address==after->size+after->address)
      {
	 before->next=after->next;
	 back->size=after->size+back1->size;
	 free(after);
	 after=NULL;
      }
      else
      {
	 after=after->next;
	 before=before->next;
      }
      before=head;
      after=head->next;
      while(after!=NULL)
      if(after->address==back1->size+back1->address) /*����һ��ϲ�*/
      {
	 back1->size=back1->size+after->size;
	 before->next=after->next;
	 free(after);
	 after=NULL;
      }
      else
      {
	 before=before->next;
	 after=after->next;
      }
      before=head;/*�����ս����뵽���ʵ�λ��*/
      after=head->next;
      do{
	 if(after==NULL||(after->size>back1->size))
	 {
	    before->next=back1;
	    back1->next=after;
	    insert=1;
	 }
	 else
	 {
	    before=before->next;
	    after=after->next;
	 }
      }while(!insert);
      if(head->size<back1->size) /*�޸�����ֵ��������*/
      {
	 head->size=back1->size;
	 maxblocknum++;
      }
      else
	 if(head->size==back1->size)
	    maxblocknum++;
   }
}

void print(RECT *head) /*�������*/
{
   RECT *before,*after;
   int index,k;
   before=head->next;
   index=1;
   if(head->next==NULL)
      printf("NO part for assignment!!\n");
   else
   {
      printf("*****index*******address********end*********size*****\n");
      while(before!=NULL)
      {
	 printf("----------------------------------------------------\n");
	 printf("     %-13d%-13d%-13d%-13d\n",index,before->address,before->address+before->size-1,before->size);
	 printf("----------------------------------------------------\n");
	 index++;
	 before=before->next;
      }
   }
}
/*�����տ�ĺϷ��ԣ�back1ΪҪ���յĽ���ַ*/
int backcheck(RECT *head,RECT *back1)
{
   RECT *before,*after;
   int check=1;
   if(back1->address<0||back1->size<0)
      check=0;/*��ַ�ʹ�С����Ϊ��*/
   before=head->next;
   while((before!=NULL)&&check)/*��ַ���ܺͿ��������н������ص�*/
      if(((back1->address<before->address)
	 &&(back1->address+back1->size>before->address))
	 ||((back1->address>=before->address)
	&&(back1->address<before->address+before->size)))
	 check=0;
      else
	 before=before->next;
   if(check==0)
      printf("Error input!!\n");
   return check;  /*���ؼ����*/
}
