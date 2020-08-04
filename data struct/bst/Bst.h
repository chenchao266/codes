//binary search tree

struct BiNode
{
int value;
struct BiNode *left;
struct BiNode *right;
};

class BinarySearchTree
{
    public:
	struct BiNode *root_ptr;
	void insertNode(struct BiNode  **link,int val);
	struct BiNode *findNode(struct BiNode *link,int val);
	struct BiNode *deleteSmallestNode(struct BiNode  **link);
	void deleteNode(struct BiNode **link,int val);
	void deleteAll(struct BiNode  **link);
	void printTree(struct BiNode *link,int level);
	int getHeight(struct BiNode *link);
};


