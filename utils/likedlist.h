#include "../models/data.h"

#ifndef LINKEDILIST_H
#define LINKEDILIST_H

typedef struct ListItem {
  Term *data;
  struct ListItem *next;
} ListItem;

#endif
