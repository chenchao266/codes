#include<malloc.h>
#include<stdio.h>
#include "Bst.h"

void BinarySearchTree::insertNode(struct BiNode **link,int val)
{
    if(*link==NULL)
    {
    	(*link)=(struct BiNode*)malloc(sizeof(struct BiNode));
    	(*(*link)).value=val;
    	(*(*link)).left=NULL;
    	(*(*link)).right=NULL;
    	printf("insertNode(): inserting %d\n",val);
    }
    else if(val<(*(*link)).value)
    {	
    	printf("insertNode(): moving left\n",val);
    	insertNode(&((*(*link)).left),val);
    }
    else
    {
    	printf("insertNode(): moving right\n",val);
    	insertNode(&((*(*link)).right),val);
    }
    return;
    
}//end insertNode

struct BiNode* BinarySearchTree::findNode(struct BiNode *link,int val)
{
    if(link==NULL)
    {
    	return(NULL);
    }
    else if((*link).value==val)
    {
    	return(link);
    }
    else if(val>=(*link).value)
    {
    	return(findNode((*link).right,val));
    }
    else
    {
    	return(findNode((*link).left,val));
    }
}//end findNode

struct BiNode* BinarySearchTree::deleteSmallestNode(struct BiNode **link)
{
    if((*(*link)).left!=NULL)
    {
    return (deleteSmallestNode(&((*(*link)).left)));
    }
    else
    {
        struct BiNode *temp;
        temp=*link;
        (*link)=(*(*link)).right;
        return(temp);
    }
}//end deleteSmallestNode

void BinarySearchTree::deleteNode(struct BiNode **link,int val)
{
    if((*link)==NULL)
    {
    printf("deleteNdoe():%d does not exist\n",val);
    return;
    }

    if(val<(*(*link)).value)
    {
    deleteNode(&((*(*link)).left),val);
    }
    else if(val>(*(*link)).value)
    {
    deleteNode(&((*(*link)).right),val);
    }
    else
    {
        struct BiNode *temp;
        temp=*link;
        if((*(*link)).right==NULL)
        {
            (*link)=(*(*link)).left;
        }
        else if((*(*link)).left==NULL)
        {
            (*link)=(*(*link)).right;
        }
        else
        {
            temp=deleteSmallestNode(&((*(*link)).right));
            (*(*link)).value=(*temp).value;
        }

        printf("deleteNode(): freeing %d\n",val);
        free(temp);
        }
        return ;
}//end deleteNode

void BinarySearchTree::deleteAll(struct BiNode **link)
{
    if((*link)==NULL)
    {
        return;
    }
    deleteAll(&((*(*link)).left));
    deleteAll(&((*(*link)).right));

    printf("deleteAlll():freeing %d\n",(*(*link)).value);
    free((*link));
    *link=NULL;
    return;
}//end deleteAll

void BinarySearchTree::printTree(struct BiNode *link,int level)
{
    int i;
    if(link==NULL)
    {
    return;
    }
    printTree((*link).right,level+1);
    
    for(i=0;i<level;i++){printf("-");}
    printf("(%d)\n",(*link).value);

    printTree((*link).left,level+1);
    return;
}//end printTree

int BinarySearchTree::getHeight(struct BiNode* link)
{
    int u,v;
    if(link==NULL){return(-1);}
    u=getHeight((*link).left);
    v=getHeight((*link).right);
    if(u>v){return(u+1);}

    else {return(v+1);}

    
    

