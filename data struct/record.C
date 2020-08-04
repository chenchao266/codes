/*10.3.2Դ����*/
/******ͷ�ļ���.h��***********/
#include "stdio.h"   /*I/O����*/
#include "stdlib.h" /*��׼�⺯��*/
#include "string.h"/*�ַ�������*/
#include "ctype.h" /*�ַ���������*/
#define M 50  /*���峣����ʾ��¼��*/
typedef struct /*�������ݽṹ*/
{
   char name[20]; /*����*/
   char units[30];  /*��λ*/
   char tele[10];  /*�绰*/
}ADDRESS;
/******�����Ǻ���ԭ��*******/
int enter(ADDRESS t[]); /*�����¼*/
void list(ADDRESS t[],int n); /*��ʾ��¼*/
void search(ADDRESS t[],int n); /*������������ʾ��¼*/
int delete(ADDRESS t[],int n); /*ɾ����¼*/
int  add(ADDRESS t[],int n); /*�����¼*/
void save(ADDRESS t[],int n); /*��¼����Ϊ�ļ�*/
int load(ADDRESS t[]);  /*���ļ��ж���¼*/
void display(ADDRESS t[]); /*����Ų�����ʾ��¼*/
void sort(ADDRESS t[],int n); /*����������*/
void qseek(ADDRESS t[],int n); /*���ٲ��Ҽ�¼*/
void copy();  /*�ļ�����*/
void print(ADDRESS temp); /*��ʾ������¼*/
int find(ADDRESS t[],int n,char *s) ; /*���Һ���*/
int menu_select();  /*���˵�����*/
/******��������ʼ*******/
main()
{
   int i;
   ADDRESS adr[M];  /*����ṹ������*/
   int length;  /*�����¼����*/
   clrscr();  /*����*/
   for(;;)/*����ѭ��*/
   {
      switch(menu_select())   /*�������˵�����������ֵ������������������*/
      {
	 case 0:length=enter(adr);break;/*�����¼*/
	 case 1:list(adr,length);break; /*��ʾȫ����¼*/
	 case 2:search(adr,length);break; /*���Ҽ�¼*/
	 case 3:length=delete(adr,length);break; /*ɾ����¼*/
	 case 4:length=add(adr,length);  break;   /*�����¼*/
	 case 5:save(adr,length);break; /*�����ļ�*/
	 case 6:length=load(adr); break; /*���ļ�*/
	 case 7:display(adr);break;  /*�������ʾ��¼*/
	 case 8:sort(adr,length);break; /*����������*/
	 case 9:qseek(adr,length);break; /*���ٲ��Ҽ�¼*/
	 case 10:copy();break; /*�����ļ�*/
	 case 11:exit(0); /*�緵��ֵΪ11��������*/
      }
   }
}
/*�˵���������������ֵΪ������������ѡ�Ĳ˵���*/
menu_select()
{
   char s[80];
   int c;
   gotoxy(1,25);/*����궨Ϊ�ڵ�25�У���1��*/
   printf("press any key enter menu......\n");/*��ʾѹ���������*/
   getch(); /*���������ַ�*/
   clrscr(); /*����*/
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
      printf("\n	 Enter you choice(0~11):"); /*��ʾ����ѡ��*/
      scanf("%s",s); /*����ѡ����*/
      c=atoi(s); /*��������ַ���ת��Ϊ������*/
   }while(c<0||c>11); /*ѡ�����0~11֮������*/
   return c; /*����ѡ�����������ݸ���������Ӧ�ĺ���*/
}
/***�����¼���β�Ϊ�ṹ�����飬����ֵ��������Ϊ���ͱ�ʾ��¼����*/
int  enter(ADDRESS t[])
{
   int i,n;
   char *s;
   clrscr(); /*����*/
   printf("\nplease input num \n"); /*��ʾ��Ϣ*/
   scanf("%d",&n); /*�����¼��*/
   printf("please input record \n"); /*��ʾ�����¼*/
   printf("name             unit                     telephone\n");
   printf("------------------------------------------------\n");
   for(i=0;i<n;i++)
   {
      scanf("%s%s%s",t[i].name,t[i].units,t[i].tele);  /*�����¼*/
      printf("----------------------------------------------\n");
   }
   return n;  /*���ؼ�¼����*/
}
/*��ʾ��¼������Ϊ��¼����ͼ�¼����*/
void list(ADDRESS t[],int n)
{
   int i;
   clrscr();
   printf("\n\n*******************ADDRESS******************\n");
   printf("name               unit                     telephone\n");
   printf("------------------------------------------------\n");
   for(i=0;i<n;i++)
   printf("%-20s%-30s%-10s\n",t[i].name,t[i].units,t[i].tele);
   if((i+1)%10==0)   /*�ж�����Ƿ�ﵽ10����¼*/
   {
      printf("Press any key continue...\n"); /*��ʾ��Ϣ*/
      getch();  /*ѹ���������*/
   }
   printf("************************end*******************\n");
}
/*���Ҽ�¼*/
void search(ADDRESS t[],int n)
{
   char s[20];   /*��������������ַ���*/
   int i;   /*������ҵ��������*/
   clrscr();   /*����*/
   printf("please search name\n");
   scanf("%s",s); /*�������������*/
   i=find(t,n,s); /*����find�������õ�һ������*/
   if(i>n-1)  /*�������iֵ����n-1��˵��û�ҵ�*/
      printf("not found\n");
   else
      print(t[i]);  /*�ҵ���������ʾ������ʾ��¼*/
}
/*��ʾָ����һ����¼*/
void print(ADDRESS temp)
{
   clrscr();
   printf("\n\n********************************************\n");
   printf("name                unit                      telephone\n");
   printf("------------------------------------------------\n");
   printf("%-20s%-30s%-10s\n",temp.name,temp.units,temp.tele);
   printf("**********************end***********************\n");
}
/*���Һ���������Ϊ��¼����ͼ�¼�����Լ�����s */
int find(ADDRESS t[],int n,char *s)
{
   int i;
   for(i=0;i<n;i++)/*�ӵ�һ����¼��ʼ��ֱ�����һ��*/
   {
      if(strcmp(s,t[i].name)==0)  /*��¼�е������ʹ��Ƚϵ������Ƿ����*/
      return i;   /*��ȣ��򷵻ظü�¼���±�ţ�������ǰ�����*/
   }
   return i;  /*����iֵ*/
}
/*ɾ������������Ϊ��¼����ͼ�¼����*/
int delete(ADDRESS t[],int n)
{
   char s[20];  /*Ҫɾ����¼������*/
   int ch=0;
   int i,j;
   printf("please deleted name\n"); /*��ʾ��Ϣ*/
   scanf("%s",s);/*��������*/
   i=find(t,n,s); /*����find����*/
   if(i>n-1)  /*���i>n-1����������ĳ���*/
      printf("no found not deleted\n"); /*��ʾû�ҵ�Ҫɾ���ļ�¼*/
   else
   {
      print(t[i]); /*�������������ʾ������¼��Ϣ*/
      printf("Are you sure delete it(1/0)\n");  /*ȷ���Ƿ�Ҫɾ��*/
      scanf("%d",&ch);  /*����һ������0��1*/
      if(ch==1)  /*���ȷ��ɾ������Ϊ1*/
      {
	 for(j=i+1;j<n;j++)  /*ɾ���ü�¼��ʵ�ʺ�����¼ǰ��*/
	 {
	    strcpy(t[j-1].name,t[j].name); /*����һ����¼������������ǰһ��*/
	    strcpy(t[j-1].units,t[j].units); /*����һ����¼�ĵ�λ������ǰһ��*/
	    strcpy(t[j-1].tele,t[j].tele); /*����һ����¼�ĵ绰������ǰһ��*/
	 }
	 n--;  /*��¼����1*/
      }
   }
   return n;  /*���ؼ�¼��*/
}
/*�����¼����������Ϊ�ṹ������ͼ�¼��*/
int add(ADDRESS t[],int n)/*���뺯��������Ϊ�ṹ������ͼ�¼��*/
{
   ADDRESS temp;  /*�²����¼��Ϣ*/
   int i,j;
   char s[20]; /*ȷ���������ĸ���¼֮ǰ*/
   printf("please input record\n");
   printf("************************************************\n");
   printf("name                unit                      telephone\n");
   printf("--------------------------------------------------\n");
   scanf("%s%s%s",temp.name,temp.units,temp.tele); /*���������Ϣ*/
   printf("------------------------------------------------\n");
   printf("please input locate name \n");
   scanf("%s",s); /*�������λ�õ�����*/
   i=find(t,n,s);  /*����find��ȷ������λ��*/
   for(j=n-1;j>=i;j--)   /*�����һ����㿪ʼ����ƶ�һ��*/
   {
      strcpy(t[j+1].name,t[j].name); /*��ǰ��¼��������������һ��*/
      strcpy(t[j+1].units,t[j].units); /*��ǰ��¼�ĵ�λ��������һ��*/
      strcpy(t[j+1].tele,t[j].tele); /*��ǰ��¼�ĵ绰��������һ��*/
   }
   strcpy(t[i].name,temp.name); /*���²����¼��������������i��λ��*/
   strcpy(t[i].units,temp.units); /*���²����¼�ĵ�λ��������i��λ��*/
   strcpy(t[i].tele,temp.tele); /*���²����¼�ĵ绰��������i��λ��*/
   n++;   /*��¼����1*/
   return n; /*���ؼ�¼��*/
}
/*���溯��������Ϊ�ṹ������ͼ�¼��*/
void save(ADDRESS t[],int n)
{
   int i;
   FILE *fp;  /*ָ���ļ���ָ��*/
   if((fp=fopen("record.txt","wb"))==NULL)  /*���ļ������жϴ��Ƿ�����*/
   {
      printf("can not open file\n");/*û��*/
      exit(1);  /*�˳�*/
   }
   printf("\nSaving file\n"); /*�����ʾ��Ϣ*/
   fprintf(fp,"%d",n);  /*����¼��д���ļ�*/
   fprintf(fp,"\r\n");  /*�����з���д���ļ�*/
   for(i=0;i<n;i++)
   {
      fprintf(fp,"%-20s%-30s%-10s",t[i].name,t[i].units,t[i].tele);/*��ʽд���¼*/
      fprintf(fp,"\r\n"); /*�����з���д���ļ�*/
   }
   fclose(fp);/*�ر��ļ�*/
   printf("****save success***\n"); /*��ʾ����ɹ�*/
}
/*���뺯��������Ϊ�ṹ������*/
int load(ADDRESS t[])
{
   int i,n;
   FILE *fp; /*ָ���ļ���ָ��*/
   if((fp=fopen("record.txt","rb"))==NULL)/*���ļ�*/
   {
      printf("can not open file\n");  /*���ܴ�*/
      exit(1);  /*�˳�*/
   }
   fscanf(fp,"%d",&n); /*�����¼��*/
   for(i=0;i<n;i++)
      fscanf(fp,"%20s%30s%10s",t[i].name,t[i].units,t[i].tele); /*����ʽ�����¼*/
   fclose(fp);  /*�ر��ļ�*/
   printf("You have success read data from file!!!\n"); /*��ʾ����ɹ�*/
   return n; /*���ؼ�¼��*/
}
/*�������ʾ��¼����*/
void display(ADDRESS t[])
{
   int id,n;
   FILE *fp; /*ָ���ļ���ָ��*/
   if((fp=fopen("record.txt","rb"))==NULL) /*���ļ�*/
   {
      printf("can not open file\n"); /*���ܴ��ļ�*/
      exit(1);  /*�˳�*/
   }
   printf("Enter order number...\n"); /*��ʾ��Ϣ*/
   scanf("%d",&id);  /*�������*/
   fscanf(fp,"%d",&n); /*���ļ������¼��*/
   if(id>=0&&id<n) /*�ж�����Ƿ��ڼ�¼��Χ��*/
   {
      fseek(fp,(id-1)*sizeof(ADDRESS),1); /*�ƶ��ļ�ָ�뵽�ü�¼λ��*/
      print(t[id]); /*�������������ʾ�ü�¼*/
      printf("\r\n");
   }
   else
      printf("no %d number record!!!\n ",id); /*�����Ų�������ʾ��Ϣ*/
   fclose(fp);  /*�ر��ļ�*/
}
/*������������Ϊ�ṹ������ͼ�¼��*/
void sort(ADDRESS t[],int n)
{
   int i,j,flag;
   ADDRESS temp; /*��ʱ����������������*/
   for(i=0;i<n;i++)
   {
      flag=0;  /*���־�ж��Ƿ���������*/
      for(j=0;j<n-1;j++)
      if((strcmp(t[j].name,t[j+1].name))>0) /*�Ƚϴ�С*/
      {
	 flag=1;
	 strcpy(temp.name,t[j].name);  /*������¼*/
	 strcpy(temp.units,t[j].units);
	 strcpy(temp.tele,t[j].tele);
	 strcpy(t[j].name,t[j+1].name);
	 strcpy(t[j].units,t[j+1].units);
	 strcpy(t[j].tele,t[j+1].tele);
	 strcpy(t[j+1].name,temp.name);
	 strcpy(t[j+1].units,temp.units);
	 strcpy(t[j+1].tele,temp.tele);
      }
      if(flag==0)break;  /*�����־Ϊ0��˵��û�з���������ѭ������*/
   }
   printf("sort sucess!!!\n"); /*��ʾ����ɹ�*/
}
/*���ٲ��ң�����Ϊ�ṹ������ͼ�¼��*/
void qseek(ADDRESS t[],int n)
{
   char s[20];
   int l,r,m;
   printf("\nPlease  sort before qseek!\n"); /*��ʾȷ���ڲ���֮ǰ����¼�Ƿ�������*/
   printf("please enter  name for qseek\n"); /*��ʾ����*/
   scanf("%s",s); /*��������ҵ�����*/
   l=0;r=n-1;  /*������߽����ұ߽�ĳ�ֵ*/
   while(l<=r) /*����߽�<=�ұ߽�ʱ*/
   {
      m=(l+r)/2; /*�����м�λ��*/
      if(strcmp(t[m].name,s)==0) /*���м��������ֶ����Ƚ����Ƿ����*/
      {
	 print(t[m]); /*�����ȣ������print������ʾ��¼��Ϣ*/
	 return ; /*����*/
      }
      if(strcmp(t[m].name,s)<0)  /*����м���С*/
	 l=m+1;  /*�޸���߽�*/
      else
	 r=m-1; /*�����м�����޸��ұ߽�*/
   }
   if(l>r)   /*�����߽�����ұ߽�ʱ*/
      printf("not found\n"); /*��ʾû�ҵ�*/
}
/*�����ļ�*/
void copy()
{
   char outfile[20]; /*Ŀ���ļ���*/
   int i,n;
   ADDRESS temp[M];  /*������ʱ����*/
   FILE *sfp,*tfp; /*����ָ���ļ���ָ��*/
   clrscr();/*����*/
   if((sfp=fopen("record.txt","rb"))==NULL) /*�򿪼�¼�ļ�*/
   {
      printf("can not open file\n"); /*��ʾ���ܴ��ļ���Ϣ*/
      exit(1); /*�˳�*/
   }
   printf("Enter outfile name,for example c:\\f1\\te.txt:\n"); /*��ʾ��Ϣ*/
   scanf("%s",outfile); /*����Ŀ���ļ���*/
   if((tfp=fopen(outfile,"wb"))==NULL) /*��Ŀ���ļ�*/
   {
      printf("can not open file\n"); /*��ʾ���ܴ��ļ���Ϣ*/
      exit(1); /*�˳�*/
   }
   fscanf(sfp,"%d",&n); /*�����ļ���¼��*/
   fprintf(tfp,"%d",n);/*д��Ŀ���ļ���*/
   fprintf(tfp,"\r\n"); /*д�뻻�з�*/
   for(i=0;i<n;i++)
   {
      fscanf(sfp,"%20s%30s%10s\n",temp[i].name,temp[i].units,
	temp[i].tele); /*�����¼*/
      fprintf(tfp,"%-20s%-30s%-10s\n",temp[i].name,
	temp[i].units,temp[i].tele); /*д���¼*/
      fprintf(tfp,"\r\n"); /*д�뻻�з�*/
   }
   fclose(sfp); /*�ر�Դ�ļ�*/
   fclose(tfp); /*�ر�Ŀ���ļ�*/
   printf("you have success copy  file!!!\n"); /*��ʾ���Ƴɹ�*/
}
