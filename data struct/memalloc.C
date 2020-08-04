/*9.3.2 源程序*/
/***pcb.c***/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX 32767
typedef struct node   /*设置分区描述器*/
{
   int address,size;
   struct node *next;
}RECT;
/*函数原型*/
RECT *assignment(RECT *head,int application);
void acceptment1(RECT *head,RECT *back1);
void acceptment2(RECT *head,RECT *back1) ;
int backcheck(RECT *head,RECT *back1);
void print(RECT *head);
/*变量声明*/
RECT *head,*back,*assign1,*p;
int application1,maxblocknum;
char way;
/*主函数*/
main()
{
   char choose[10];
   int check;
   head=malloc(sizeof(RECT)); /*建立可利用区表的初始状态*/
   p=malloc(sizeof(RECT));
   head->size=MAX;
   head->address=0;
   head->next=p;
   maxblocknum=1;
   p->size=MAX;
   p->address=0;
   p->next=NULL;
   print(head);  /*输出可利用表初始状态*/
   printf("Enter the way(best or first(b/f)\n");/*选择适应策略*/
   scanf("%c",&way);
   do{
      printf("Enter the assign or accept(as/ac)\n");
      scanf("%s",choose); /*选择分配或回收*/
      if(strcmp(choose,"as")==0) /*as为分配*/
      {
	 printf("Input application:\n");
	 scanf("%d",&application1);/*输入申请空间大小*/
	 assign1=assignment(head,application1);/*调用分配函数*/
	 if(assign1->address==-1)/*分配不成功*/
	    printf("Too large application!,assign fails!!\n\n");
	 else
	    printf("Success!!ADDRESS=%5d\n",assign1->address); /*分配成功*/
	 print(head); /*输出*/
      }
      else
	 if(strcmp(choose,"ac")==0) /*回收*/
	 {
	    back=malloc(sizeof(RECT));
	    printf("Input Adress and Size!!\n");
	    scanf("%d%d",&back->address,&back->size);/*输入回收地址和大小*/
	    check=backcheck(head,back); /*检查*/
	    if(check==1)
	    {
	       if(tolower(way)=='f')/*首先适应算法*/
		  acceptment1(head,back); /*首先适应*/
	       else
		  acceptment2(head,back);/*最佳适应*/
	       print(head);
	    }
	 }
   }while(!strcmp(choose,"as")||!strcmp(choose,"ac"));
}
/*分配函数*/
RECT *assignment(RECT *head,int application)
{
   RECT *after,*before,*assign;
   assign=malloc(sizeof(RECT)); /*分配申请空间*/
   assign->size=application;
   assign->next=NULL;
   if(application>head->size||application<=0)
      assign->address=-1; /*申请无效*/
   else
   {
      before=head;
      after=head->next;
      while(after->size<application)/*查找适应的结点*/
      {
	 before=before->next;
	 after=after->next;
      }
      if(after->size==application) /*结点大小等于申请大小则完全分配*/
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
	 after->size=after->size-application; /*大于申请空间则截取相应大小分配*/
	 assign->address=after->address+after->size;
	 if(tolower(way)=='b')/*如果是最佳适应，将截取后剩余结点重新回收到合适位置*/
	 {
	    before->next=after->next;
	    back=after;
	    acceptment2(head,back);
	 }
      }
      if(maxblocknum==0) /*修改最大数和头结点值*/
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
   return assign1; /*返回分配给用户的地址*/
}
void acceptment1(RECT *head,RECT *back1)/*首先适应*/
{
   RECT *before,*after;
   int insert;
   before=head;
   after=head->next;
   insert=0;
   while(!insert) /*将回收区插入空闲区表*/
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
   if(back1->address==before->address+before->size)/*与上一块合并*/
   {
      before->size=before->size+back1->size;
      before->next=back1->next;
      free(back1);
      back1=before;
   }
   if(after!=NULL&&(after->address==back1->address+back1->size))
   {  /*与下一块合并*/
      back1->size=back1->size+after->size;
      back1->next=after->next;
      free(after);
   }
   if(head->size<back1->size) /*修改最大块值和最大块个数*/
   {
      head->size=back1->size;
      maxblocknum=1;
   }
   else
      if(head->size==back1->size)
	 maxblocknum++;
}
/*最佳适应，back1为回收结点的地址*/
void acceptment2(RECT *head,RECT *back1)
{
   RECT *before,*after;
   int insert ;
   insert=0;
   before=head;
   after=head->next;
   if(head->next==NULL) /*如果可利用区表为空*/
   {
      head->size=back1->size;
      head->next=back1;
      maxblocknum++;
      back1->next=NULL;
   }
   else
   {
      while(after!=NULL) /*与上一块合并*/
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
      if(after->address==back1->size+back1->address) /*与下一块合并*/
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
      before=head;/*将回收结点插入到合适的位置*/
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
      if(head->size<back1->size) /*修改最大块值和最大块数*/
      {
	 head->size=back1->size;
	 maxblocknum++;
      }
      else
	 if(head->size==back1->size)
	    maxblocknum++;
   }
}

void print(RECT *head) /*输出链表*/
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
/*检查回收块的合法性，back1为要回收的结点地址*/
int backcheck(RECT *head,RECT *back1)
{
   RECT *before,*after;
   int check=1;
   if(back1->address<0||back1->size<0)
      check=0;/*地址和大小不能为负*/
   before=head->next;
   while((before!=NULL)&&check)/*地址不能和空闲区表中结点出现重叠*/
      if(((back1->address<before->address)
	 &&(back1->address+back1->size>before->address))
	 ||((back1->address>=before->address)
	&&(back1->address<before->address+before->size)))
	 check=0;
      else
	 before=before->next;
   if(check==0)
      printf("Error input!!\n");
   return check;  /*返回检查结果*/
}
