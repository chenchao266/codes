
#include<stdio.h> 
#include<stdlib.h> 
#include<malloc.h> 
#define LEN sizeof(struct scorenode)
#define DEBUG
#include<string.h> 
struct scorenode
{int number;/*学号*/
char name[10];/*姓名*/
float yuwen;/*语文成绩*/
float yingyu;/*英语成绩*/
float shuxue;/*数学成绩 */
struct scorenode *next;
};
typedef struct scorenode score;
int n,k;/*n,k为全局变量，本程序中的函数均可以使用它*/
/*==============================================================================================*/
score *creat2311(void)
/*功能：创建链表，此函数带回一个指向链表头的指针*/
{
	score*head;
	score *p1,*p2,*p3,*max;
	int i,j;
	float fen;
	char t[10];
	n=0;
	p1=p2=p3=(score *)malloc(LEN);head=p3; 
	printf("Please input resaurce print 0exit!\n");
repeat1: printf("Please input NO：(>0)");
		 scanf("%d",&p1->number);
		 while(p1->number<0)
		 {getchar();
		 printf("error,Please input again!");
		 scanf("%d",&p1->number);} 
		 
		 if(p1->number==0)
			 goto end;
		 else 
		 { 
			 p3=head;
			 if(n>0)
			 {for(i=1;i<n;i++){
				 if(p1->number!=p3->number)
					 p3=p3->next;
				 else 
				 {printf("学号重复,请重输!\n");
				 goto repeat1;
				 
				 }
			 } 
			 } 
		 }
		 printf("Please input name:");
		 scanf("%s",&p1->name);
		 printf("Please input chinese score(0~100)：");
		 scanf("%f",&p1->yuwen);
		 while(p1->yuwen<0||p1->yuwen>100)
		 {getchar();
		 printf("error,Please input again!");
		 scanf("%f",&p1->yuwen);}
		 printf("Please input英语成绩(0~100)：");
		 scanf("%f",&p1->yingyu);
		 while(p1->yingyu<0||p1->yingyu>100)
		 {getchar();
		 printf("输入错误，请重新输入英语成绩");
		 scanf("%f",&p1->yingyu);}
		 printf("Please input数学成绩(0~100)：");
		 scanf("%f",&p1->shuxue);
		 while(p1->shuxue<0||p1->shuxue>100)
		 {getchar();
		 printf("输入错误，请重新输入数学成绩");
		 scanf("%f",&p1->shuxue);}
		 head=NULL;
		 while(p1->number!=0)
		 {
			 n=n+1;
			 if(n==1) 
				 head=p1;
			 else
				 
				 p2->next=p1;
			 p2=p1;
			 p1=(score *)malloc(LEN);
			 printf("Please input学生资料，输0退出!\n");
repeat2:printf("Please input学生学号(学号应大于0)：");
		scanf("%d",&p1->number);
		
		while(p1->number<0)
		{getchar();
		printf("输入错误,请重新输入学生学号:");
		scanf("%d",&p1->number);} 
		
		if(p1->number==0)
			goto end;
		else
		{
			p3=head;
			if(n>0)
			{for(i=1;i<n;i++)
			{if(p1->number!=p3->number)
			p3=p3->next;
			else 
			{printf("学号重复,请重输!\n");
			goto repeat2;
			
			}
			} 
			} 
			
		}
		printf("Please input学生姓名：");
		scanf("%s",&p1->name);
		printf("Please input语文成绩(0~100)：");
		scanf("%f",&p1->yuwen);
		while(p1->yuwen<0||p1->yuwen>100)
		{getchar();
		printf("输入错误，请重新输入语文成绩");
		scanf("%f",&p1->yuwen);}
		
		printf("Please input英语成绩(0~100)：");
		scanf("%f",&p1->yingyu);
		while(p1->yingyu<0||p1->yingyu>100)
		{getchar();
		printf("输入错误，请重新输入英语成绩");
		scanf("%f",&p1->yingyu);}
		printf("Please input数学成绩(0~100)：");
		scanf("%f",&p1->shuxue);
		while(p1->shuxue<0||p1->shuxue>100)
		{getchar();
		printf("输入错误，请重新输入数学成绩");
		scanf("%f",&p1->shuxue);}
		
		 }
end: p1=head;
	 p3=p1;
	 for(i=1;i<n;i++){
		 for(j=i+1;j<=n;j++)
		 {
			 max=p1;
			 p1=p1->next;
			 
			 if(max->number>p1->number)
			 {
				 k=max->number;
				 max->number=p1->number;
				 p1->number=k;
				 
				 
				 strcpy(t,max->name);
				 strcpy(max->name,p1->name);
				 strcpy(p1->name,t);
				 
				 fen=max->yuwen;
				 max->yuwen=p1->yuwen;
				 p1->yuwen=fen;
				 
				 
				 fen=max->yingyu;
				 max->yingyu=p1->yingyu;
				 p1->yingyu=fen;
				 
				 
				 fen=max->shuxue;
				 max->shuxue=p1->shuxue;
				 p1->shuxue=fen;
				 
			 }
		 }
		 max=head;p1=head;
	 } 
	 p2->next=NULL;
	 printf("输入的学生数为:%d个!\n",n);
	 return(head);
                                }
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                score *load2311(score *head)
									/*功能:从文件读入学生记录*/
                                { score *p1,*p2;
                                int m=0;
                                char filepn[10];
                                FILE *fp;
								
                                printf("Please input文件路径及文件名:");
                                scanf("%s",filepn);
                                if((fp=fopen(filepn,"r+"))==NULL)
                                {
									printf("不能打开文件!\n");
									return 0;
                                }
                                fscanf(fp," 考试成绩管理系统\n");
                                fscanf(fp,"作者:happyyangxu 班级：010911 学号：28 \n");
                                fscanf(fp,"-----------------------------------------\n");
                                fscanf(fp,"|学号\t|姓名\t|语文\t|英语\t|数学\t|\n");
                                fscanf(fp,"-----------------------------------------\n");
                                printf(" 考试成绩管理系统 \n");
                                printf(" 作者:happyyangxu 班级：010911 学号：28 \n");
                                printf("-----------------------------------------\n");
                                printf("|学号\t|姓名\t|语文\t|英语\t|数学\t|\n");
                                printf("-----------------------------------------\n");
                                m=m+1;
                                if(m==1)
                                {
									
									p1=(score *)malloc(LEN);
									fscanf(fp,"%d%s%f%f%f",&p1->number,p1->name,&p1->yuwen,&p1->yingyu,&p1->shuxue);
									printf("|%dt|%st|%.1ft|%.1ft|%.1ft|\n",p1->number,p1->name,p1->yuwen,p1->yingyu,p1->shuxue);
									
									head=NULL;
									do
									{
										n=n+1;
										if(n==1) head=p1;
										else p2->next=p1;
										p2=p1;
										p1=(score *)malloc(LEN);
										fscanf(fp,"%d%s%f%f%fn",&p1->number,p1->name,&p1->yuwen,&p1->yingyu,&p1->shuxue);
										printf("|%dt|%st|%.1ft|%.1ft|%.1ft|\n",p1->number,p1->name,p1->yuwen,p1->yingyu,p1->shuxue);
										
									}while(!feof(fp));
									p2->next=p1;
									p1->next=NULL;
									n=n+1; 
                                }printf("-----------------------------------------\n");
                                fclose(fp);
								
                                return (head);
                                }
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                score *add2311(score *head,score *stu)
									/*功能：追加学生资料,并且将所有学生资料按学号排序*/
                                {
									score *p0,*p1,*p2,*p3,*max;
									int i,j;
									float fen;
									char t[10];
									p3=stu=(score *)malloc(LEN);
									printf("\n输入要增加的学生的资料!");
repeat4: printf("Please input学生学号(学号应大于0)：");
		 scanf("%d",&stu->number); 
		 
		 while(stu->number<0)
		 {getchar();
		 printf("输入错误，请重新输入学生学号:");
		 scanf("%d",&stu->number);}
		 /******************************************************/
		 if(stu->number==0)
			 goto end2;
		 else 
		 { 
			 p3=head;
			 if(n>0)
			 {for(i=1;i<n;i++)
			 {if(stu->number!=p3->number)
			 p3=p3->next;
			 else 
			 {printf("学号重复,请重输!\n");
			 goto repeat4;
			 
			 }
			 } 
			 } 
		 }
		 
		 /******************************************************/ 
		 
		 printf("输入学生姓名：");
		 scanf("%s",stu->name); 
		 printf("Please input语文成绩(0~100)：");
		 scanf("%f",&stu->yuwen); 
		 while(stu->yuwen<0||stu->yuwen>100)
		 {getchar();
		 printf("输入错误，请重新输入语文成绩");
		 scanf("%f",&stu->yuwen);} 
		 printf("Please input英语成绩(0~100)：");
		 scanf("%f",&stu->yingyu);
		 while(stu->yingyu<0||stu->yingyu>100)
		 {getchar();
		 printf("输入错误，请重新输入英语成绩");
		 scanf("%f",&stu->yingyu);}
		 printf("Please input数学成绩(0~100)：");
		 scanf("%f",&stu->shuxue);
		 while(stu->shuxue<0||stu->shuxue>100)
		 {getchar();
		 printf("输入错误，请重新输入数学成绩");
		 scanf("%f",&stu->shuxue);}
		 p1=head;
		 p0=stu;
		 if(head==NULL)
		 {head=p0;p0->next=NULL;}
		 else
		 {
			 if(p1->next==NULL)
			 {
				 p1->next=p0;
				 p0->next=NULL;
			 }
			 else
			 {
				 while(p1->next!=NULL)
				 {
					 p2=p1;p1=p1->next;
				 }
				 p1->next=p0;
				 p0->next=NULL;
			 }
		 }
		 n=n+1;
		 
		 p1=head;
		 p0=stu;
		 for(i=1;i<n;i++){
			 for(j=i+1;j<=n;j++)
			 {
				 max=p1;
				 p1=p1->next;
				 
				 if(max->number>p1->number)
				 {
					 k=max->number;
					 max->number=p1->number;
					 p1->number=k;
					 
					 
					 strcpy(t,max->name);
					 strcpy(max->name,p1->name);
					 strcpy(p1->name,t);
					 
					 fen=max->yuwen;
					 max->yuwen=p1->yuwen;
					 p1->yuwen=fen;
					 
					 
					 fen=max->yingyu;
					 max->yingyu=p1->yingyu;
					 p1->yingyu=fen;
					 
					 
					 fen=max->shuxue;
					 max->shuxue=p1->shuxue;
					 p1->shuxue=fen;
					 
				 }
			 }
			 max=head;p1=head;
		 }
end2:
		 printf("现在的学生数为:%d个!\n",n);
		 return(head);
                                }
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                score *search2311(score *head)
									/*功能：查询学生成绩*/
                                {int number;
                                score *p1,*p2;
                                printf("输入要查询的学生的学号,");
                                scanf("%d",&number);
                                while(number!=0)
                                {
									if(head==NULL)
									{printf("\n没有任何学生资料!\n");return(head);}
									printf("-----------------------------------------\n");
									printf("|学号\t|姓名\t|语文\t|英语\t|数学\t|\n");
									printf("-----------------------------------------\n");
									p1=head;
									while(number!=p1->number&&p1->next!=NULL)
									{p2=p1;p1=p1->next;} 
									if(number==p1->number) 
									{printf("|%dt|%st|%.1ft|%.1ft|%.1ft|\n",p1->number,p1->name,p1->yuwen,p1->yingyu,p1->shuxue);
									printf("-----------------------------------------\n");}
									else 
										printf("%d不存在此学生!\n",number);
									
									printf("输入要查询的学生的学号,");
									scanf("%d",&number);
                                }
                                printf("已经退出了!\n");
                                return(head);}
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                score *del2311(score *head)
                                {
									score *p1,*p2;
									int number;
									printf("输入要删除的学生的学号(输入0时退出):");
									scanf("%d",&number);
									getchar();
									while(number!=0)/*输入学号为0时退出*/
									{
										
										if(head==NULL)
										{
											printf("\n没有任何学生资料!\n");
											return(head);
										}
										
										p1=head;
										
										while(number!=p1->number&&p1->next!=NULL)
											
										{
											p2=p1;p1=p1->next;
										} 
										
										if(number==p1->number) 
											
										{
											if(p1==head)
												head=p1->next;
											
											else 
												p2->next=p1->next;
											
											printf("删除:%d\n",number);n=n-1;
										}
										else
											printf("%d不存在此学生!\n",number);
										
										
										
										printf("输入要删除的学生的学号:");
										scanf("%d",&number);
										getchar();
									}
#ifdef DEBUG 
									printf("已经退出了!\n");
#endif
									printf("现在的学生数为:%d个!\n",n);
									return(head);
                                } 
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                void print2311(score *head)
									/*功能：显示学生成绩*/
                                {
									score *p;
									if(head==NULL) 
									{printf("\n没有任何学生资料!\n");}
									else
									{printf("%d\n",n);
									printf("-----------------------------------------\n");
									printf("|学号\t|姓名\t|语文\t|英语\t|数学\t|\n");
									printf("-----------------------------------------\n");
									p=head;
									do
									{printf("|%d\t|%s\t|%.1f\t|%.1f\t|%.1f\t|\n",p->number,p->name,p->yuwen,p->yingyu,p->shuxue);
									printf("-----------------------------------------\n");
									p=p->next;}while (p!=NULL);
									}
                                }
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                score *statistics2311(score *head)
									/*功能：统计学生成绩*/
                                {
									float 
										sum1=0,sum2=0,sum3=0,ave1=0,ave2=0,ave3=0,max=0,min=0;
									score *p;
									int x,y=0,i=0;
									p=head;
									printf("1个人总分和平均分\t2单科平均分\t3总分最高分\t4总分最低分\n");
									scanf("%d",&x);
									/*getchar();*/
									switch(x)
										
									{
									case 1: if(head==NULL)
											{printf("\n没有任何学生资料!\n");return(head);}
										else
										{
											printf("---------------------------------------------------------\n");
											printf("|学号\t|姓名\t|语文\t|英语\t|数学\t|总分\t|平均分\t|\n");
											printf("---------------------------------------------------------\n");
											while(p!=NULL)
											{
												sum1=p->yuwen+p->yingyu+p->shuxue; /*计算个人总分*/ 
												ave1=sum1/3;/*计算个人平均分*/
												
												printf("|%d\t|%s\t|%.1f\t|%.1f\t|%.1f\t|%.1f\t|%.1f\t|\n",p->number,p->name,p->yuwen,p->yingyu,p->shuxue,sum1,ave1);
												
												printf("---------------------------------------------------------\n");
												p=p->next;}
										}
										return(head);
									case 2:
										if(head==NULL)
										{printf("\n没有任何学生资料!\n");return(head);}
										while(p!=NULL)
										{
											sum1=sum1+p->yuwen;
											sum2=sum2+p->yingyu;
											sum3=sum3+p->shuxue;/*计算总分*/
											y=y+1;
											ave1=sum1/y;
											ave2=sum2/y;
											ave3=sum3/y;/*计算平均分*/
											p=p->next;
										}
										printf("语文平均分是%.1f\n",ave1);
										printf("英语平均分是%.1f\n",ave2);
										printf("数学平均分是%.1f\n",ave3);
										return(head);
									case 3: 
										if(head==NULL)
										{printf("\n没有任何学生资料!\n");return(head);}
										max=p->yuwen+p->yingyu+p->shuxue;
										while(i<n) {
											i=i+1;
											sum1=p->yuwen+p->yingyu+p->shuxue; /*计算个人总分*/
											if(max<sum1)
												max=sum1;
											p=p->next;
										}
										
										printf("总分最高分:%.1f",max);
										printf("\n");
										return(head);
										
										
									case 4: if(head==NULL)
											{printf("\n没有任何学生资料!\n");return(head);}
										while(p!=NULL)
										{
											min=p->yuwen+p->yingyu+p->shuxue;
											while(p!=NULL)
											{sum2=p->yuwen+p->yingyu+p->shuxue; 
											
											if(min>sum2)
												min=sum2;
											p=p->next;
											}
										}
										
										printf("总分最低分:%.1f",min);
										printf("\n");
										return(head);
										
									default :printf("输入错误,请重试!\n");
									}
									return(head);
									
                                }
								
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                save2311(score *p1) 
                                { 
									FILE *fp;
									
									char filepn[20];
									
									printf("Please input文件路径及文件名:");
									scanf("%s",filepn);
									if((fp=fopen(filepn,"w+"))==NULL)
									{
										printf("不能打开文件!\n");
										return 0;
									}
									fprintf(fp," 考试成绩管理系统 \n");
									fprintf(fp," 作者:happyyangxu 班级：010911 学号：28 \n");
									fprintf(fp,"-----------------------------------------\n");
									fprintf(fp,"|学号\t|姓名\t|语文\t|英语\t|数学\t|\n");
									fprintf(fp,"-----------------------------------------\n");
									
									while(p1!=NULL)
									{
										fprintf(fp,"%d\t%s\t%.1f\t%.1f\t%.1f\t\n",p1->number,p1->name,p1->yuwen,p1->yingyu,p1->shuxue);
										
										p1=p1->next;/*下移一个结点*/
										
									}
									fclose(fp);
									printf("文件已经保存!\n");
									return 0;
                                }
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                score *taxis2311(score *head) 
									
                                { score *p,*max;
                                int i,j,x;
							
                                float fen;
                                char t[10];
                                if(head==NULL)
                                {printf("\n没有任何学生资料，请先建立链表!\n");return(head);}/*链表为空*/
                                max=p=head;
								
                                for(i=0;i<80;i++)
									printf("*");
                                printf("1按学生学号排序\t2按学生姓名排序\t3按语文成绩排序\n");
                                printf("4按英语成绩排序\t5按数学成绩排序tn");
                                for(i=0;i<80;i++)
									printf("*");
								
                                printf("请选择操作:");
                                scanf("%d",&x);
                                /*getchar();*/
                                switch(x)
                                {case 1: 
                                for(i=1;i<n;i++) {
									for(j=i+1;j<=n;j++)
									{
										max=p;
										p=p->next;
										
										if(max->number>p->number)
										{
											k=max->number;
											max->number=p->number;
											p->number=k;
											
											
											strcpy(t,max->name);
											strcpy(max->name,p->name);
											strcpy(p->name,t);
											
											fen=max->yuwen;
											max->yuwen=p->yuwen;
											p->yuwen=fen;
											
											
											fen=max->yingyu;
											max->yingyu=p->yingyu;
											p->yingyu=fen;
											
											
											fen=max->shuxue;
											max->shuxue=p->shuxue;
											p->shuxue=fen;
											
										}
									}
									max=head;p=head;
                                }
								print2311(head);
								break;
                                case 2: 
									for(i=1;i<n;i++) {
										for(j=i+1;j<=n;j++)
										{
											max=p;
											p=p->next;
											if(strcmp(max->name,p->name)>0)
											{strcpy(t,max->name);
											strcpy(max->name,p->name);
											strcpy(p->name,t);
											
											k=max->number;
											max->number=p->number;
											p->number=k;
											fen=max->yuwen;
											max->yuwen=p->yuwen;
											p->yuwen=fen;
											
											fen=max->yingyu;
											max->yingyu=p->yingyu;
											p->yingyu=fen;
											
											fen=max->shuxue;
											max->shuxue=p->shuxue;
											p->shuxue=fen;
											
										}
									}
									p=head;
									max=head;
                                }
                                print2311(head);
                                break;
								case 3:
									for(i=1;i<n;i++){
										for(j=i+1;j<=n;j++)
										{max=p;
										p=p->next;
										if(max->yuwen>p->yuwen)
										{
											fen=max->yuwen;
											max->yuwen=p->yuwen;
											p->yuwen=fen;
											
											
											k=max->number;
											max->number=p->number;
											p->number=k; 
											
											strcpy(t,max->name);
											strcpy(max->name,p->name);
											strcpy(p->name,t);
											
											fen=max->yingyu;
											max->yingyu=p->yingyu;
											p->yingyu=fen;
											
											fen=max->shuxue;
											max->shuxue=p->shuxue;
											p->shuxue=fen;
											
										}
										}
										p=head;
										max=head;
									}
									print2311(head);
									break;
								case 4:
									for(i=1;i<n;i++){
										for(j=i+1;j<=n;j++)
										{max=p;
										p=p->next;
										if(max->yingyu>p->yingyu)
										{
											fen=max->yingyu;
											max->yingyu=p->yingyu;
											p->yingyu=fen;
											
											
											k=max->number;
											max->number=p->number;
											p->number=k;
											
											strcpy(t,max->name);
											strcpy(max->name,p->name);
											strcpy(p->name,t);
											
											fen=max->yuwen;
											max->yuwen=p->yuwen;
											p->yuwen=fen;
											
											fen=max->shuxue;
											max->shuxue=p->shuxue;
											p->shuxue=fen;
										}
										}
										p=head;
										max=head;
									}
									print2311(head);
									break;
                                case 5:
									for(i=1;i<n;i++)
									{for(j=i+1;j<=n;j++)
									{max=p;
									p=p->next;
									if(max->shuxue>p->shuxue)
									{
										fen=max->shuxue;
										max->shuxue=p->shuxue;
										p->shuxue=fen; 
										
										k=max->number;
										max->number=p->number;
										p->number=k; 
										
										strcpy(t,max->name);
										strcpy(max->name,p->name);
										strcpy(p->name,t); 
										
										fen=max->yuwen;
										max->yuwen=p->yuwen;
										p->yuwen=fen;
										
										
										fen=max->yingyu;
										max->yingyu=p->yingyu;
										p->yingyu=fen;
										
									}
									}
									p=head;
									max=head;
									}
									print2311(head);
									break;
									
                                default:
									printf("输入错误,请重试! \n");
                                }
								return (0);
                                }
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                int menu2311(void)/*函数menu2311,功能：菜单选择界面*/
                                {
									int i,k;

									printf("\t\t\t\t考试成绩管理系统\n");
									printf("\t\t\t作者:504 班级:010911 学号:25\n");
									for(i=0;i<80;i++)
										printf("*");
									printf("1输入学生的资料\t\t\t2从文件调入资料\t\t\t3查询学生的成绩\n");
									printf("4删除学生的资料\t\t\t5追加学生的资料\t\t\t6显示学生的成绩\n"); 
									printf("7统计学生的成绩\t\t\t8保存输入的资料\t\t\t9对成绩进行排序\n");
									/*菜单选择界面*/ 
									for(i=0;i<80;i++)
										printf("*");
									printf("欢迎进入成绩管理系统，请选择您所要的操作(选择(0)退出):");
									scanf("%d",&k);/*选择操作*/
									/*getchar();*/
									
									return (k);
                                }
								
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                void main() /*主函数main*/
                                {
									score *head=0,*stu=0;
									while(1)
									{
									k=menu2311();
									switch(k)/*用switch语句实现功能选择*/
									{case 1: head=creat2311();break;/*调用创建链表函数*/
									case 2: head=load2311(head);break;/*从文件调入记录函数*/
									case 3: head=search2311(head);break;/*调用成绩查询函数*/ 
										
									case 4: head=del2311(head); break;/*调用删除学生资料函数*/
									case 5: 
										head=add2311(head,stu);break;/*调用追加学生资料函数*/
									case 6: print2311(head); break;/*调用显示学生资料函数*/
									case 7: statistics2311(head); break;/*调用统计函数*/
									case 8: save2311(head);break;/*调用保存函数*/
									case 9: taxis2311(head);break;/*调用排序函数*/ 
									case 0: exit(0);/*退出系统，返回主界面*/
									default: printf("输入错误,请重试!\n"); }
									}
                                }
								