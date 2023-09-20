#include "data.h"
#include <stdio.h>
#include <stdlib.h>

ParametersList *init_params_list() {
  ParametersList *list = (ParametersList *)malloc(sizeof(ParametersList));
  if (list == NULL) {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  list->items = (Parameter **)malloc(1 * sizeof(Parameter *));
  if (list->items == NULL) {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  list->size = 0;

  return list;
}

void add_param(ParametersList *list, Parameter *element) {
  list->items[list->size] = element;

  list->items = (Parameter **)realloc(list->items,
                                      (list->size + 1) * sizeof(Parameter *));
  if (list->items == NULL) {
    printf("Memory reallocation failed.\n");
    exit(1);
  }
  list->size++;
}

void free_params(ParametersList *list) {
  for (int i = 0; i < list->size; i++) {
    free(list->items[i]);
  }

  free(list->items);
  free(list);
}

void print_params(ParametersList *list) {
  for (int i = 0; i < list->size; i++) {
    printf("Parameter: %s -> \n", list->items[i]->text);
  }
}

ArgsList *init_args_list() {
  ArgsList *list = (ArgsList *)malloc(sizeof(ArgsList));
  if (list == NULL) {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  list->items = (Term **)malloc(1 * sizeof(Term *));
  if (list->items == NULL) {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  list->size = 0;

  return list;
}

void add_arg(ArgsList *list, Term *element) {
  list->items[list->size] = element;
  list->items =
      (Term **)realloc(list->items, (list->size + 1) * sizeof(Term *));

  if (list->items == NULL) {
    printf("Memory reallocation failed.\n");
    exit(1);
  }

  list->size++;
}

void free_args(ArgsList *list) {
  for (int i = 0; i < list->size; i++) {
    free(list->items[i]);
  }

  free(list->items);
  free(list);
}

void print_args(ArgsList *list) {
  for (int i = 0; i < list->size; i++) {
    printf("Argument Kind: %d -> \n", list->items[i]->kind);
    printf("Argument Value: %d -> \n", list->items[i]->data.intTerm.value);
  }
}
