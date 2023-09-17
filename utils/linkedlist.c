#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

struct ListItem *createNode(Term *data) {
  struct ListItem *newListItem = malloc(sizeof(ListItem));
  if (newListItem == NULL) {
    perror("Failed to allocate memory for a new node");
    exit(1);
  }
  newListItem->data = data;
  newListItem->next = NULL;
  return newListItem;
}

void insert_item(ListItem *list, Term *term) {
  struct ListItem *newNode = createNode(term);
  newNode->next = list;
}

void print_list(ListItem *list) {
  while (list->next) {
    printf("Items");
  }
}
