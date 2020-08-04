#include "Bst.cpp"

void  main()
{
    BinarySearchTree bst;
    bst.root_ptr=NULL;

    bst.insertNode(&(bst.root_ptr),15);
    bst.insertNode(&(bst.root_ptr),20);
    bst.insertNode(&(bst.root_ptr),7);
    bst.insertNode(&(bst.root_ptr),17);
    bst.insertNode(&(bst.root_ptr),25);
    bst.insertNode(&(bst.root_ptr),2);
    bst.insertNode(&(bst.root_ptr),30);
    bst.insertNode(&(bst.root_ptr),1);
    bst.insertNode(&(bst.root_ptr),7);

   printf("height=%d\n",bst.getHeight(bst.root_ptr));
   bst.printTree(bst.root_ptr,0);

   bst.deleteNode(&(bst.root_ptr),20);
   printf("height=%d\n",bst.getHeight(bst.root_ptr));
   bst.printTree(bst.root_ptr,0);

   bst.deleteNode(&(bst.root_ptr),2);
   printf("height=%d\n",bst.getHeight(bst.root_ptr));
   bst.printTree(bst.root_ptr,0);

   bst.deleteNode(&(bst.root_ptr),13);
   printf("height=%d\n",bst.getHeight(bst.root_ptr));
   bst.printTree(bst.root_ptr,0);

   bst.deleteNode(&(bst.root_ptr),20);
   printf("height=%d\n",bst.getHeight(bst.root_ptr));
   bst.printTree(bst.root_ptr,0);

   if((bst.findNode(bst.root_ptr,17))!=NULL)){printf("found 17\n");}
   else{ printf("could not find 17\n");}

   if((bst.findNode(bst.root_ptr,8))!=NULL)){printf("found 17\n");}
   else{ printf("could not find 17\n");}

   bst.deleteAll(&(bst.root_ptr));
   printf("height=%d\n",bst.getHeight(bst.root_ptr));
   bst.printTree(bst.root_ptr,0);

   return;
}
//end main



