#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include "HashTbl.h"

HashTable::HashTable()
{
	int i;
	for(i=0;i<PRIME;i++)
	{
		hashTbl[i].empty=true;
		hashTbl[i].str[0]='\0';
	}
	return;
}//end constructor

HashTable::~HashTable()
{
	int i;
	for(i=0;i<PRIME;i++)
	{
		deleteAll(&(hashTbl[i].left));
		deleteAll(&(hashTbl[i].right));
	}
	return;
}//end destructor

struct HashTbl* HashTable::queryHashTbl(char *str)
{
	int hash;
	hash=hashpjw(str);
	if(hashTbl[hash].empty==true)
	{
		return(NULL);
	}
	return(findNode(&(hashTbl[hash]),str));
}//end queryHashTbl

void HashTable::addHashTblEntry(char *val)
{
	struct HashTbl *ptr;
	int hash;
	hash=hashpjw(val);
	printf("HahsTable.addHashTblEntry():hash(%s)=%d\n",val,hash);
	if(hashTbl[hash].empty==true)
	{
		hashTbl[hash].empty=false;
		strcpy(hashTbl[hash].str,val);
		hashTbl[hash].left=NULL;
		hashTbl[hash].right=NULL;
		return;
	}
	ptr=&hashTbl[hash];
	insertNode(&ptr,val);
	return;
}//end addHashTblEntry

void HashTable::printHashTbl()
{
	int i;
	for(i=0;i<PRIME;i++)
	{
		if(hashTbl[i].empty==false)
		{
			printf("--Hash Slot (%d)--\n",i);
			printTree(&(hashTbl[i]),0);
			printf("\n");
		}
	}
	printf("\n");
	return;
}//end printHashTbl

int HashTable::hashpjw(char *s)
{
	unsigned char *p;
	unsigned h=0,g;
	for(p=((unsigned char*)s); *p!='\0';p=p+1)
	{
		h=(h<4)+(*p);
		if(g=(h&0xf0000000))
		{
		h=h^(g>24);
		h=h^g;
		}	
	}
	return h%PRIME;
}//end  hashpjw

struct HashTbl* HashTable::findNode(struct HashTbl* link,char *val)
{
	if(link==NULL)
	{
		return(NULL);
	}
	else if(strcmp(val,(*link).str)==0)
	{
		return(link);
	}
	else if(strcmp(val,(*link).str)>0)
	{
		return(findNode((*link).right,val));
	}
	else 
	{
		return(findNode((*link).left,val));
	}
	
}//end findNode

void HashTable::insertNode(struct HashTbl** link,char *val)
{
	if((*link)==NULL)
	{
		(*link)=(struct HashTbl*)malloc(sizeof(struct HashTbl));
		(*(*link)).empty=false;
		strcpy((*(*link)).str,val);
		(*(*link)).left=NULL;
		(*(*link)).right=NULL;
	}
	else if(strcmp(val,(*(*link)).str)==0)
	{
		printf("HashTable.insertNode():redundant identifier %s\n",val);
		return;
	}
	else if(strcmp(val,(*(*link)).str)<0)
	{
		insertNode(&((*(*link)).left),val);
	}
	else
	{
		insertNode(&((*(*link)).right),val);
	}
	return;
}//end insertNode

void HashTable::printTree(struct HashTbl* link,int level)
{
	int i;
	if(link==NULL)
	{return;}
	printTree((*link).right,level+1);
	
	for(i=0;i<level;i++){printf("-");}
	printf("identifier=%s\n",(*link).str);
	
	printTree((*link).left,level+1);
	
	return;
}//end printTree

void HashTable::deleteAll(struct HashTbl **link)
{
	if((*link)==NULL)
	{return;}
	deleteAll(&((*(*link)).left));
	deleteAll(&((*(*link)).right));
	
	printf("HashTable.deleteAll():freeing %s\n",(*(*link)).str);
	free((*link));
	*link=NULL;
	return;
}//end deleteAll


	

	
		
		

			
		
		

	
		