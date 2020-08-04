struct HashTbl
{
	bool empty;
	char str[32];
	struct HashTbl *left;
	struct HashTbl *right;
};

#define PRIME 5

class	HashTable
{
	struct HashTbl hashTbl[PRIME];
	int hashpjw(char *s); //???
	struct HashTbl* findNode(struct HashTbl* link,char *val);
	void insertNode(struct HashTbl** link,char *val);
	void printTree(struct HashTbl* link,int level);
	void deleteAll(struct HashTbl **link);
	
	public:
	HashTable();
	~HashTable();
	struct HashTbl* queryHashTbl(char *str);
	void addHashTblEntry(char *val);
	void printHashTbl();
};

