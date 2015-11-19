#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> //toupper
#include <string.h>

tree_t *new_tree(){
  tree_t *new;
  new = calloc(1,sizeof(tree_t));
  new->root = NULL;
  return new;
}


tree_node_t *new_tree_node(ware_t elem){
  tree_node_t *new;
  new = calloc(1,sizeof(tree_node_t));
  new->elem = elem;
  new->left = NULL;
  new->right = NULL;
  return new;
}


int tree_size(tree_node_t *tree){
  int size = 1;
  if(tree->left != NULL){
    size += tree_size(tree->left);
  }
  if(tree->right != NULL){
    size += tree_size(tree->right);
  }
  return size;
}


int tree_depth(tree_node_t *tree){
  int ldepth, rdepth;
  if(tree == NULL){
    return 0;
  }
  else{
    ldepth = tree_depth(tree->left);
    rdepth = tree_depth(tree->right);
    if(ldepth > rdepth){
      return ldepth + 1;
    } else {
      return rdepth + 1;
    }
  }
}


void tree_print(tree_node_t *subtree, int *index, int *wait, int stop){
  if(subtree->left != NULL){
    tree_print(subtree->left, index, wait, stop);
  }
  if(*wait == 0){
    if(*index <= stop){
      printf("%d. %s\n", *index, subtree->elem.name);
      (*index)++;
    }
  } else {
    (*wait)--;
  }
  if(subtree->right != NULL){
    tree_print(subtree->right, index, wait, stop);
  }
}


int compare_name(tree_node_t *tree, char *name){
  char* node_name = tree->elem.name;
  return strcmp(node_name, name);
}


int compare_spot(tree_node_t *tree, information_t *info){
  for(int i = 0; i < list_length(tree->elem.list); i++){
    node_t *node = list_get(tree->elem.list, i);
    char str1 = node->info.character;
    char num1 = node->info.number;
    char str2 = info->character;
    char num2 = info->number;
    if(num1 == num2){
      if(str1 == str2){
	return 0;
      }
    }
  }
  return 1;
}


bool add_to_list(ware_t *ware, information_t *info){
  int i = 0;
  list_t *current = ware->list;
  while(i < list_length(current)){
    node_t *node = list_get(current, (i));
    if(info->amount > node->info.amount){
      return list_insert(current, i, info);
    } else {
      i++;
      if(i == list_length(current)){
	return list_insert(current, i, info);
      }
    }
  }
  return false;
}


bool tree_insert(tree_node_t *tree, ware_t *elem){
  int result = compare_name(tree, elem->name);
  if(result == 0){
    return add_to_list(&tree->elem, &(elem->list->first->info));
  }
  if(result > 0){
    if(tree->left == NULL){
      tree->left = new_tree_node(*elem);
      printf("hittade plats för %s på vänster sida\n", elem->name);
      return true;
    }
    puts("gick vänster");
    return tree_insert(tree->left, elem);
  }
  else if(result < 0){
    if (tree->right == NULL){
      tree->right = new_tree_node(*elem);
      printf("hittade plats för %s på höger sida\n", elem->name);
      return true;
    }
    puts("gick höger");
    return tree_insert(tree->right, elem);
  }
  return false;
}


void rearrange(tree_node_t *tree, tree_node_t *parent, ware_t *elem){
  int direction = 1;
  tree_node_t *new_node = calloc(1, sizeof(tree_t));
  if(parent->left != NULL){
    if(compare_name(parent->left, elem->name) == 0){
      direction = 0;
    }
  }
  if(direction == 0){
    tree_node_t *unwanted = parent->left;
    if(tree->left == NULL && tree->right == NULL){
      parent->left = NULL;
    }
    else if(tree->left != NULL){
      new_node->elem = tree->left->elem;
      new_node->left = tree->left->left;
      new_node->right = tree->right;
      parent->left = new_node;
    } else{
      parent->left = tree->right;
    }
    free(unwanted);
  } else{
    tree_node_t *unwanted = parent->right;
     if(tree->left == NULL && tree->right == NULL){
       parent->right = NULL;
     }
     else if(tree->left != NULL){
      new_node->elem = tree->left->elem;
      new_node->left = tree->left->left;
      new_node->right = tree->right;
      parent->right = new_node;
    } else{
      parent->right = tree->right;
    }
    free(unwanted);
  }
}

//
tree_node_t *new_root_aux(tree_node_t *tree){
  if(tree->right == NULL){
    return tree;
  } else {
    return new_root_aux(tree->right);
  }
} 

tree_node_t *find_new_root(tree_t *tree){
  tree_node_t *new_root;
  if(tree->root->left == NULL && tree->root->right == NULL){
    tree->root = NULL;
    new_root = NULL;
  }
  else if(tree->root->left != NULL){
    new_root = new_root_aux(tree->root->left);
  }
  else {
    new_root = tree->root->right;
  }
  return new_root;
}


bool tree_remove_aux(tree_node_t *tree, tree_node_t *parent, ware_t *elem){
  int result = compare_name(tree, elem->name);
  if(result == 0){
    puts("hittade element med samma namn!");
    rearrange(tree, parent, elem);
    return true;
  }
  if(result > 0){
    puts("letar vänster.");
    return tree_remove_aux(tree->left, tree, elem);
  }
  if(result < 0){
    puts("letar höger.");
    return tree_remove_aux(tree->right, tree, elem);
  }
  return false;
}


bool tree_remove(tree_t *tree, ware_t *elem){
  int result = compare_name(tree->root, elem->name);
  printf("\ntar bort %s\n", elem->name);
  if(result == 0){
    puts("hittade element med samma namn!");
    tree_node_t *new = malloc(sizeof(tree_node_t));
    tree_node_t *unwanted = tree->root;
    new = find_new_root(tree);
    if(new == NULL){
      free(new);
      free(unwanted);
      free(tree);
      return true;
    }
    new->left = tree->root->left;
    new->right = tree->root->right;
    tree->root = new;
    free(new);
    free(unwanted);
    if(tree->root->left != NULL){
      tree_remove_aux(tree->root->left, tree->root, &new->elem);
    } else if (tree->root->right != NULL){
      tree_remove_aux(tree->root->right, tree->root, &new->elem);
    }
    return true;
  }
  if(result > 0){
    puts("letar vänster");
    return tree_remove_aux(tree->root->left, tree->root, elem);
  }
  if(result < 0){
    puts("letar höger");
    return tree_remove_aux(tree->root->right, tree->root, elem);
  }
  return false;
}

