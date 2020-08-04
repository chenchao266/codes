/*FoxBASE+模拟系统文件名：foxsys.c*/
/*功能：模拟FoxBASE+，实现显示库结构和记录，以及统计功能*/
#include "stru_fox.h"
void use(FILE *), liststruct( ), list(FILE *), countfor(FILE *,char *);
int  check( );						/*函数说明*/
/**************************************************/
/*主函数main()：完成对命令的识别和相应子函数的调用*/
/**************************************************/
main()
   { char temp[81], *command, *phrase;      		/*命令行和子句最大长度为80*/
     int  ptr, prompt=0;                   		/* prompt: 错误命令标志*/
     FILE *filepointer=NULL;                		/*将文件指针初始化为空*/
     clrscr();	                         		/*调用clrscr()函数完成清屏*/
     printf("\n\n    FoxBASE+模拟系统\n\n");
     /*完成对命令的识别和相应子函数的调用*/
     for( ; 1; )                            		/*恒为真, 直到输入"quit"退出*/
       { if(prompt)                         		/*当prompt为真时, 提示出错*/
	    printf("\n  不识别的命令动词，按任一键继续...\n\n");
	 command=phrase=temp;               		/*command和phrase复位*/
	 printf(". ");
	 gets(command);                     		/*从键盘接收命令*/
	 /*1.截取命令行的命令动词和子句。*/
	 for( ; *command==' '; command++) ; 		/*舍弃命令动词的前导空格 */
	 if(strlen(command)==0)             		/*直接回车或仅由空格构成*/
	      { prompt=0;                     		/*置错误标志为"假"*/
		continue;                     		/*回到循环头*/
	      }
	 /* 设置命令动词结束位置(ptr) */
	 for(ptr=0; (*(command+ptr)!='\0')&&(*(command+ptr)!=' '); ptr++)  ;
	 /* 判断命令是否带有子句 */
	 if(*(command+ptr)==' ')           		/*命令动词结束, 可能带有子句*/
	    { *(command+ptr)='\0';           		/*在命令动词后置结束标志*/
	       /*处理子句*/
	       for(ptr++; *(command+ptr)!='\0'&&*(command+ptr)==' '; ptr++) ;
	       if(*(command+ptr)!='\0')       		/*存在子句*/
		 { phrase=command+ptr;       		/*phrase指向子句首字符*/
		   ptr=strlen(phrase) - 1;    		/*子句尾字符位置*/
		   for(; *(phrase+ptr)==' '; ptr--) ;
		   *(phrase+ptr+1)='\0';      		/*舍弃尾部空格*/
		 }
	       /*不存在子句时, phrase=temp ! */
	    }
         /* 2. 命令识别和相应子函数调用*/
	 if(strcmp(strlwr(command),"quit")==0) 		/*退出命令*/
	   { if( filepointer != NULL )         		/*已打开一个数据库文件*/
	       { free(ftp);                    		/*释放由use()函数申请的内存块*/
		 fclose(filepointer);
	       }
	     system("cls");
	     break;                            		/*退出循环, 结束运行*/
	   }
	 if(strcmp(strlwr(command),"clear")==0) 	/*清屏命令*/
	    { system("cls");
	       prompt=0;
	       continue;
	    }
	 if(strcmp(strlwr(command),"use")==0)  		/*打开数据库文件命令*/
	    { if(phrase==temp)                  	/*缺数据库文件名*/
	         { printf("\n  请输入数据库文件名: ");
	            gets(phrase);
	            printf("\n");
	         }
	       /*处理文件的扩展名*/
	       if(strchr(phrase, '.')==NULL)      	/*未带扩展名*/
		   strcat(phrase,".dbf");            	/*默认为.dbf*/
	       if((filepointer=fopen(phrase,"rb")) == NULL)
	          { printf("\n  文件不存在或打不开，按任一键继续... ");
		    getch();                      	/*等待按任意键(不回显)*/
		    printf("\n\n");
	          }
	       else                              	/*打开成功, 处理文件名*/
		 { if(DBF.filename!=NULL)         	/*非空(表示已经打开一个文件)则先释放*/
		      free(DBF.filename);
	            DBF.filename=(char *)malloc(strlen(phrase)+1);
		    strcpy(DBF.filename, phrase);  	/*复制文件名*/
		    use( filepointer );            	/*调用use()函数*/
	         }
	       prompt=0;
	       continue;
	   }
	 if(strcmp(strlwr(command),"list")==0)    	/*显示命令*/
	   { if(check()==1)                      	/*已打开一个数据库文件*/
	       if( strcmp(phrase,"stru") == 0 )  	/*显示结构*/
		  liststruct( );
	       else
		  if(phrase==temp)                  	/*无stru子句, 显示记录*/
		    list(filepointer);
		  else
		    { printf("\n  非法短语: %s，按任一键继续...", phrase);
		      getch();
		      printf("\n\n");
		    }
             prompt=0;
	     continue;
	   }
	 if(strcmp(strlwr(command),"countfor")==0)   	/*统计命令*/
	   { if(check()==1)                         	/*已打开一个数据库文件*/
		{ if(phrase==temp)                  	/*无统计条件, 提示输入*/
	             { printf("\n  请输入统计条件表达式: ");
		       gets(phrase);
	             }
		   countfor(filepointer, phrase);	/*调用统计函数*/
	        }
	     prompt=0;
	     continue;
	   }
      prompt=1;                                  	/*置命令错误标志*/
    }
  }                                              	/*主函数main()结束*/

/***************************/
/*check()函数：合法性检查  */
/* 形参：无。              */
/*返回值：1--合法，0--非法 */
/***************************/
int check()
  { int value=1;       				/*预置返回值为“真”*/
    if(ftp==NULL)      				/*未打开数据库文件*/
       { printf("\n  必须首先打开数据库文件，按任一键继续...");
	     getch();
	     printf("\n\n");
	     value=0;      			/*置非法标志*/
       }
    return(value);
  }                   		 		/* check()函数结束*/

/***************************************************************************/
/* use()函数: 打开数据库文件，即将文件说明内容读出、并存储到结构变量DBF中，*/
/*            同时，将字段说明内容读出、并存储到结构变量指针ftp中。 	   */
/* 形  参：FILE *							   */
/* 返回值：无								   */
/***************************************************************************/
void use( FILE *fp )
  { unsigned int loop, offset;
    /*读取文件的前12字节: 标志1B,日期3B,记录数4B,结构长度2B,记录长度2B*/
    fread( &DBF, 12, 1, fp);
    /*检查是否是数据库文件*/
    if( DBF.tag!=3 && DBF.tag!=131 )            	/*非数据库文件*/
       { printf("\n  not a database file, press any key to continue...");
	 getch();
	 printf("\n\n");
	 return;                                	/*返回*/
       }
    DBF.FieldCount=(DBF.FileStructSize-33)/32;  	/*计算字段数*/
    ftp=(struct FIELD *)malloc(sizeof(struct FIELD)*DBF.FieldCount);
    for(loop=1; loop <= DBF.FieldCount; loop++)
      { offset=(long)loop*32;
	fseek(fp, offset, 0);      			/*移动到字段说明的开始位置*/
	fgets(ftp->name, 11, fp);  			/*读字段名*/
	fseek(fp, offset+11, 0);   			/*移动到字段类型数据位置*/
	ftp->type=fgetc(fp);
	ftp->offset=getw(fp);      			/*读字段在记录中的起始位置*/
	fseek(fp, offset+16, 0);   			/*移动到字段长度数据位置*/
	ftp->width=fgetc(fp);
	ftp->decimal=fgetc(fp);
	ftp++;                     			/*指针下移一个元素*/
      }
    ftp = ftp - DBF.FieldCount;    			/*ftp复位*/
  }                                			/* use()函数结束 */

/************************************************/
/* liststruct()函数：分屏显示当前数据库文件结构 */
/* 形参：无                                     */
/* 返回值：无                                   */
/************************************************/
void liststruct( )
  { int loop=1;
    printf("Structure for database: %s\n",DBF.filename);
    printf("Number of data records: %ld\n",DBF.RecordCount);
    printf("Date of last update: ");
    printf("%2d.%2d.%2d\n",DBF.date[0],DBF.date[1],DBF.date[2]);
    /*打印表头*/
    printf("FieldNo Fieldname  Type Width Decimal\n");
    /*输出所有字段的名称、类型、宽度和小数位数*/
    for(; loop <= DBF.FieldCount; loop++, ftp++)
       { printf("%7d %-10s %-4c ",loop,ftp->name,ftp->type);
	 printf("%-5d %-d\n",ftp->width,ftp->decimal);
	 if(loop%20==0)                        		/*每显示20个字段, 暂停*/
	   { printf(" Press any key to continue... ");
	     getch();
	   }
       }
    printf("** Total **  %d\n\n",DBF.RecordSize);
    ftp = ftp - DBF.FieldCount;                		/*ftp复位*/
  }                                    			/*liststruct()函数结束 */

/***********************************************/
/* list()函数: 分屏显示当前数据库文件的全部记录*/
/* 形参：FILE *                                */
/* 返回值：无                                  */
/*说明: 备注字段仅显示"Memo", 不显示具体内容   */
/***********************************************/
void list( FILE *fp )
  { int  lp1, lp2;  					/*循环控制变量loop1, loop2*/
    int  space;     					/*字段名(或内容)后应加的空格数*/
    char *temp;     					/*暂存从文件中读取的每个字段内容*/
    /* 1. 打印表头 */
    printf("RecNo ");                           	/*记录号*/
    for(lp1=1; lp1 <= DBF.FieldCount; lp1++, ftp++)
      { printf("%s ",ftp->name);               		/*显示字段名*/
	/* 1.1 对齐处理: 当字段名窄于字段宽度时, 字段名后面加空格*/
	space = ftp->width - strlen(ftp->name);
	/* 1.2 处理备注字段: 定义宽度为10, 实际显示为"Memo"(宽度为4)*/
	if(ftp->type=='M') space = space - 6;   	/*减去差值6*/
	for(; space>0 ; space--) printf(" ");
      }
    printf("\n");                                	/*换行*/
    ftp = ftp - DBF.FieldCount;                  	/*ftp复位*/
    /* 2. 显示记录*/
    fseek(fp, DBF.FileStructSize, 0);        		/*将指针移动到首记录开始处*/
    for(lp1=1; lp1<=DBF.RecordCount; lp1++)  		/*外循环：控制所有记录显示*/
       { printf("%4d %c", lp1, fgetc(fp));  		/*显示记录号和删除标志*/
	 for(lp2=1;lp2<=DBF.FieldCount;lp2++,ftp++)	/*内循环：显示1条记录*/
	   { temp=(char *)malloc( ftp->width + 1 );	/*申请暂存空间*/
	     fgets(temp, ftp->width+1, fp);       	/*读取字段内容*/
	     if (ftp->type=='D')                   	/*日期型数据*/
	       { printf("%c%c-%c",*(temp+2),*(temp+3),*(temp+4));
		  printf("%c-%c%c  ",*(temp+5),*(temp+6),*(temp+7));
	       }
	     else
		{ if(ftp->type=='L')                	/*逻辑型数据*/
		    { if(*temp==' ')               	/*逻辑"假", 显示"F"*/
			strcpy(temp,"F");
		    }
		  else
		    { if(ftp->type=='M')            	/*备注字段, 显示"Memo"*/
			strcpy(temp,"Memo");
		    }
		  printf("%s ",temp);              	/*显示字段内容*/
		}
              /*对齐处理: 当字段内容窄于字段名时, 内容后面加空格*/
	      space = strlen(ftp->name) - strlen(temp);
	      for(; space>0 ; space--) printf(" ");
	      free(temp);                             	/*释放temp指向的内存空间*/
	    }                                         				/*内循环结束*/
	  printf("\n");                               	/*换行*/
	  ftp = ftp - DBF.FieldCount;                 	/*ftp复位*/
	  /* 3. 分屏处理*/
	  if(lp1%20==0)                               	/*每显示20个记录, 暂停*/
	    { printf(" Press any key to continue... ");
	      getch();
	      printf("\n");
	    }
       }                                            				/*外循环结束*/
     printf("\n");
  }                           				/* list()函数结束 */

/*********************************************************************/
/*countfor()函数: 统计满足一定条件的记录数                           */
/*形参：fp——指向当前数据库文件，cond——为统计条件表达式字符串指针 */
/*说明: 本函数仅实现基于数值字段的三种关系运算：=、>和<。            */
/*********************************************************************/
void countfor( FILE *fp, char *cond)
  { int  lp1, lp2;  					/*循环控制变量loop1, loop2 */
    int  count=0;   					/*存放统计结果*/
    char *temp;     					/*暂存字段内容的内存块指针*/
    char fieldname[11],operate,data[81];		/*存条件表达式分解后的三部分内容*/
    /* 1. 将条件表达式分解为三部分：字段名、关系运算符和内容*/
    /* 1.1 截取字段名  */
    for (lp1=0;cond[lp1]!='\0'&&cond[lp1]==' ';lp1++); 	/*跳过前导空格*/
    for (lp2=0; cond[lp1]!='\0'; lp1++,lp2++)
      { if(cond[lp1]!='>'&&cond[lp1]!='<'&&cond[lp1]!='='&&cond[lp1]!=' ')
	  fieldname[lp2]=cond[lp1];
	else
	  { fieldname[lp2]='\0';
	    break;                                       			/*字段名截取完成*/
	  }
       }
    /* 1.2 截取关系运算符 */
    if(cond[lp1]==' ')                                  			/*运算符前有空格*/
       for(;cond[lp1]!='\0'&&cond[lp1]==' '; lp1++) ;  	/*跳过! */
    operate=cond[lp1];
    /* 1.3 截取内容 */
    for(lp1++; cond[lp1]!='\0'&&cond[lp1]==' '; lp1++); /*跳过前导空格*/
    for(lp2=0; cond[lp1]!='\0' && cond[lp1]!=' '; lp1++,lp2++)
       data[lp2]=cond[lp1];
    data[lp2]='\0';                                      			/*置结束标志*/
    /* 2. 检查字段名和字段类型的合法性*/
    for(lp1=1; lp1 <= DBF.FieldCount; lp1++, ftp++)
       if(strcmp(ftp->name,fieldname)==0 && ftp->type=='N')
	 break;                     			/*合法字段，使ftp指向要统计的字段*/
    if( lp1 > DBF.FieldCount )       			/*字段名或(和)类型非法*/
      { ftp = ftp - DBF.FieldCount;  /*ftp复位*/
	printf("\n  字段名或(和)类型非法, 按任一键继续...");
	getch();
	printf("\n\n");
	return;
      }
    /* 3. 实施统计*/
    for(lp2=1; lp2<=DBF.RecordCount; lp2++)
      { /*将读写指针移动到字段内容开始处*/
	fseek(fp, DBF.FileStructSize+(lp2-1)*DBF.RecordSize+ftp->offset, 0);
	temp=(char *)malloc( ftp->width + 1 );  	/*申请暂存空间*/
	fgets(temp, ftp->width+1, fp);          	/*读取字段内容*/
	/*整型和浮点数据统一转换成浮点数据比较*/
	switch(operate)
	  {case '=': if(atof(temp)==atof(data)) count++;
		 break;
	   case '>': if(atof(temp)>atof(data)) count++;
		break;
	   case '<': if(atof(temp)<atof(data)) count++;
		break;
	   default:  printf("关系运算符非法, 按任一键继续...");
		 getch();
		 printf("\n\n");
		 return;
	   }
	 free(temp);                        		/*释放内存块*/
       }
    /* 4. 显示统计结果*/
    printf("\n  满足统计条件的记录个数 = %d\n\n", count);
    ftp = ftp - (lp1-1);                     		/*ftp复位*/
  }                                          		/* countfor()函数结束 */

