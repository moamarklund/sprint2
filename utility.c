#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> //toupper
#include <string.h>
#include <assert.h>

void clear_scr(){
  for(int i = 0; i < 30; i++){
    puts("\n");
  }
}


ware_t *get_ware_aux(tree_node_t *tree, ware_t *ware, int *index, int pos, bool *found){
  if(*found == true){
    return ware;
  }
  if(tree->left != NULL){
    ware = get_ware_aux(tree->left, ware, index, pos, found);
  }
  if(*index == pos && *found == false){
    *found = true;
    ware = &tree->elem;
  }
  (*index)++;
  if(tree->right != NULL){
    ware = get_ware_aux(tree->right, ware, index, pos, found);
  }
  return ware;
}


 ware_t *get_ware(tree_node_t *tree, int position, bool *found){
  ware_t ware;
  int index = 1;
  return get_ware_aux(tree, &ware, &index, position, found); 
}


int total_amount(list_t *list){
  node_t *current;
  int total = 0;
  for(int i = 0; i < (list_length(list)); i++){
    current = list_get(list, i);
    total += current->info.amount;
  }
  return total;
}


void print_ware(ware_t *ware){
  node_t *current;
  int total = 0;

  printf("=================================\nVara:\t\t%s\nBeskrivning:\t%s\n", ware->name, ware->description);
  
  printf("Plats:\t\t");
  for(int i = 0; i < (list_length(ware->list)); i++){
    current = list_get(ware->list, i);
    printf("%c%i, ", current->info.character, current->info.number);
  }
  printf("\nPris:\t\t%i", ware->price);
  total = total_amount(ware->list);
  printf("\nAntal:\t\t%i\n=================================\n", total);
}


void print_custom(ware_t *ware){
  node_t *list = ware->list->first;
  clear_scr();
  printf("%s finns på platserna:\n\n", ware->name);
  printf("%c%i (%i)\n", list->info.character, list->info.number, list->info.amount);
  while(list->next != NULL){
    list = list->next;
    printf("%c%i (%i)\n", list->info.character, list->info.number, list->info.amount);
  }
}

bool exists(tree_node_t *tree, char *name){
  if(tree == NULL){
    return false;
  }
  if(compare_name(tree, name) == 0){
    return true;
  } else {
    if(tree->left != NULL){
      return exists(tree->left, name);
    } else if(tree->right != NULL){
      return exists(tree->right, name);
    }
  }
  return false;
}

bool spot_exists(tree_node_t *tree, information_t *info, bool exists){
  if(tree == NULL){
    exists = false;
  }
  else if(compare_spot(tree, info) == 0){
    exists = true;
  } else {
    if(tree->left != NULL){
      exists = spot_exists(tree->left, info, exists);
    }
    if(tree->right != NULL){
      exists = spot_exists(tree->right, info, exists);
    }
  }
  return exists;
}

