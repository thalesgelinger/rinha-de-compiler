#include "../models/data.h"

#ifndef HASH_H
#define HASH_H

#define HASH_SIZE 100

typedef struct Node {
  char *key;
  Val *value;
  struct Node *next;
} Node;

typedef struct Hash {
  struct Node *list[HASH_SIZE];
} Hash;

Val *search(Hash *hash, char *key);

void insert_node(Hash *hash, char *key, Val *value);

#endif
