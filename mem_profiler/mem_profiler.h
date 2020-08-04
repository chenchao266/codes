
// Definição do tipo booleano //
typedef unsigned char bool;
 
#ifndef true
 #define true  1
#endif
 
#ifndef false
 #define false 0
#endif


// Definição da estrutura de um nó //
struct node {
 void *        base_address;
 unsigned long block_size;
 char *        comment;

 struct node * next_node;
 struct node * previous_node;
};

typedef struct node node;

// Definição da estrutura de uma lista //
struct linked_list {
 struct node * head;
 struct node * tail;
};

typedef struct linked_list linked_list;

// Declaração das funções que operam sobre o mapa de memória //
void   profile_start  (void);
void   profile_stop   (void);
void * profile_malloc (size_t size);
void * profile_malloc_with_comment (size_t size, const char * comment);
void * profile_free   (void * block_to_free);
void   profile_print  (void);

// Declaração das funções que operam sobre listas //
node *      list_search_block (linked_list list_to_search, void * block_to_search_for);
linked_list list_create       (void);
linked_list list_delete       (linked_list list_to_delete);
linked_list list_insert_node  (linked_list list_to_edit, node * node_to_insert);
linked_list list_remove_node  (linked_list list_to_edit, node * node_to_remove);
bool        list_node_exists  (linked_list list_to_search, node * node_to_search_for);
void        list_print        (linked_list list_to_print);
void        list_print_blocks (linked_list list_to_print);

// Declaração das funções que operam sobre nós //
node * node_create   (void * base_address, unsigned long block_size, const char * comment);
node * node_delete   (node * node_to_delete);
void   node_print    (node * node);

