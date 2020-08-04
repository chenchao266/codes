#include "HashTbl.cpp"

void main()
{
	char str[32];
	HashTable ht;
	ht.addHashTblEntry("register");
	ht.addHashTblEntry("asm");
	ht.addHashTblEntry("union");
	ht.addHashTblEntry("goto");
	ht.addHashTblEntry("do");
	ht.addHashTblEntry("public");
	ht.addHashTblEntry("extern");
	ht.addHashTblEntry("main");
	ht.addHashTblEntry("break");
	ht.addHashTblEntry("this");
	ht.addHashTblEntry("float");
	ht.addHashTblEntry("if");
	ht.addHashTblEntry("void");
	ht.addHashTblEntry("new");
	ht.addHashTblEntry("case");
	ht.addHashTblEntry("sizeof");
	
	ht.addHashTblEntry("goto");
	
	ht.printHashTbl();
	
	strcpy(str,"float");
	if((ht.queryHashTbl(str))!=NULL){printf("found %s\n",str);}
	else{printf("did not find %s\n",str);}

	strcpy(str,"tax_audit");
	if((ht.queryHashTbl(str))!=NULL){printf("found %s\n",str);}
	else{printf("did not find %s\n",str);}
	
	strcpy(str,"extern");
	if((ht.queryHashTbl(str))!=NULL){printf("found %s\n",str);}
	else{printf("did not find %s\n",str);}
	
	strcpy(str,"goto");
	if((ht.queryHashTbl(str))!=NULL){printf("found %s\n",str);}
	else{printf("did not find %s\n",str);}
	
	strcpy(str,"technobabble");
	if((ht.queryHashTbl(str))!=NULL){printf("found %s\n",str);}
	else{printf("did not find %s\n",str);}
	
	return;
}	
	