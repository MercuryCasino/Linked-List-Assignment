// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/*
  *Allocates a new empty list.
  *Returns a pointer to the newlu allocated listm or NULL if memory fails
*/

list_t *list_alloc() { 
  list_t* mylist =  (list_t *) malloc(sizeof(list_t)); 
  if (mylist != NULL){
    mylist->head = NULL;
  }
  return mylist;
}

/*
  -Frees all memory associated with the list, including all nodes.
  -Gracefully handles NULL list inputs.
*/

void list_free(list_t *l) {
  if (l == NULL) return;

  node_t *curr = l->head;
  node_t *temp;

  while (curr != NULL){
    temp = curr;
    curr = curr->next;
    free(temp);
  }

  free(l);
}
/*
  -Prints the elements of the list sequentially.
*/

void list_print(list_t *l) {
  if (l == NULL) return;

  node_t *curr = l->head;
  while (curr != NULL){
    printf("%d -> ", curr->value);
    curr = curr->next;
  }
  printf("NULL\n");
}

/*
  -Converts the list into a string representation.
  -Dynamically resizes the buffer to prevent overflow on very large lists.
  -Returns a dynamically allocated string (caller must free).
*/

char * listToString(list_t *l) {
  if (l == NULL) return NULL;

  int capacity = 1024;
  char* buf = (char *) malloc(sizeof(char) * capacity);
  if (buf == NULL) return NULL;

  buf[0] = '\0';
  char tbuf[32];

	node_t* curr = l->head;
  while (curr != NULL) {
    snprintf(tbuf, sizeof(tbuf), "%d->", curr->value);
    
    int current_len = strlen(buf);
    int append_len = strlen(tbuf);

    // Check if we need more memory before concatenting
    if (current_len + append_len +5 > capacity){
      capacity *= 2;

      // Use a temp pointer to avoid nenmory leaks if realloc fails
      char *temp = (char *) realloc(buf, sizeof(char) * capacity);
      if (temp == NULL){
        free(buf);
        return NULL;
      }
      buf = temp;
    }
    
    strcat(buf, tbuf);
    curr = curr->next;
  }
  strcat(buf, "NULL");
  return buf;
}
/*
  -Returns the total number of nodes in the list.
  -Returns 0 if list is empty or NULL.
*/

int list_length(list_t *l) { 
  if (l == NULL) return 0;

  int count = 0;
  node_t *curr = l->head;
  while (curr != NULL){
    count++;
    curr = curr->next;
  }
  return count;
 }

 /*
  -Helper function: Allocates and initalizes a new node.
  -Returns a pointer to the new node, or NULL if memory allocation fails.
 */

 node_t * getNode(elem value){
  node_t *mynode = (node_t *) malloc(sizeof(node_t));
  if (mynode != NULL){
    mynode->value = value;
    mynode->next = NULL;
  }
  return mynode;
 }

 /*
  -Adds a new element to the back (tail) of the list.
  -Traverses the list in O(n) time to find the end.
 */

void list_add_to_back(list_t *l, elem value) {
  if (l == NULL) return;

  node_t *new_node = getNode(value);

  if (l->head == NULL){
    l->head = new_node;
    return;
  }

  node_t *curr = l->head;
  while (curr->next != NULL){
    curr = curr->next;
  }
  curr->next = new_node;
}

/*
  -Adds a new element to the front (head) of the list.
*/
void list_add_to_front(list_t *l, elem value) {
     if (l == NULL) return;

     node_t *new_node = getNode(value);
     if (new_node == NULL) return;

     new_node->next = l->head;
     l->head = new_node;
}

/*
  -Adds a new element at a specific index
  -Ignores invalid indices or indices greater than list_length + 1
*/
void list_add_at_index(list_t *l, elem value, int index){
  if (l == NULL || index < 1) return;

  if (index ==1){
    list_add_to_front(l, value);
    return;
  }

  node_t *curr = l->head;
  int curr_idx = 1;
  
  // Traverse to the node immediately before the insertion point
  while(curr!= NULL && curr_idx < index - 1){
    curr = curr->next;
    curr_idx++;
  }

  if (curr != NULL){
    node_t *new_node = getNode(value);
    if (new_node != NULL){
      new_node->next = curr->next;
      curr->next = new_node;
    }
  }
}

/*
  -Removes and returns the element at the back of the list.
  -Returns -1 if the list is empty.
*/
elem list_remove_from_back(list_t *l) { 
  if (l== NULL || l->head == NULL) return -1;

  node_t *curr = l->head;

  if (curr->next == NULL){
    elem val = curr->value;
    free(curr);
    l->head = NULL;
    return val;
  }

  // Traverse to the second-to-last node
  while (curr->next->next != NULL){
    curr = curr ->next;
  }

  elem val = curr ->next->value;
  free(curr->next);
  curr->next = NULL;
  return val;
 }

 /*
  -Removes and returns the element at the front of the list.
  -Returns -1 if the list is empty.
 */
elem list_remove_from_front(list_t *l) { 
  if (l == NULL || l->head == NULL) return -1;

  node_t *curr = l->head;
  elem val = curr->value;

  l->head = curr->next;
  free(curr);
  return val;
 }

 /*
  -Removes and returns the element at a specific index.
  -Returns -1 if the index is out of bounds or list is empty.
 */
elem list_remove_at_index(list_t *l, int index) { 
  if (l==NULL || l->head == NULL || index < 1) return -1;

  if (index == 1){
    return list_remove_from_front(l);
  }

  node_t *curr = l->head;
  int curr_idx = 1;
  
  // Traverse to the node immediately before the deletion point
  while (curr->next != NULL && curr_idx < index - 1){
    curr = curr->next;
    curr_idx++;
  }
  
  // Check if index exceeds list length
  if (curr->next == NULL){
    return -1;
  }

  node_t *target = curr->next;
  elem val = target->value;
  curr->next = target->next;
  free(target);

  return val;
}

/*
  -Checks if a given value exist within the list.
  -Returns true if found, false otherwise.
*/

bool list_is_in(list_t *l, elem value) { 
  if (l == NULL) return false;
  
  node_t *curr = l->head;
  while (curr != NULL){
    if (curr->value == value){
      return true;
    }
    curr = curr->next;
  }

  return false;
}

/*
  -Retrieves the element at a specific index.
  -Returns -1 if index is invalid or out of bounds.
*/
elem list_get_elem_at(list_t *l, int index) { 
  if (l == NULL || index < 1) return -1;

  node_t *curr = l->head;
  int curr_idx = 1;

  while (curr != NULL && curr_idx < index){
    curr = curr->next;
    curr_idx++;
  }

  if (curr != NULL){
    return curr->value;
  }

  return -1;
 }

 /*
  -Finds the first 1-based index where a given element appeats.
  -Returns -1 if the element is not found.
 */
int list_get_index_of(list_t *l, elem value) { 
  if (l==NULL) return -1;

  node_t *curr = l->head;
  int curr_idx = 1;

  while (curr != NULL){
    if (curr->value == value){
      return curr_idx;
    }
    curr = curr->next;
    curr_idx++;
  }

  return -1;
 }

