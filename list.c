#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> //toupper
#include <string.h>
#include <assert.h>


list_t *list_new(){
  list_t *list = calloc(1,sizeof(list_t));
  return list;
}

node_t *newnode(information_t *info){
  node_t *new;
  new = malloc(sizeof(node_t));
  new->next = NULL;
  new->info = *info;
  return new;
}


bool isEmpty(list_t *list) {
  return (list->first == NULL);
}

node_t *list_last(list_t *list){
  assert(list != NULL);
  assert(!isEmpty(list));
  node_t *current = list->first;
  while(current->next != NULL){
    current = current->next;
  }
  return current;
}


void list_append(list_t *list, information_t *info){
  assert(list != NULL);
  if(!isEmpty(list)){
    list_last(list)->next = newnode(info);
  } else {
    list->first = newnode(info);
  }
}


void list_prepend(list_t *list, information_t *info){
  assert(list != NULL);
  node_t *new = newnode(info);
  if(!isEmpty(list)){
    new->next = list->first;
    list->first = new;
  } else {
    list->first = new;
  }
}


node_t *list_get(list_t *list, int index){
  node_t *current = list->first;
  while (index > 0){
    current = current->next;
    index--;
  }
  return current;
}


int list_length(list_t *list){
  int i = 1;
  node_t *current = list->first;
  while (current->next != NULL){
    i++;
    current = current->next;
  }
  return i;
}


bool list_insert(list_t *list, int index, information_t *info){
  node_t *current = list->first; 
  node_t *new = newnode(info);
  if(index == 0){
    list_prepend(list, info); 
    return true;
  }
  if(index == list_length(list)){
    list_append(list, info); 
    return true;
  } else {
    while(index > 0){
      current = current->next;
      index--;
    }
    new->next = current->next;
    current->next = new;
    return true;
  }
  return false;
}

bool list_remove(list_t *list, int index){
  node_t *current = list->first;
  node_t *temp;
  switch(index){
  case 0: 
    list->first = current->next; 
    free(current);
    return true;
  default:
    while(index > 0){
      temp = current;
      current = current->next;
      index--;
    }
    temp->next = current->next;
    free(current);
    return true;
  }
}

