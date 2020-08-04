/*FoxBASE+模拟系统的头文件名：stru_fox.h*/
/*内容：系统头文件+用户自定义的字段结构类型和数据库结构类型*/
#include "stdio.h"
#include "conio.h"
#include "stdlib.h"
#include "alloc.h"
#include "math.h"
/*字段结构类型及其指针变量ftp*/
struct FIELD
   { char name[11];  			/*字段名,多出的1位存结束标志'\0'*/
     char type;     			/*字段类型*/
     char width;    			/*字段宽度*/
     char decimal;   			/*小数位数*/
     int  offset;   			/*字段在记录中的起始位置*/
   } *ftp;          			/* FieldTablePointer：字段表指针*/
/*DBF结构类型及其变量DBF*/
struct
   { unsigned char tag;              	/*数据库文件标签*/
     unsigned char date[3];          	/*建立或修改的最后日期*/
     unsigned long int RecordCount;	/*记录总数*/
     unsigned int FileStructSize;    	/*文件说明部分长度*/
     unsigned int RecordSize;        	/*记录长度*/
     char *filename;                 	/*数据库文件名*/
     unsigned int FieldCount;        	/*字段数*/
   } DBF;

