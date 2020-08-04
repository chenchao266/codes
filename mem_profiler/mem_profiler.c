
#include <stdio.h>
#include "mem_profiler.h"


// A lista que ir� conter a informa��o do mapa da mem�ria //
linked_list profile_list;


void profile_start (void) {
 // Criar uma nova lista e assign�-la � lista que cont�m o perfil da mem�ria //
 profile_list = list_create ();
}

void profile_stop (void) {
 // Apagar a lista que cont�m o perfil da mem�ria //
 profile_list = list_delete (profile_list);
}

void * profile_malloc (size_t size) {
 // Usa a fun��o standard do C para alocar a mem�ria pedida //
 void * allocated_block = (void *) malloc (size);
 
 // Criar um n� para guardar a informa��o sobre o bloco alocado //
 node * new_node = node_create (allocated_block, size, "");
 // Inserir o n� na lista //
 profile_list = list_insert_node (profile_list, new_node);
 // Devolver o apontador para o bloco de mem�ria alocado //
 return (allocated_block);
}

void * profile_malloc_with_comment (size_t size, const char * comment) {
 // Usa a fun��o standard do C para alocar a mem�ria pedida //
 void * allocated_block = (void *) malloc (size);
 
 // Criar um n� para guardar a informa��o sobre o bloco alocado //
 node * new_node = node_create (allocated_block, size, comment);
 // Inserir o n� na lista //
 profile_list = list_insert_node (profile_list, new_node);
 // Devolver o apontador para o bloco de mem�ria alocado //
 return (allocated_block);
}

void * profile_free (void * block_to_free) {
 // Procurar o primeiro bloco n�o-livre cujo endere�o-base seja igual �quele que se pretende libertar //
 node * profile_node = list_search_block (profile_list, block_to_free);
 // Se n�o foi encontrado nenhum bloco ainda por libertar //
 if (profile_node == (node *) NULL) {
  // Imprime uma mensagem de erro //
  fprintf (stderr, "ERRO : APLICACAO : profile_free : Tentativa de libertar bloco de memoria invalido.");   
  // Sai da aplica��o //
  exit (1);
 }
 // Remover da lista o n� que guarda a informa��o relativa ao bloco que est� a ser libertado //
 profile_list = list_remove_node (profile_list, profile_node);
 // Devolver o mesmo que a fun��o standard do C devolve //
 return (void *) (free (block_to_free));
}

void profile_print (void) {
 list_print_blocks (profile_list);
}

node * list_search_block (linked_list list_to_search, void * block_to_search_for) {
 // Come�ar a procura na cabe�a da lista //
 node * current_node = list_to_search.head;
 
 // Enquanto o fim da lista n�o tiver sido atingido //
 while (current_node != (node *) NULL) {
  // Se o endere�o-base do bloco atual � igual ao procurado //
  if (current_node->base_address == block_to_search_for) {
   // O n� foi encontrado //
   return (current_node);
  }
  // Se o endere�o-base do bloco atual n�o � igual ao procurado //
  else {
   // Passa para o pr�ximo bloco //
   current_node = current_node->next_node;
  }
 }
 // Se o c�digo chegar aqui, n�o foi encontrado nenhum n� com o bloco //
 return ((node *) NULL);
}

void list_print (linked_list list_to_print) {
 node * node_to_print;

 // Come�ar a impress�o da lista pela sua cabe�a //
 node_to_print = list_to_print.head;
 // Enquanto o fim da lista n�o tiver sido atingido //
 while (node_to_print != (node *) NULL) {
  // Imprimir a informa��o do n� actual //
  node_print (node_to_print);
  // Saltar para o pr�ximo n� //
  node_to_print = node_to_print->next_node;
 }
}

void list_print_blocks (linked_list list_to_print) {
 node * node_to_print;
 // Come�ar pelo n� que est� � cabe�a da lista //
 node_to_print = list_to_print.head;
 // Enquanto o fim da lista n�o for atingido //
 while (node_to_print != (node *) NULL) {
  // Imprime o n� //
  node_print (node_to_print);
  // Passa para o pr�ximo n� //
  node_to_print = node_to_print->next_node;
 }
}

linked_list list_create (void) {
 linked_list new_list;
 
 // A lista est� inicialmente vazia //
 new_list.head = NULL;
 new_list.tail = NULL;
 // Devolver a lista //
 return new_list;
}

linked_list list_delete (linked_list list_to_delete) {
 node * next_node;

 // Se o fim da lista tiver sido atingido //
 if (list_to_delete.head == (node *) NULL) {
  // Por raz�es de seguran�a //
  list_to_delete.head = NULL;
  list_to_delete.tail = NULL;
  // Devolver a lista vazia //
  return (list_to_delete);
 }
 else {
  // Guardar temporariamente o apontador para o pr�ximo n� //
  next_node = list_to_delete.head->next_node;
  // Apagar o n� apontado pela cabe�a da lista //
  node_delete (list_to_delete.head);
  // A nova lista j� n�o ter� o n� apagado //
  list_to_delete.head = next_node;
  // Apagar (recursivamente) o resto da lista //
  return (list_delete (list_to_delete));
 }
}

linked_list list_insert_node (linked_list list_to_edit, node * node_to_insert) {
 // Se este � o primeiro n� da lista //
 if (list_to_edit.head == (node *) NULL) {
  // Tanto a head como a tail apontam para ele //
  list_to_edit.head = node_to_insert;
  list_to_edit.tail = node_to_insert;
 }
 // Se este n�o � o primeiro n� da lista //
 else {
  // Insere o n� no final da lista //
  list_to_edit.tail->next_node = node_to_insert;
  // O novo n� aponta tamb�m para o seu predecessor //
  node_to_insert->previous_node = list_to_edit.tail;
  // O �ltimo n� aponta sempre para NULL //
  node_to_insert->next_node = (node *) NULL;
  // Atualizar o apontador para o fim da lista //
  list_to_edit.tail = node_to_insert;
 }
 return (list_to_edit);
}

linked_list list_remove_node (linked_list list_to_edit, node * node_to_remove) {
 // Se o n� n�o existir na lista //
 if (list_node_exists (list_to_edit, node_to_remove) == false) {
  // Sair sem fazer nada //
  return (list_to_edit);
 }
 // Se o pr�ximo n� na lista n�o � nulo //
 if (node_to_remove->next_node != (node *) NULL) {
  // O predecessor do pr�ximo n� na lista ser� o predecessor do n� que est� a ser removido //
  node_to_remove->next_node->previous_node = node_to_remove->previous_node;
 }
 // Se o pr�ximo n� na lista � nulo //
 else {
  // A cauda da lista passa a ser o predecessor do n� que est� a ser removido //
  list_to_edit.tail = node_to_remove->previous_node;
 }
 // Se o n� anterior na lista n�o � nulo //
 if (node_to_remove->previous_node != (node *) NULL) {
  // O sucessor do n� anterior na lista ser� o sucessor do n� que est� a ser removido //
  node_to_remove->previous_node->next_node = node_to_remove->next_node;
 }
 // Se o n� anterior na lista � nulo //
 else {
  // A cabe�a da lista passa a ser o sucessor do n� que est� a ser removido //
  list_to_edit.head = node_to_remove->next_node;
 }
 // Libertar a mem�ria alocada ao n� //
 node_delete (node_to_remove);
 // Devolver a lista com o n� removido //
 return (list_to_edit);
}

bool list_node_exists (linked_list list_to_search, node * node_to_search_for) {
 node * current_node = list_to_search.head;
 
 while (current_node != ( node *) NULL) {
  // Se o endere�o-base do bloco atual � igual ao procurado //
  if (current_node == node_to_search_for) {
   // O n� foi encontrado //
   return (true);
  }
  // Se o endere�o-base do bloco atual n�o � igual ao procurado //
  else {
   // Passa para o pr�ximo bloco //
   current_node = current_node->next_node;
  }
 }
 // Se o c�digo chegar aqui, n�o foi encontrado nenhum n� com o bloco //
 return (false);    
}

node * node_create (void * base_address, unsigned long block_size, const char * comment) {
 // Alocar mem�ria para o n� //
 node * new_node = (node *) malloc (sizeof ( node));
 // Alocar mem�ria para o coment�rio //
 new_node->comment = (char *) malloc (sizeof (char) * (strlen (comment) + 1));
 // Inicializar todos os membros da estrutura com os devidos valores //
 new_node->base_address = base_address;
 new_node->block_size   = block_size;
 new_node->comment = strcpy (new_node->comment, comment);
 // Este n� ainda n�o est� ligado a nenhum outro //
 new_node->next_node = NULL;
 new_node->previous_node = NULL;
 // Devolver o n� criado com os devidos valores //
 return new_node;
}

node * node_delete (node * node_to_delete) {
 // Liberta a mem�ria alocada para o coment�rio //
 free (node_to_delete->comment);
 // Por raz�es de seguran�a //
 node_to_delete->comment = (char *) NULL;
 // Liberta a mem�ria alocada para o n� //
 free (node_to_delete);
 // Por raz�es de seguran�a //
 node_to_delete = ( node *) NULL;
 // Devolver um apontador para NULL //
 return ( node *) NULL;
}

void node_print (node * node_to_print) {
  printf ("Endereco Base: %p <> Tamanho: %d <> Comentario: %s\n", node_to_print->base_address, node_to_print->block_size, node_to_print->comment);
}

