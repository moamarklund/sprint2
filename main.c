#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "header.h"
#include "list.c"
#include "asker.c"
#include "utility.c"
#include "tree.c"

enum flagtype{
  ADD,
  EDIT,
  REMOVE,
  UNCHANGED
};


bool exit_program(tree_t *tree){
  int size = tree_size(tree->root);
  bool found = false, running = true;
  ware_t *current;
  
  switch(ask_question_char("\nÄr du säker på att du vill avsluta?\n[J]a eller [N]ej _")){
  case 'J':
    while(running){
      current = get_ware(tree->root, 1, &found);
      if(current != &tree->root->elem){
	tree_remove_aux(tree->root->left, tree->root, current);
      } else {
	running = false;
      }
      found = false;
    }
    running = true;
    while(running){
      size = tree_size(tree->root);
      current = get_ware(tree->root, size, &found);
      if(current != &tree->root->elem){
	tree_remove_aux(tree->root->right, tree->root, current);
      } else {
	running = false;
      }
      size--;
      found = false;
    }
    tree_remove(tree, get_ware(tree->root, 1, &found));
    return false; 
    break;
  case 'N': return true; break;
  default:  return exit_program(tree);
  }
}


void add_goods(tree_t *tree){
  bool running = true, dupe = false;
  ware_t ware;
  
  printf("=================================\n");
  ask_question_string("Ange namnet på varan: ", ware.name);
  if (exists(tree->root, ware.name)){
    puts("Varan finns redan, använder samma info.");
  } else {
    ask_question_string("Beskriv varan: ", ware.description);
    ware.price = ask_question_int("Ange priset för varan: ");
  }
  
  information_t info;
  while(running){
    info = (information_t){
      .character=ask_question_char("Ange hylla: "),
      .number=ask_question_int("Ange plats: ")
    };
    if(spot_exists(tree->root, &info, dupe) == true){
      puts("Platsen redan tagen, ange en annan.");
    } else {
      running = false;
    }
  }
  info.amount=ask_question_int("Ange antal: ");
  ware.list = list_new();
  list_append(ware.list, &info);

  tree->copy_flag = ADD;
  tree->copy_elem = ware;
  if(tree->root == NULL){
    tree->root = new_tree_node(ware);
  } else{
    tree_insert(tree->root, &ware);
  }
}


void remove_goods(tree_t *tree){
  bool running = true, found = false;
  int index = 1, page = 1, wait = 0;
  char answer;
  if(tree->root == NULL){
    printf("Finns inget att ta bort.\n");
    answer = ask_question_char("Gå till[b]aka: ");
    if(answer == 'B'){
      running = false;
    }
  }
  while(running){
    tree_print(tree->root, &index, &wait, (page*PAGESIZE));
    index = ((page*PAGESIZE)+1);
    wait = page*PAGESIZE;
    
    answer = ask_question_char("=================================\n[V]älj en vara att ta bort.\nVisa [m]er\n[A]vsluta.\n_ ");
    
    if(answer == 'A'){
      running = false;
    } else if(answer == 'M'){
      if(tree_size(tree->root) > (page*PAGESIZE)){
	page++;
      } else {
	puts("Finns inga fler varor att visa!");
	running = false;
      }
    } else if(answer == 'V'){
      int value = ask_question_int("Ange ett nummer _");
      ware_t *current = get_ware(tree->root, value, &found);
      print_custom(current);
      printf("\nVilken plats ska tas bort? (1-%i) ", list_length(current->list));
      value = ask_question_int("");
      answer = ask_question_char("Är du säker? [j]a/[n]ej ");
      if(answer == 'J'){
	tree->copy_flag = REMOVE;
	tree->copy_elem = *current;
	if(list_remove(current->list, (value-1))){
	  if(current->list->first == NULL){
	    free(current->list);
	    tree_remove(tree, current);
	  }
	  running = false;
	}
      }
      else{
	running = false;
      }
    }
  }
}


void edit_goods(tree_t *tree){
  bool running = true, found = false, exists = false;
  int index = 1, page = 1, wait = 0;
  char answer;
   if(tree->root == NULL){
    printf("Finns inget att redigera.\n");
    answer = ask_question_char("Gå till[b]aka: ");
    if(answer == 'B'){
      running = false;
    }
  }
  while(running){
    clear_scr();
    tree_print(tree->root, &index, &wait, (page*PAGESIZE));
    index = ((page*PAGESIZE)+1);
    wait = page*PAGESIZE;
    
    answer = ask_question_char("=================================\n[V]älj en vara att redigera.\nVisa [m]er\n[A]vsluta.\n_ ");
    
    if(answer == 'A'){
      running = false;
    }
    else if(answer == 'M'){
      if(tree_size(tree->root) > (page*PAGESIZE)){
	page++;
      } else {
	puts("Finns inga fler varor att visa!");
	running = false;
      }
    }
    else if(answer == 'V'){
      
      int value = ask_question_int("Ange numret på den vara du vill ändra:_ ");
      clear_scr();
      ware_t *current = get_ware(tree->root, value, &found);
      print_ware(current);

      tree->copy_flag = EDIT;
      tree->copy_elem = *current;
      
      while(running){
	char answer = ask_question_char("\nVälj vad du vill redigera\n[N]amn\n[B]eskrivning\n[P]ris\n[L]agerhylla\nAn[t]al\neller [A]vbryt:_");
	switch(answer){
	case 'N':
	  printf("Nuvarande namn: %s\n", current->name);
	  ask_question_string("Nytt namn:_", current->name);
	  break;
	case 'B':
	  printf("Nuvarande beskrivning: %s\n", current->description);
	  ask_question_string("Ny beskrivning:_", current->description);
	  break;
	case 'P':
	  printf("Nuvarande pris: %i\n", current->price);
	  current->price = ask_question_int("Nytt pris:_");
	  break;
	case 'L':
	  printf("Nuvarande lagerhylla: %c%i\n", current->list->first->info.character, current->list->first->info.number);
	  bool go = true;
	  information_t info;
	  while(go){
	    info = (information_t) {
	      .character=ask_question_char("Ange hylla: "),
	      .number=ask_question_int("Ange plats: ")
	    };
	    if(spot_exists(tree->root, &info, exists)){
	      puts("Platsen är upptagen");
	    }
	    else{
	      current->list->first->info.character = info.character;
	      current->list->first->info.number = info.number;
	      go = false;
	    }
	  }
	  break;
	case 'T':
	  printf("Nuvarande antal: %i\n", current->list->first->info.amount);
	  current->list->first->info.amount = ask_question_int("Nytt antal:_");
	  break;
	case 'A': running = false;
	  break;
	}
      }
      tree->edited_elem = *current;
    }
  }
}

void undo_goods(tree_t *tree){
  if(tree->copy_flag == UNCHANGED){
    ask_question_char("Du måste göra något innan du ångrar! Till[b]aka: ");
  } else if(tree->copy_flag == ADD){
    ask_question_char("Ångrade ADD, till[b]aka: ");
    tree_remove(tree, &tree->copy_elem);
  } else if(tree->copy_flag == REMOVE){
    ask_question_char("Ångrade REMOVE, till[b]aka: ");
    tree_insert(tree->root, &tree->copy_elem);
  } else if(tree->copy_flag == EDIT){
    ask_question_char("Ångrade EDIT, till[b]aka: ");
    tree_remove(tree, &tree->edited_elem);
    tree_insert(tree->root, &tree->copy_elem);
  }
  tree->copy_flag = UNCHANGED;
}



void list_goods(tree_t *tree){
  bool running = true, found = false;
  int page = 1, index = 1, wait = 0;
  char answer;
   if(tree->root == NULL){
    printf("Finns inga varor att lista.\n");
    answer = ask_question_char("Gå till[b]aka: ");
    if(answer == 'B'){
      running = false;
    }
  }
  while(running){
    clear_scr();
    tree_print(tree->root, &index, &wait, (page*PAGESIZE));
    index = ((page*PAGESIZE)+1);
    wait = page*PAGESIZE;
    
    answer = ask_question_char("=================================\n[V]älj ett nummer för att visa mer information om varan.\nVisa [m]er\n[A]vsluta.\n_ ");
    
    if(answer == 'A'){
      running = false;
    } else if(answer == 'M'){
      if(tree_size(tree->root) > (page*PAGESIZE)){
	page++;
      } else {
	clear_scr();
	ask_question_char("Finns inga fler varor att visa! till[b]aka: ");
	running = false;
      }
    } else if(answer == 'V'){
      int value = ask_question_int("Ange ett nummer _");
      clear_scr();
      ware_t *current = get_ware(tree->root, value, &found);
      print_ware(current);
      ask_question_char("till[b]aka: ");
      running = false;
    }
  }
}

void load_goods(tree_t *tree){
  bool running = true, found = false;
  int page = 1, index = 1, wait = 0;
  char answer;
  if(tree->root == NULL){
    printf("Finns inget att packa.\n");
    answer = ask_question_char("Gå till[b]aka: ");
    if(answer == 'B'){
      running = false;
    }
  }
  list_t *pall = list_new();
   
  while(running){
    clear_scr();
    node_t *list = pall->first;
    if(pall->first == NULL){
      printf("Din pall är tom\n\n");
    } else {
      printf("Din pall innehåller:\n\n");
      printf("%s (%i)\n", pall->first->info.elem->name, pall->first->info.amount);
      while(list->next != NULL){
	list = list->next;
	printf("%s (%i)\n", list->info.elem->name, list->info.amount);
      }
      printf("\n");
    }
    printf("=================================\n");
    tree_print(tree->root, &index, &wait, (page*PAGESIZE));
    index = ((page*PAGESIZE)+1);
    wait = page*PAGESIZE;
    
    answer = ask_question_char("=================================\n[V]älj en vara att packa.\nVisa [m]er\n[F]ärdig!.\n_ ");
  
    if(answer == 'F'){
      running = false;
    } else if(answer == 'M'){
      if(tree_size(tree->root) > (page*PAGESIZE)){
	page++;
      } else {
	clear_scr();
	puts("Finns inga fler varor att visa!");
	page = 1;
	index = 1;
	wait = 0;
	found = false;
	ask_question_char("Gå till[b]aka till början av listan: ");
      }
    } else if(answer == 'V'){
      int value = ask_question_int("Ange ett nummer _");
      ware_t *current = get_ware(tree->root, value, &found);
      bool go = true;
      while(go){
	printf("Namn: %s\nAntal: %d\n", current->name, total_amount(current->list));
	printf("Hur många skaru ha? (0-%d) ", total_amount(current->list));
	value = ask_question_int("");
	if(0 > value || value > total_amount(current->list)){
	  printf("\nAnge ett giltigt nummer\n");
	} else {
	  go = false;
	}
      }
      information_t info;
      info = (information_t) {
	.elem= current,
	.amount= value,
      };
      list_append(pall, &info);
      answer = ask_question_char("Vill du packa mer? [J]a/[N]ej ");
      if(answer == 'J'){
	page = 1;
	index = 1;
	wait = 0;
	found = false;
      }
      if(answer == 'N'){
	running = false;
      }
    }
  }
  while(tree->root != NULL){
  node_t *list = pall->first;
  information_t info = list->info;
  int total_price = 0;
  clear_scr();
  printf("Slutgiltig pall:\n\n");

  printf("%s (%i)\n", info.elem->name, info.amount);
  total_price = (info.elem->price * info.amount);
  while(list->next != NULL){
    list = list->next;
    printf("%s (%i)\n", list->info.elem->name, list->info.amount);
    total_price += (list->info.elem->price * list->info.amount);
  }

  printf("\nTotalkostnad: %i\n", total_price);
  printf("\nPlatser att packa: \n");
  list = pall->first;
  node_t *ware_info = info.elem->list->first;
  for(int i = 0; i < list_length(pall); i++){
    while(info.amount > ware_info->info.amount){
      printf("%c%i (%i %s)\n", ware_info->info.character, ware_info->info.number, ware_info->info.amount, info.elem->name);
      info.amount -= ware_info->info.amount;
      ware_info = ware_info->next;
    }
    printf("%c%i (%i %s)\n", ware_info->info.character, ware_info->info.number, info.amount, info.elem->name);
    if(list->next != NULL){
      info = list->next->info;
      ware_info = info.elem->list->first;
    }
  }
  ask_question_char("\ntill[b]aka: ");
  }
}




int main(int argc, char *argv[]){  
  
  information_t info1 = (information_t) {.character='A', .number=4, .amount=40};
  information_t info2 = (information_t) {.character='B', .number=4, .amount=30};
  information_t info3 = (information_t) {.character='C', .number=4, .amount=20};
  information_t info4 = (information_t) {.character='D', .number=4, .amount=20};
  information_t info5 = (information_t) {.character='E', .number=4, .amount=20};
  information_t info6 = (information_t) {.character='F', .number=4, .amount=20};
  information_t info7 = (information_t) {.character='G', .number=4, .amount=40};
  information_t info8 = (information_t) {.character='H', .number=4, .amount=30};
  information_t info9 = (information_t) {.character='I', .number=4, .amount=20};
  information_t info10 = (information_t) {.character='J', .number=4, .amount=20};
  information_t info11 = (information_t) {.character='K', .number=4, .amount=20};
  information_t info12 = (information_t) {.character='L', .number=4, .amount=20};
  
  list_t *list1 = list_new();
  list_t *list2 = list_new();
  list_t *list3 = list_new();
  list_t *list4 = list_new();
  list_t *list5 = list_new();
  list_t *list6 = list_new();
  list_t *list7 = list_new();
  list_t *list8 = list_new();
  list_t *list9 = list_new();
  list_t *list10 = list_new();
  list_t *list11= list_new();
  list_t *list12 = list_new();
  
  list_append(list1, &info1);
  list_append(list2, &info2);
  list_append(list3, &info3);
  list_append(list4, &info4);
  list_append(list5, &info5);
  list_append(list6, &info6);
  list_append(list7, &info7);
  list_append(list8, &info8);
  list_append(list9, &info9);
  list_append(list10, &info10);
  list_append(list11, &info11);
  list_append(list12, &info12);
  
  ware_t test1 = (ware_t) {.name="katt", .description="katt", .list=list1, .price=1230};
  ware_t test2 = (ware_t) {.name="btt", .description="katt", .list=list2, .price=1230};
  ware_t test3 = (ware_t) {.name="catt", .description="katt", .list=list3, .price=1230};
  ware_t test4 = (ware_t) {.name="patt", .description="katt", .list=list4, .price=1230};
  ware_t test5 = (ware_t) {.name="att", .description="katt", .list=list5, .price=1230};
  ware_t test6 = (ware_t) {.name="batt", .description="katt", .list=list6, .price=1230};
  ware_t test7 = (ware_t) {.name="gfatt", .description="katt", .list=list7, .price=1230};
  ware_t test8 = (ware_t) {.name="kutt", .description="katt", .list=list8, .price=1230};
  ware_t test9 = (ware_t) {.name="xatt", .description="katt", .list=list9, .price=1230};
  ware_t test10 = (ware_t) {.name="statt", .description="katt", .list=list10, .price=1230};
  ware_t test11 = (ware_t) {.name="maatt", .description="katt", .list=list11, .price=1230};
  ware_t test12 = (ware_t) {.name="öatt", .description="katt", .list=list12, .price=1230};
  /*
  tree_t *tree;
  tree = calloc(1, sizeof(tree_t));

  list_t *list;
  list = calloc(1, sizeof(list_t));
  */
  tree_t *tree = new_tree();
  //tree->root = new_tree_node();
  //list = list_new();
  
  tree->root = new_tree_node(test1);
  tree_insert(tree->root, &test2);
  tree_insert(tree->root, &test3);
  tree_insert(tree->root, &test4);
  tree_insert(tree->root, &test5);
  tree_insert(tree->root, &test6);
  tree_insert(tree->root, &test7);
  tree_insert(tree->root, &test8);
  tree_insert(tree->root, &test9);
  tree_insert(tree->root, &test10);
  tree_insert(tree->root, &test11);
  tree_insert(tree->root, &test12);
  

  

  tree->copy_flag = UNCHANGED;

  bool running = true;

  char input;
  
  while(running){
    clear_scr();
    input = ask_question_char("\nVälkommen till lagerhantering 2.0\n=================================\n[L]ägga till en vara\n[T]a bort en vara\n[R]edigera en vara\nÅn[g]ra senaste ändringen\nLista [h]ela varukatalogen\n[p]acka en pall\n[A]vsluta\n\nVad vill du göra idag? _");
    
    switch(input){
    case 'L': clear_scr(); add_goods(tree); break;
    case 'T': clear_scr(); remove_goods(tree); break;
    case 'R': clear_scr(); edit_goods(tree); break;
    case 'G': clear_scr(); undo_goods(tree); break;
    case 'H': clear_scr(); list_goods(tree); break;
    case 'P': clear_scr(); load_goods(tree); break;
    case 'A': clear_scr(); running = exit_program(tree); break;
    default: puts("Okänt kommando.");
    }
  }
  return 0;
}

