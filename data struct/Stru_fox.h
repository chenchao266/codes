/*FoxBASE+ģ��ϵͳ��ͷ�ļ�����stru_fox.h*/
/*���ݣ�ϵͳͷ�ļ�+�û��Զ�����ֶνṹ���ͺ����ݿ�ṹ����*/
#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#include "alloc.h"
#include "math.h"
/*�ֶνṹ���ͼ���ָ�����ftp*/
struct FIELD
   { char name[11];  			/*�ֶ���,�����1λ�������־'\0'*/
     char type;     			/*�ֶ�����*/
     char width;    			/*�ֶο��*/
     char decimal;   			/*С��λ��*/
     int  offset;   			/*�ֶ��ڼ�¼�е���ʼλ��*/
   } *ftp;          			/* FieldTablePointer���ֶα�ָ��*/
/*DBF�ṹ���ͼ������DBF*/
struct
   { unsigned char tag;              	/*���ݿ��ļ���ǩ*/
     unsigned char date[3];          	/*�������޸ĵ��������*/
     unsigned long int RecordCount;	/*��¼����*/
     unsigned int FileStructSize;    	/*�ļ�˵�����ֳ���*/
     unsigned int RecordSize;        	/*��¼����*/
     char *filename;                 	/*���ݿ��ļ���*/
     unsigned int FieldCount;        	/*�ֶ���*/
   } DBF;

