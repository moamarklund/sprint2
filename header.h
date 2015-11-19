/**
 *  \file
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> //toupper
#include <string.h>

#define PAGESIZE 20


/**
 * The struct for a linked list
 */
 xstruct list{
  struct node *first; 
};
  
typedef struct list list_t;

/**
 * The struct for a ware
 */

struct ware{
  char name[64]; 
  char description[128]; 
  list_t *list; 
  int price;
};  
typedef struct ware ware_t;

/**
 * The struct for information about storage location and amount in a ware
 */

struct information{
  ware_t *elem; 
  char character; 
  int number; 
  int amount; 
};
typedef struct information information_t;

/**
 * The struct for a node containing information about a ware in a list 
 */
struct node{
  struct information info; 
  struct node *next; 
};
typedef struct node node_t;

/**
 * The struct for a node in a tree
 */
struct tree_node{
  ware_t elem; 
  struct tree_node *left; 
  struct tree_node *right; 
};
typedef struct tree_node tree_node_t;

/**
 * The struct for a binary search tree
 */
struct tree{
  tree_node_t *root; 
  int copy_flag;
  ware_t copy_elem;
  ware_t edited_elem;
};
typedef struct tree tree_t;



/**
 *  Checks if a char is a letter and makes it uppercase
 *  \param message a list of chars
 *  \returns the char in uppercase, or asks for a valid letter in case the input was a non valid character
 */
char ask_question_char(char message[]);

/**
 * Checks an input so that it is a valid int
 * \param message a list of chars
 * \returns the given input if it is an int, else asks for a valid int 
 */
int ask_question_int(char message[]);

/**
 * Checks a string for non valid characters (such as ints)
 * \param message a list of chars - the question from the program
 * \param return_message a list of chars given from an input from the user
 * \returns an error message in case the string contains unvalid characters
 */
void ask_question_string(char message[], char return_message[]);

/**
 * Searches for a ware in a tree
 * \param tree a pointer to a tree
 * \param position the position of the ware in the tree
 * \param found a boolean representating if the ware is found
 * \returns the ware searched for 
 */
ware_t *get_ware(tree_node_t *subtree, int index, bool *found);

/**
 * Calculates the total amount for ware by searching through its storage locations
 * \param list a pointer to a list
 * \returns an int representing the total amount 
 */
int total_amount(list_t *list);

/**
 * Prints all the storage locations where a given ware is placed
 * \param ware a pointer to a ware
 */
void print_custom(ware_t *ware);

/**
 * Prints all info about a single ware
 * \param a pointer to a ware
 */
void print_ware(ware_t *ware);

/**
 * Checks if there already is a ware in the tree with the same name as given to the function
 * \param tree a pointer to a tree
 * \param name a pointer to a name to compare with
 * \returns true if there already was a ware with that name, otherwise false 
 */
bool exists(tree_node_t *tree, char *name);

/**
 * Checks if a given storage location already is taken
 * \param tree a pointer to a tree
 * \param info information about a ware
 * \param exists a boolean representing if the storage location was taken or not
 * \returns true if the storage location was taken, otherwise false 
 */
bool spot_exists(tree_node_t *tree, information_t *ware, bool exists);



/**
 * Creates a new ware and adds it to the tree 
 *  \param tree a pointer to the tree
 *  \returns After checking if the storage location is taken, or if there already exists a ware with the same name- adds the new ware to the tree
 */
void add_goods(tree_t *tree);

/**
 * Asks for a number and if there are just one storage location for that ware it removes the ware  * with that index from the tree, otherwise it asks which storage location the user wants to remove
 *  \param db a pointer to the tree
 */
void remove_goods(tree_t *tree);

/**
 * Asks for a number, shows the ware on that location and asks what the user wants to edit
 *  \param tree a pointer to the tree
 * \returns "Platsen är upptagen" if the storage location is taken, else it just adds the changes  * to the chosen ware. 
 */
void edit_goods(tree_t *tree);
  
/**
 * Undo the last change made in the program
 *  \param tree a pointer to the tree
 * \returns "Du måste göra något innan du kan ångra" in case no changes in the program has been made, else it returns "Ångrade .." with the ware that´s been changed or removed. 
 */
void undo_goods(tree_t *tree);

/**
 * Lists all wares in the tree and gives an option to show more about a single ware or to show more wares (only lists 20 wares at the time)
 *  \param tree a pointer to the tree
 */
void list_goods(tree_t *tree);

/**
 * Asks for wares to store in a pallet and lists the pallet with the info name, storagelocation an * d amount
 *  \param tree a pointer to the tree
 */
void load_goods(tree_t *tree);

#ifndef __tree_h__
#define __tree_h__


/**
 * Inserts a new ware to a tree
 *  \param tree a pointer to a tree
 *  \param elem the ware to be inserted
 *  \returns true if the insertion succeeded
 */
bool tree_insert(tree_node_t *tree, ware_t *elem);

/**
 * Removes an element from a tree
 *  \param tree a pointer to a tree
 *  \param elem a pointer to a ware
 * \returns true if the remove succeeded 
 */
bool tree_remove(tree_t *tree, ware_t *elem);

/**
 * Compares two names of wares
 *  \param tree a pointer to a node
 *  \param name the name of a ware
 * \returns zero if the names are equal 
 */
int compare_name(tree_node_t *tree, char *name);

/**
 *Compares if a given storagelocation already is taken by some other ware
 *  \param tree a pointer to a tree
 *  \param info information about a ware
 * \returns 0 if the storage location is taken, otherwise it returns 1 
 */
int compare_spot(tree_node_t *tree, information_t *ware);

/**
 * Calculates the depth of a tree
 *  \param tree a pointer to a tree
 * \returns an int representing the depth of the tree
 */
int tree_depth(tree_node_t *tree);

/**
 * Calculates the size of a tree
 *  \param tree a pointer to a tree
 * \returns an int representing the size of the tree
 */
int tree_size(tree_node_t *tree);

/**
 * Prints out all the names of the wares in a tree in alphabetical order
 *  \param subtree a pointer to a node
 *  \param index an int representing an index of a node in the tree
 *  \param wait an int TODO
 *  \param stop TODO
 */
void tree_print(tree_node_t *tree, int *index, int *wait, int stop);
#endif

#ifndef __list_h__
#define __list_h__



/**
 * Checks if a list is empty
 *  \param list a pointer to a struct list
 * \returns true if the list is empty 
 */
bool isEmpty(list_t *list);

/**
 * Inserts a new node in a list
* \param list a pointer to a list
* \param index an int to represent an index in the list
* \param info information about a ware
* \returns true if the node was succesfullly inserted to the list
*/
bool list_insert(list_t *list, int index, information_t *info);

/**
 *Calculates the length of a given list
 * \param list a pointer to a list
 * \returns the length of the given list as an int 
 */
int list_length(list_t *list);

/**
 * Removes a node from the list at a given index
 * \param list a pointer to a list
 * \param index an int to represent an index in the list
 * \returns true if the node was succesfully removed
 */
bool list_remove(list_t *list, int index);

#endif
