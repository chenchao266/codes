
#include<stdio.h> 
#include<stdlib.h> 
#include<malloc.h> 
#define LEN sizeof(struct scorenode)
#define DEBUG
#include<string.h> 
struct scorenode
{int number;/*ѧ��*/
char name[10];/*����*/
float yuwen;/*���ĳɼ�*/
float yingyu;/*Ӣ��ɼ�*/
float shuxue;/*��ѧ�ɼ� */
struct scorenode *next;
};
typedef struct scorenode score;
int n,k;/*n,kΪȫ�ֱ������������еĺ���������ʹ����*/
/*==============================================================================================*/
score *creat2311(void)
/*���ܣ����������˺�������һ��ָ������ͷ��ָ��*/
{
	score*head;
	score *p1,*p2,*p3,*max;
	int i,j;
	float fen;
	char t[10];
	n=0;
	p1=p2=p3=(score *)malloc(LEN);head=p3; 
	printf("Please input resaurce print 0exit!\n");
repeat1: printf("Please input NO��(>0)");
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
				 {printf("ѧ���ظ�,������!\n");
				 goto repeat1;
				 
				 }
			 } 
			 } 
		 }
		 printf("Please input name:");
		 scanf("%s",&p1->name);
		 printf("Please input chinese score(0~100)��");
		 scanf("%f",&p1->yuwen);
		 while(p1->yuwen<0||p1->yuwen>100)
		 {getchar();
		 printf("error,Please input again!");
		 scanf("%f",&p1->yuwen);}
		 printf("Please inputӢ��ɼ�(0~100)��");
		 scanf("%f",&p1->yingyu);
		 while(p1->yingyu<0||p1->yingyu>100)
		 {getchar();
		 printf("�����������������Ӣ��ɼ�");
		 scanf("%f",&p1->yingyu);}
		 printf("Please input��ѧ�ɼ�(0~100)��");
		 scanf("%f",&p1->shuxue);
		 while(p1->shuxue<0||p1->shuxue>100)
		 {getchar();
		 printf("�������������������ѧ�ɼ�");
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
			 printf("Please inputѧ�����ϣ���0�˳�!\n");
repeat2:printf("Please inputѧ��ѧ��(ѧ��Ӧ����0)��");
		scanf("%d",&p1->number);
		
		while(p1->number<0)
		{getchar();
		printf("�������,����������ѧ��ѧ��:");
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
			{printf("ѧ���ظ�,������!\n");
			goto repeat2;
			
			}
			} 
			} 
			
		}
		printf("Please inputѧ��������");
		scanf("%s",&p1->name);
		printf("Please input���ĳɼ�(0~100)��");
		scanf("%f",&p1->yuwen);
		while(p1->yuwen<0||p1->yuwen>100)
		{getchar();
		printf("��������������������ĳɼ�");
		scanf("%f",&p1->yuwen);}
		
		printf("Please inputӢ��ɼ�(0~100)��");
		scanf("%f",&p1->yingyu);
		while(p1->yingyu<0||p1->yingyu>100)
		{getchar();
		printf("�����������������Ӣ��ɼ�");
		scanf("%f",&p1->yingyu);}
		printf("Please input��ѧ�ɼ�(0~100)��");
		scanf("%f",&p1->shuxue);
		while(p1->shuxue<0||p1->shuxue>100)
		{getchar();
		printf("�������������������ѧ�ɼ�");
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
	 printf("�����ѧ����Ϊ:%d��!\n",n);
	 return(head);
                                }
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                score *load2311(score *head)
									/*����:���ļ�����ѧ����¼*/
                                { score *p1,*p2;
                                int m=0;
                                char filepn[10];
                                FILE *fp;
								
                                printf("Please input�ļ�·�����ļ���:");
                                scanf("%s",filepn);
                                if((fp=fopen(filepn,"r+"))==NULL)
                                {
									printf("���ܴ��ļ�!\n");
									return 0;
                                }
                                fscanf(fp," ���Գɼ�����ϵͳ\n");
                                fscanf(fp,"����:happyyangxu �༶��010911 ѧ�ţ�28 \n");
                                fscanf(fp,"-----------------------------------------\n");
                                fscanf(fp,"|ѧ��\t|����\t|����\t|Ӣ��\t|��ѧ\t|\n");
                                fscanf(fp,"-----------------------------------------\n");
                                printf(" ���Գɼ�����ϵͳ \n");
                                printf(" ����:happyyangxu �༶��010911 ѧ�ţ�28 \n");
                                printf("-----------------------------------------\n");
                                printf("|ѧ��\t|����\t|����\t|Ӣ��\t|��ѧ\t|\n");
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
									/*���ܣ�׷��ѧ������,���ҽ�����ѧ�����ϰ�ѧ������*/
                                {
									score *p0,*p1,*p2,*p3,*max;
									int i,j;
									float fen;
									char t[10];
									p3=stu=(score *)malloc(LEN);
									printf("\n����Ҫ���ӵ�ѧ��������!");
repeat4: printf("Please inputѧ��ѧ��(ѧ��Ӧ����0)��");
		 scanf("%d",&stu->number); 
		 
		 while(stu->number<0)
		 {getchar();
		 printf("�����������������ѧ��ѧ��:");
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
			 {printf("ѧ���ظ�,������!\n");
			 goto repeat4;
			 
			 }
			 } 
			 } 
		 }
		 
		 /******************************************************/ 
		 
		 printf("����ѧ��������");
		 scanf("%s",stu->name); 
		 printf("Please input���ĳɼ�(0~100)��");
		 scanf("%f",&stu->yuwen); 
		 while(stu->yuwen<0||stu->yuwen>100)
		 {getchar();
		 printf("��������������������ĳɼ�");
		 scanf("%f",&stu->yuwen);} 
		 printf("Please inputӢ��ɼ�(0~100)��");
		 scanf("%f",&stu->yingyu);
		 while(stu->yingyu<0||stu->yingyu>100)
		 {getchar();
		 printf("�����������������Ӣ��ɼ�");
		 scanf("%f",&stu->yingyu);}
		 printf("Please input��ѧ�ɼ�(0~100)��");
		 scanf("%f",&stu->shuxue);
		 while(stu->shuxue<0||stu->shuxue>100)
		 {getchar();
		 printf("�������������������ѧ�ɼ�");
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
		 printf("���ڵ�ѧ����Ϊ:%d��!\n",n);
		 return(head);
                                }
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                score *search2311(score *head)
									/*���ܣ���ѯѧ���ɼ�*/
                                {int number;
                                score *p1,*p2;
                                printf("����Ҫ��ѯ��ѧ����ѧ��,");
                                scanf("%d",&number);
                                while(number!=0)
                                {
									if(head==NULL)
									{printf("\nû���κ�ѧ������!\n");return(head);}
									printf("-----------------------------------------\n");
									printf("|ѧ��\t|����\t|����\t|Ӣ��\t|��ѧ\t|\n");
									printf("-----------------------------------------\n");
									p1=head;
									while(number!=p1->number&&p1->next!=NULL)
									{p2=p1;p1=p1->next;} 
									if(number==p1->number) 
									{printf("|%dt|%st|%.1ft|%.1ft|%.1ft|\n",p1->number,p1->name,p1->yuwen,p1->yingyu,p1->shuxue);
									printf("-----------------------------------------\n");}
									else 
										printf("%d�����ڴ�ѧ��!\n",number);
									
									printf("����Ҫ��ѯ��ѧ����ѧ��,");
									scanf("%d",&number);
                                }
                                printf("�Ѿ��˳���!\n");
                                return(head);}
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                score *del2311(score *head)
                                {
									score *p1,*p2;
									int number;
									printf("����Ҫɾ����ѧ����ѧ��(����0ʱ�˳�):");
									scanf("%d",&number);
									getchar();
									while(number!=0)/*����ѧ��Ϊ0ʱ�˳�*/
									{
										
										if(head==NULL)
										{
											printf("\nû���κ�ѧ������!\n");
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
											
											printf("ɾ��:%d\n",number);n=n-1;
										}
										else
											printf("%d�����ڴ�ѧ��!\n",number);
										
										
										
										printf("����Ҫɾ����ѧ����ѧ��:");
										scanf("%d",&number);
										getchar();
									}
#ifdef DEBUG 
									printf("�Ѿ��˳���!\n");
#endif
									printf("���ڵ�ѧ����Ϊ:%d��!\n",n);
									return(head);
                                } 
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                void print2311(score *head)
									/*���ܣ���ʾѧ���ɼ�*/
                                {
									score *p;
									if(head==NULL) 
									{printf("\nû���κ�ѧ������!\n");}
									else
									{printf("%d\n",n);
									printf("-----------------------------------------\n");
									printf("|ѧ��\t|����\t|����\t|Ӣ��\t|��ѧ\t|\n");
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
									/*���ܣ�ͳ��ѧ���ɼ�*/
                                {
									float 
										sum1=0,sum2=0,sum3=0,ave1=0,ave2=0,ave3=0,max=0,min=0;
									score *p;
									int x,y=0,i=0;
									p=head;
									printf("1�����ֺܷ�ƽ����\t2����ƽ����\t3�ܷ���߷�\t4�ܷ���ͷ�\n");
									scanf("%d",&x);
									/*getchar();*/
									switch(x)
										
									{
									case 1: if(head==NULL)
											{printf("\nû���κ�ѧ������!\n");return(head);}
										else
										{
											printf("---------------------------------------------------------\n");
											printf("|ѧ��\t|����\t|����\t|Ӣ��\t|��ѧ\t|�ܷ�\t|ƽ����\t|\n");
											printf("---------------------------------------------------------\n");
											while(p!=NULL)
											{
												sum1=p->yuwen+p->yingyu+p->shuxue; /*��������ܷ�*/ 
												ave1=sum1/3;/*�������ƽ����*/
												
												printf("|%d\t|%s\t|%.1f\t|%.1f\t|%.1f\t|%.1f\t|%.1f\t|\n",p->number,p->name,p->yuwen,p->yingyu,p->shuxue,sum1,ave1);
												
												printf("---------------------------------------------------------\n");
												p=p->next;}
										}
										return(head);
									case 2:
										if(head==NULL)
										{printf("\nû���κ�ѧ������!\n");return(head);}
										while(p!=NULL)
										{
											sum1=sum1+p->yuwen;
											sum2=sum2+p->yingyu;
											sum3=sum3+p->shuxue;/*�����ܷ�*/
											y=y+1;
											ave1=sum1/y;
											ave2=sum2/y;
											ave3=sum3/y;/*����ƽ����*/
											p=p->next;
										}
										printf("����ƽ������%.1f\n",ave1);
										printf("Ӣ��ƽ������%.1f\n",ave2);
										printf("��ѧƽ������%.1f\n",ave3);
										return(head);
									case 3: 
										if(head==NULL)
										{printf("\nû���κ�ѧ������!\n");return(head);}
										max=p->yuwen+p->yingyu+p->shuxue;
										while(i<n) {
											i=i+1;
											sum1=p->yuwen+p->yingyu+p->shuxue; /*��������ܷ�*/
											if(max<sum1)
												max=sum1;
											p=p->next;
										}
										
										printf("�ܷ���߷�:%.1f",max);
										printf("\n");
										return(head);
										
										
									case 4: if(head==NULL)
											{printf("\nû���κ�ѧ������!\n");return(head);}
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
										
										printf("�ܷ���ͷ�:%.1f",min);
										printf("\n");
										return(head);
										
									default :printf("�������,������!\n");
									}
									return(head);
									
                                }
								
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                save2311(score *p1) 
                                { 
									FILE *fp;
									
									char filepn[20];
									
									printf("Please input�ļ�·�����ļ���:");
									scanf("%s",filepn);
									if((fp=fopen(filepn,"w+"))==NULL)
									{
										printf("���ܴ��ļ�!\n");
										return 0;
									}
									fprintf(fp," ���Գɼ�����ϵͳ \n");
									fprintf(fp," ����:happyyangxu �༶��010911 ѧ�ţ�28 \n");
									fprintf(fp,"-----------------------------------------\n");
									fprintf(fp,"|ѧ��\t|����\t|����\t|Ӣ��\t|��ѧ\t|\n");
									fprintf(fp,"-----------------------------------------\n");
									
									while(p1!=NULL)
									{
										fprintf(fp,"%d\t%s\t%.1f\t%.1f\t%.1f\t\n",p1->number,p1->name,p1->yuwen,p1->yingyu,p1->shuxue);
										
										p1=p1->next;/*����һ�����*/
										
									}
									fclose(fp);
									printf("�ļ��Ѿ�����!\n");
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
                                {printf("\nû���κ�ѧ�����ϣ����Ƚ�������!\n");return(head);}/*����Ϊ��*/
                                max=p=head;
								
                                for(i=0;i<80;i++)
									printf("*");
                                printf("1��ѧ��ѧ������\t2��ѧ����������\t3�����ĳɼ�����\n");
                                printf("4��Ӣ��ɼ�����\t5����ѧ�ɼ�����tn");
                                for(i=0;i<80;i++)
									printf("*");
								
                                printf("��ѡ�����:");
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
									printf("�������,������! \n");
                                }
								return (0);
                                }
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                int menu2311(void)/*����menu2311,���ܣ��˵�ѡ�����*/
                                {
									int i,k;

									printf("\t\t\t\t���Գɼ�����ϵͳ\n");
									printf("\t\t\t����:504 �༶:010911 ѧ��:25\n");
									for(i=0;i<80;i++)
										printf("*");
									printf("1����ѧ��������\t\t\t2���ļ���������\t\t\t3��ѯѧ���ĳɼ�\n");
									printf("4ɾ��ѧ��������\t\t\t5׷��ѧ��������\t\t\t6��ʾѧ���ĳɼ�\n"); 
									printf("7ͳ��ѧ���ĳɼ�\t\t\t8�������������\t\t\t9�Գɼ���������\n");
									/*�˵�ѡ�����*/ 
									for(i=0;i<80;i++)
										printf("*");
									printf("��ӭ����ɼ�����ϵͳ����ѡ������Ҫ�Ĳ���(ѡ��(0)�˳�):");
									scanf("%d",&k);/*ѡ�����*/
									/*getchar();*/
									
									return (k);
                                }
								
                                /*==============================================================================================*/
                                /*==============================================================================================*/
                                void main() /*������main*/
                                {
									score *head=0,*stu=0;
									while(1)
									{
									k=menu2311();
									switch(k)/*��switch���ʵ�ֹ���ѡ��*/
									{case 1: head=creat2311();break;/*���ô���������*/
									case 2: head=load2311(head);break;/*���ļ������¼����*/
									case 3: head=search2311(head);break;/*���óɼ���ѯ����*/ 
										
									case 4: head=del2311(head); break;/*����ɾ��ѧ�����Ϻ���*/
									case 5: 
										head=add2311(head,stu);break;/*����׷��ѧ�����Ϻ���*/
									case 6: print2311(head); break;/*������ʾѧ�����Ϻ���*/
									case 7: statistics2311(head); break;/*����ͳ�ƺ���*/
									case 8: save2311(head);break;/*���ñ��溯��*/
									case 9: taxis2311(head);break;/*����������*/ 
									case 0: exit(0);/*�˳�ϵͳ������������*/
									default: printf("�������,������!\n"); }
									}
                                }
								