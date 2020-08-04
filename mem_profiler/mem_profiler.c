
#include <stdio.h>
#include "mem_profiler.h"


// A lista que irá conter a informação do mapa da memória //
linked_list profile_list;


void profile_start (void) {
 // Criar uma nova lista e assigná-la à lista que contém o perfil da memória //
 profile_list = list_create ();
}

void profile_stop (void) {
 // Apagar a lista que contém o perfil da memória //
 profile_list = list_delete (profile_list);
}

void * profile_malloc (size_t size) {
 // Usa a função standard do C para alocar a memória pedida //
 void * allocated_block = (void *) malloc (size);
 
 // Criar um nó para guardar a informação sobre o bloco alocado //
 node * new_node = node_create (allocated_block, size, "");
 // Inserir o nó na lista //
 profile_list = list_insert_node (profile_list, new_node);
 // Devolver o apontador para o bloco de memória alocado //
 return (allocated_block);
}

void * profile_malloc_with_comment (size_t size, const char * comment) {
 // Usa a função standard do C para alocar a memória pedida //
 void * allocated_block = (void *) malloc (size);
 
 // Criar um nó para guardar a informação sobre o bloco alocado //
 node * new_node = node_create (allocated_block, size, comment);
 // Inserir o nó na lista //
 profile_list = list_insert_node (profile_list, new_node);
 // Devolver o apontador para o bloco de memória alocado //
 return (allocated_block);
}

void * profile_free (void * block_to_free) {
 // Procurar o primeiro bloco não-livre cujo endereço-base seja igual àquele que se pretende libertar //
 node * profile_node = list_search_block (profile_list, block_to_free);
 // Se não foi encontrado nenhum bloco ainda por libertar //
 if (profile_node == (node *) NULL) {
  // Imprime uma mensagem de erro //
  fprintf (stderr, "ERRO : APLICACAO : profile_free : Tentativa de libertar bloco de memoria invalido.");   
  // Sai da aplicação //
  exit (1);
 }
 // Remover da lista o nó que guarda a informação relativa ao bloco que está a ser libertado //
 profile_list = list_remove_node (profile_list, profile_node);
 // Devolver o mesmo que a função standard do C devolve //
 return (void *) (free (block_to_free));
}

void profile_print (void) {
 list_print_blocks (profile_list);
}

node * list_search_block (linked_list list_to_search, void * block_to_search_for) {
 // Começar a procura na cabeça da lista //
 node * current_node = list_to_search.head;
 
 // Enquanto o fim da lista não tiver sido atingido //
 while (current_node != (node *) NULL) {
  // Se o endereço-base do bloco atual é igual ao procurado //
  if (current_node->base_address == block_to_search_for) {
   // O nó foi encontrado //
   return (current_node);
  }
  // Se o endereço-base do bloco atual não é igual ao procurado //
  else {
   // Passa para o próximo bloco //
   current_node = current_node->next_node;
  }
 }
 // Se o código chegar aqui, não foi encontrado nenhum nó com o bloco //
 return ((node *) NULL);
}

void list_print (linked_list list_to_print) {
 node * node_to_print;

 // Começar a impressão da lista pela sua cabeça //
 node_to_print = list_to_print.head;
 // Enquanto o fim da lista não tiver sido atingido //
 while (node_to_print != (node *) NULL) {
  // Imprimir a informação do nó actual //
  node_print (node_to_print);
  // Saltar para o próximo nó //
  node_to_print = node_to_print->next_node;
 }
}

void list_print_blocks (linked_list list_to_print) {
 node * node_to_print;
 // Começar pelo nó que está à cabeça da lista //
 node_to_print = list_to_print.head;
 // Enquanto o fim da lista não for atingido //
 while (node_to_print != (node *) NULL) {
  // Imprime o nó //
  node_print (node_to_print);
  // Passa para o próximo nó //
  node_to_print = node_to_print->next_node;
 }
}

linked_list list_create (void) {
 linked_list new_list;
 
 // A lista está inicialmente vazia //
 new_list.head = NULL;
 new_list.tail = NULL;
 // Devolver a lista //
 return new_list;
}

linked_list list_delete (linked_list list_to_delete) {
 node * next_node;

 // Se o fim da lista tiver sido atingido //
 if (list_to_delete.head == (node *) NULL) {
  // Por razões de segurança //
  list_to_delete.head = NULL;
  list_to_delete.tail = NULL;
  // Devolver a lista vazia //
  return (list_to_delete);
 }
 else {
  // Guardar temporariamente o apontador para o próximo nó //
  next_node = list_to_delete.head->next_node;
  // Apagar o nó apontado pela cabeça da lista //
  node_delete (list_to_delete.head);
  // A nova lista já não terá o nó apagado //
  list_to_delete.head = next_node;
  // Apagar (recursivamente) o resto da lista //
  return (list_delete (list_to_delete));
 }
}

linked_list list_insert_node (linked_list list_to_edit, node * node_to_insert) {
 // Se este é o primeiro nó da lista //
 if (list_to_edit.head == (node *) NULL) {
  // Tanto a head como a tail apontam para ele //
  list_to_edit.head = node_to_insert;
  list_to_edit.tail = node_to_insert;
 }
 // Se este não é o primeiro nó da lista //
 else {
  // Insere o nó no final da lista //
  list_to_edit.tail->next_node = node_to_insert;
  // O novo nó aponta também para o seu predecessor //
  node_to_insert->previous_node = list_to_edit.tail;
  // O último nó aponta sempre para NULL //
  node_to_insert->next_node = (node *) NULL;
  // Atualizar o apontador para o fim da lista //
  list_to_edit.tail = node_to_insert;
 }
 return (list_to_edit);
}

linked_list list_remove_node (linked_list list_to_edit, node * node_to_remove) {
 // Se o nó não existir na lista //
 if (list_node_exists (list_to_edit, node_to_remove) == false) {
  // Sair sem fazer nada //
  return (list_to_edit);
 }
 // Se o próximo nó na lista não é nulo //
 if (node_to_remove->next_node != (node *) NULL) {
  // O predecessor do próximo nó na lista será o predecessor do nó que está a ser removido //
  node_to_remove->next_node->previous_node = node_to_remove->previous_node;
 }
 // Se o próximo nó na lista é nulo //
 else {
  // A cauda da lista passa a ser o predecessor do nó que está a ser removido //
  list_to_edit.tail = node_to_remove->previous_node;
 }
 // Se o nó anterior na lista não é nulo //
 if (node_to_remove->previous_node != (node *) NULL) {
  // O sucessor do nó anterior na lista será o sucessor do nó que está a ser removido //
  node_to_remove->previous_node->next_node = node_to_remove->next_node;
 }
 // Se o nó anterior na lista é nulo //
 else {
  // A cabeça da lista passa a ser o sucessor do nó que está a ser removido //
  list_to_edit.head = node_to_remove->next_node;
 }
 // Libertar a memória alocada ao nó //
 node_delete (node_to_remove);
 // Devolver a lista com o nó removido //
 return (list_to_edit);
}

bool list_node_exists (linked_list list_to_search, node * node_to_search_for) {
 node * current_node = list_to_search.head;
 
 while (current_node != ( node *) NULL) {
  // Se o endereço-base do bloco atual é igual ao procurado //
  if (current_node == node_to_search_for) {
   // O nó foi encontrado //
   return (true);
  }
  // Se o endereço-base do bloco atual não é igual ao procurado //
  else {
   // Passa para o próximo bloco //
   current_node = current_node->next_node;
  }
 }
 // Se o código chegar aqui, não foi encontrado nenhum nó com o bloco //
 return (false);    
}

node * node_create (void * base_address, unsigned long block_size, const char * comment) {
 // Alocar memória para o nó //
 node * new_node = (node *) malloc (sizeof ( node));
 // Alocar memória para o comentário //
 new_node->comment = (char *) malloc (sizeof (char) * (strlen (comment) + 1));
 // Inicializar todos os membros da estrutura com os devidos valores //
 new_node->base_address = base_address;
 new_node->block_size   = block_size;
 new_node->comment = strcpy (new_node->comment, comment);
 // Este nó ainda não está ligado a nenhum outro //
 new_node->next_node = NULL;
 new_node->previous_node = NULL;
 // Devolver o nó criado com os devidos valores //
 return new_node;
}

node * node_delete (node * node_to_delete) {
 // Liberta a memória alocada para o comentário //
 free (node_to_delete->comment);
 // Por razões de segurança //
 node_to_delete->comment = (char *) NULL;
 // Liberta a memória alocada para o nó //
 free (node_to_delete);
 // Por razões de segurança //
 node_to_delete = ( node *) NULL;
 // Devolver um apontador para NULL //
 return ( node *) NULL;
}

void node_print (node * node_to_print) {
  printf ("Endereco Base: %p <> Tamanho: %d <> Comentario: %s\n", node_to_print->base_address, node_to_print->block_size, node_to_print->comment);
}

