/*FoxBASE+ģ��ϵͳ�ļ�����foxsys.c*/
/*���ܣ�ģ��FoxBASE+��ʵ����ʾ��ṹ�ͼ�¼���Լ�ͳ�ƹ���*/
#include "stru_fox.h"
void use(FILE *), liststruct( ), list(FILE *), countfor(FILE *,char *);
int  check( );						/*����˵��*/
/**************************************************/
/*������main()����ɶ������ʶ�����Ӧ�Ӻ����ĵ���*/
/**************************************************/
main()
   { char temp[81], *command, *phrase;      		/*�����к��Ӿ���󳤶�Ϊ80*/
     int  ptr, prompt=0;                   		/* prompt: ���������־*/
     FILE *filepointer=NULL;                		/*���ļ�ָ���ʼ��Ϊ��*/
     clrscr();	                         		/*����clrscr()�����������*/
     printf("\n\n    FoxBASE+ģ��ϵͳ\n\n");
     /*��ɶ������ʶ�����Ӧ�Ӻ����ĵ���*/
     for( ; 1; )                            		/*��Ϊ��, ֱ������"quit"�˳�*/
       { if(prompt)                         		/*��promptΪ��ʱ, ��ʾ����*/
	    printf("\n  ��ʶ�������ʣ�����һ������...\n\n");
	 command=phrase=temp;               		/*command��phrase��λ*/
	 printf(". ");
	 gets(command);                     		/*�Ӽ��̽�������*/
	 /*1.��ȡ�����е�����ʺ��Ӿ䡣*/
	 for( ; *command==' '; command++) ; 		/*��������ʵ�ǰ���ո� */
	 if(strlen(command)==0)             		/*ֱ�ӻس�����ɿո񹹳�*/
	      { prompt=0;                     		/*�ô����־Ϊ"��"*/
		continue;                     		/*�ص�ѭ��ͷ*/
	      }
	 /* ��������ʽ���λ��(ptr) */
	 for(ptr=0; (*(command+ptr)!='\0')&&(*(command+ptr)!=' '); ptr++)  ;
	 /* �ж������Ƿ�����Ӿ� */
	 if(*(command+ptr)==' ')           		/*����ʽ���, ���ܴ����Ӿ�*/
	    { *(command+ptr)='\0';           		/*������ʺ��ý�����־*/
	       /*�����Ӿ�*/
	       for(ptr++; *(command+ptr)!='\0'&&*(command+ptr)==' '; ptr++) ;
	       if(*(command+ptr)!='\0')       		/*�����Ӿ�*/
		 { phrase=command+ptr;       		/*phraseָ���Ӿ����ַ�*/
		   ptr=strlen(phrase) - 1;    		/*�Ӿ�β�ַ�λ��*/
		   for(; *(phrase+ptr)==' '; ptr--) ;
		   *(phrase+ptr+1)='\0';      		/*����β���ո�*/
		 }
	       /*�������Ӿ�ʱ, phrase=temp ! */
	    }
         /* 2. ����ʶ�����Ӧ�Ӻ�������*/
	 if(strcmp(strlwr(command),"quit")==0) 		/*�˳�����*/
	   { if( filepointer != NULL )         		/*�Ѵ�һ�����ݿ��ļ�*/
	       { free(ftp);                    		/*�ͷ���use()����������ڴ��*/
		 fclose(filepointer);
	       }
	     system("cls");
	     break;                            		/*�˳�ѭ��, ��������*/
	   }
	 if(strcmp(strlwr(command),"clear")==0) 	/*��������*/
	    { system("cls");
	       prompt=0;
	       continue;
	    }
	 if(strcmp(strlwr(command),"use")==0)  		/*�����ݿ��ļ�����*/
	    { if(phrase==temp)                  	/*ȱ���ݿ��ļ���*/
	         { printf("\n  ���������ݿ��ļ���: ");
	            gets(phrase);
	            printf("\n");
	         }
	       /*�����ļ�����չ��*/
	       if(strchr(phrase, '.')==NULL)      	/*δ����չ��*/
		   strcat(phrase,".dbf");            	/*Ĭ��Ϊ.dbf*/
	       if((filepointer=fopen(phrase,"rb")) == NULL)
	          { printf("\n  �ļ������ڻ�򲻿�������һ������... ");
		    getch();                      	/*�ȴ��������(������)*/
		    printf("\n\n");
	          }
	       else                              	/*�򿪳ɹ�, �����ļ���*/
		 { if(DBF.filename!=NULL)         	/*�ǿ�(��ʾ�Ѿ���һ���ļ�)�����ͷ�*/
		      free(DBF.filename);
	            DBF.filename=(char *)malloc(strlen(phrase)+1);
		    strcpy(DBF.filename, phrase);  	/*�����ļ���*/
		    use( filepointer );            	/*����use()����*/
	         }
	       prompt=0;
	       continue;
	   }
	 if(strcmp(strlwr(command),"list")==0)    	/*��ʾ����*/
	   { if(check()==1)                      	/*�Ѵ�һ�����ݿ��ļ�*/
	       if( strcmp(phrase,"stru") == 0 )  	/*��ʾ�ṹ*/
		  liststruct( );
	       else
		  if(phrase==temp)                  	/*��stru�Ӿ�, ��ʾ��¼*/
		    list(filepointer);
		  else
		    { printf("\n  �Ƿ�����: %s������һ������...", phrase);
		      getch();
		      printf("\n\n");
		    }
             prompt=0;
	     continue;
	   }
	 if(strcmp(strlwr(command),"countfor")==0)   	/*ͳ������*/
	   { if(check()==1)                         	/*�Ѵ�һ�����ݿ��ļ�*/
		{ if(phrase==temp)                  	/*��ͳ������, ��ʾ����*/
	             { printf("\n  ������ͳ����������ʽ: ");
		       gets(phrase);
	             }
		   countfor(filepointer, phrase);	/*����ͳ�ƺ���*/
	        }
	     prompt=0;
	     continue;
	   }
      prompt=1;                                  	/*����������־*/
    }
  }                                              	/*������main()����*/

/***************************/
/*check()�������Ϸ��Լ��  */
/* �βΣ��ޡ�              */
/*����ֵ��1--�Ϸ���0--�Ƿ� */
/***************************/
int check()
  { int value=1;       				/*Ԥ�÷���ֵΪ���桱*/
    if(ftp==NULL)      				/*δ�����ݿ��ļ�*/
       { printf("\n  �������ȴ����ݿ��ļ�������һ������...");
	     getch();
	     printf("\n\n");
	     value=0;      			/*�÷Ƿ���־*/
       }
    return(value);
  }                   		 		/* check()��������*/

/***************************************************************************/
/* use()����: �����ݿ��ļ��������ļ�˵�����ݶ��������洢���ṹ����DBF�У�*/
/*            ͬʱ�����ֶ�˵�����ݶ��������洢���ṹ����ָ��ftp�С� 	   */
/* ��  �Σ�FILE *							   */
/* ����ֵ����								   */
/***************************************************************************/
void use( FILE *fp )
  { unsigned int loop, offset;
    /*��ȡ�ļ���ǰ12�ֽ�: ��־1B,����3B,��¼��4B,�ṹ����2B,��¼����2B*/
    fread( &DBF, 12, 1, fp);
    /*����Ƿ������ݿ��ļ�*/
    if( DBF.tag!=3 && DBF.tag!=131 )            	/*�����ݿ��ļ�*/
       { printf("\n  not a database file, press any key to continue...");
	 getch();
	 printf("\n\n");
	 return;                                	/*����*/
       }
    DBF.FieldCount=(DBF.FileStructSize-33)/32;  	/*�����ֶ���*/
    ftp=(struct FIELD *)malloc(sizeof(struct FIELD)*DBF.FieldCount);
    for(loop=1; loop <= DBF.FieldCount; loop++)
      { offset=(long)loop*32;
	fseek(fp, offset, 0);      			/*�ƶ����ֶ�˵���Ŀ�ʼλ��*/
	fgets(ftp->name, 11, fp);  			/*���ֶ���*/
	fseek(fp, offset+11, 0);   			/*�ƶ����ֶ���������λ��*/
	ftp->type=fgetc(fp);
	ftp->offset=getw(fp);      			/*���ֶ��ڼ�¼�е���ʼλ��*/
	fseek(fp, offset+16, 0);   			/*�ƶ����ֶγ�������λ��*/
	ftp->width=fgetc(fp);
	ftp->decimal=fgetc(fp);
	ftp++;                     			/*ָ������һ��Ԫ��*/
      }
    ftp = ftp - DBF.FieldCount;    			/*ftp��λ*/
  }                                			/* use()�������� */

/************************************************/
/* liststruct()������������ʾ��ǰ���ݿ��ļ��ṹ */
/* �βΣ���                                     */
/* ����ֵ����                                   */
/************************************************/
void liststruct( )
  { int loop=1;
    printf("Structure for database: %s\n",DBF.filename);
    printf("Number of data records: %ld\n",DBF.RecordCount);
    printf("Date of last update: ");
    printf("%2d.%2d.%2d\n",DBF.date[0],DBF.date[1],DBF.date[2]);
    /*��ӡ��ͷ*/
    printf("FieldNo Fieldname  Type Width Decimal\n");
    /*��������ֶε����ơ����͡����Ⱥ�С��λ��*/
    for(; loop <= DBF.FieldCount; loop++, ftp++)
       { printf("%7d %-10s %-4c ",loop,ftp->name,ftp->type);
	 printf("%-5d %-d\n",ftp->width,ftp->decimal);
	 if(loop%20==0)                        		/*ÿ��ʾ20���ֶ�, ��ͣ*/
	   { printf(" Press any key to continue... ");
	     getch();
	   }
       }
    printf("** Total **  %d\n\n",DBF.RecordSize);
    ftp = ftp - DBF.FieldCount;                		/*ftp��λ*/
  }                                    			/*liststruct()�������� */

/***********************************************/
/* list()����: ������ʾ��ǰ���ݿ��ļ���ȫ����¼*/
/* �βΣ�FILE *                                */
/* ����ֵ����                                  */
/*˵��: ��ע�ֶν���ʾ"Memo", ����ʾ��������   */
/***********************************************/
void list( FILE *fp )
  { int  lp1, lp2;  					/*ѭ�����Ʊ���loop1, loop2*/
    int  space;     					/*�ֶ���(������)��Ӧ�ӵĿո���*/
    char *temp;     					/*�ݴ���ļ��ж�ȡ��ÿ���ֶ�����*/
    /* 1. ��ӡ��ͷ */
    printf("RecNo ");                           	/*��¼��*/
    for(lp1=1; lp1 <= DBF.FieldCount; lp1++, ftp++)
      { printf("%s ",ftp->name);               		/*��ʾ�ֶ���*/
	/* 1.1 ���봦��: ���ֶ���խ���ֶο���ʱ, �ֶ�������ӿո�*/
	space = ftp->width - strlen(ftp->name);
	/* 1.2 ������ע�ֶ�: �������Ϊ10, ʵ����ʾΪ"Memo"(����Ϊ4)*/
	if(ftp->type=='M') space = space - 6;   	/*��ȥ��ֵ6*/
	for(; space>0 ; space--) printf(" ");
      }
    printf("\n");                                	/*����*/
    ftp = ftp - DBF.FieldCount;                  	/*ftp��λ*/
    /* 2. ��ʾ��¼*/
    fseek(fp, DBF.FileStructSize, 0);        		/*��ָ���ƶ����׼�¼��ʼ��*/
    for(lp1=1; lp1<=DBF.RecordCount; lp1++)  		/*��ѭ�����������м�¼��ʾ*/
       { printf("%4d %c", lp1, fgetc(fp));  		/*��ʾ��¼�ź�ɾ����־*/
	 for(lp2=1;lp2<=DBF.FieldCount;lp2++,ftp++)	/*��ѭ������ʾ1����¼*/
	   { temp=(char *)malloc( ftp->width + 1 );	/*�����ݴ�ռ�*/
	     fgets(temp, ftp->width+1, fp);       	/*��ȡ�ֶ�����*/
	     if (ftp->type=='D')                   	/*����������*/
	       { printf("%c%c-%c",*(temp+2),*(temp+3),*(temp+4));
		  printf("%c-%c%c  ",*(temp+5),*(temp+6),*(temp+7));
	       }
	     else
		{ if(ftp->type=='L')                	/*�߼�������*/
		    { if(*temp==' ')               	/*�߼�"��", ��ʾ"F"*/
			strcpy(temp,"F");
		    }
		  else
		    { if(ftp->type=='M')            	/*��ע�ֶ�, ��ʾ"Memo"*/
			strcpy(temp,"Memo");
		    }
		  printf("%s ",temp);              	/*��ʾ�ֶ�����*/
		}
              /*���봦��: ���ֶ�����խ���ֶ���ʱ, ���ݺ���ӿո�*/
	      space = strlen(ftp->name) - strlen(temp);
	      for(; space>0 ; space--) printf(" ");
	      free(temp);                             	/*�ͷ�tempָ����ڴ�ռ�*/
	    }                                         				/*��ѭ������*/
	  printf("\n");                               	/*����*/
	  ftp = ftp - DBF.FieldCount;                 	/*ftp��λ*/
	  /* 3. ��������*/
	  if(lp1%20==0)                               	/*ÿ��ʾ20����¼, ��ͣ*/
	    { printf(" Press any key to continue... ");
	      getch();
	      printf("\n");
	    }
       }                                            				/*��ѭ������*/
     printf("\n");
  }                           				/* list()�������� */

/*********************************************************************/
/*countfor()����: ͳ������һ�������ļ�¼��                           */
/*�βΣ�fp����ָ��ǰ���ݿ��ļ���cond����Ϊͳ����������ʽ�ַ���ָ�� */
/*˵��: ��������ʵ�ֻ�����ֵ�ֶε����ֹ�ϵ���㣺=��>��<��            */
/*********************************************************************/
void countfor( FILE *fp, char *cond)
  { int  lp1, lp2;  					/*ѭ�����Ʊ���loop1, loop2 */
    int  count=0;   					/*���ͳ�ƽ��*/
    char *temp;     					/*�ݴ��ֶ����ݵ��ڴ��ָ��*/
    char fieldname[11],operate,data[81];		/*����������ʽ�ֽ�������������*/
    /* 1. ����������ʽ�ֽ�Ϊ�����֣��ֶ�������ϵ�����������*/
    /* 1.1 ��ȡ�ֶ���  */
    for (lp1=0;cond[lp1]!='\0'&&cond[lp1]==' ';lp1++); 	/*����ǰ���ո�*/
    for (lp2=0; cond[lp1]!='\0'; lp1++,lp2++)
      { if(cond[lp1]!='>'&&cond[lp1]!='<'&&cond[lp1]!='='&&cond[lp1]!=' ')
	  fieldname[lp2]=cond[lp1];
	else
	  { fieldname[lp2]='\0';
	    break;                                       			/*�ֶ�����ȡ���*/
	  }
       }
    /* 1.2 ��ȡ��ϵ����� */
    if(cond[lp1]==' ')                                  			/*�����ǰ�пո�*/
       for(;cond[lp1]!='\0'&&cond[lp1]==' '; lp1++) ;  	/*����! */
    operate=cond[lp1];
    /* 1.3 ��ȡ���� */
    for(lp1++; cond[lp1]!='\0'&&cond[lp1]==' '; lp1++); /*����ǰ���ո�*/
    for(lp2=0; cond[lp1]!='\0' && cond[lp1]!=' '; lp1++,lp2++)
       data[lp2]=cond[lp1];
    data[lp2]='\0';                                      			/*�ý�����־*/
    /* 2. ����ֶ������ֶ����͵ĺϷ���*/
    for(lp1=1; lp1 <= DBF.FieldCount; lp1++, ftp++)
       if(strcmp(ftp->name,fieldname)==0 && ftp->type=='N')
	 break;                     			/*�Ϸ��ֶΣ�ʹftpָ��Ҫͳ�Ƶ��ֶ�*/
    if( lp1 > DBF.FieldCount )       			/*�ֶ�����(��)���ͷǷ�*/
      { ftp = ftp - DBF.FieldCount;  /*ftp��λ*/
	printf("\n  �ֶ�����(��)���ͷǷ�, ����һ������...");
	getch();
	printf("\n\n");
	return;
      }
    /* 3. ʵʩͳ��*/
    for(lp2=1; lp2<=DBF.RecordCount; lp2++)
      { /*����дָ���ƶ����ֶ����ݿ�ʼ��*/
	fseek(fp, DBF.FileStructSize+(lp2-1)*DBF.RecordSize+ftp->offset, 0);
	temp=(char *)malloc( ftp->width + 1 );  	/*�����ݴ�ռ�*/
	fgets(temp, ftp->width+1, fp);          	/*��ȡ�ֶ�����*/
	/*���ͺ͸�������ͳһת���ɸ������ݱȽ�*/
	switch(operate)
	  {case '=': if(atof(temp)==atof(data)) count++;
		 break;
	   case '>': if(atof(temp)>atof(data)) count++;
		break;
	   case '<': if(atof(temp)<atof(data)) count++;
		break;
	   default:  printf("��ϵ������Ƿ�, ����һ������...");
		 getch();
		 printf("\n\n");
		 return;
	   }
	 free(temp);                        		/*�ͷ��ڴ��*/
       }
    /* 4. ��ʾͳ�ƽ��*/
    printf("\n  ����ͳ�������ļ�¼���� = %d\n\n", count);
    ftp = ftp - (lp1-1);                     		/*ftp��λ*/
  }                                          		/* countfor()�������� */

