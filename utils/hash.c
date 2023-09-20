#include "hash.h"
#include "../models/data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int hash_code(char *key) {
  unsigned int hash = 0;
  while (*key) {
    hash = (hash * 31) + (*key++);
  }
  return hash % HASH_SIZE;
}

Node *create_node(char *key, Val *value) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->key = key;
  node->value = value;
  return node;
}

void insert_node(Hash *hash, char *key, Val *value) {
  unsigned int index = hash_code(key);
  Node *node = hash->list[index];
  if (node == NULL) {
    hash->list[index] = create_node(key, value);
  } else {
    while (node) {
      if (node->next == NULL) {
        node->next = create_node(key, value);
        break;
      }
      node = node->next;
    }
  }
}

Val *search(Hash *hash, char *key) {
  unsigned int index = hash_code(key);
  Node *node = hash->list[index];
  while (node) {
    if (strcmp(node->key, key) == 0) {
      return node->value;
    }
  }
  return NULL;
}

void print_hash(Hash *hash) {
  for (int i = 0; i < HASH_SIZE; i++) {
    Node *current = hash->list[i];
    while (current != NULL) {
      printf("Key: %s, Value: %d\n", current->key,
             current->value->value
                 .intValue); // Replace some_value with the
                             // appropriate field in your Val struct
      current = current->next;
    }
  }
}
