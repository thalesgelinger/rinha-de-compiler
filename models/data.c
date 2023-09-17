#include "data.h"
#include <stdio.h>
#include <stdlib.h>

Parameters *create_parameter(Parameters *previous, Parameter *parameter) {
  Parameters *newParameter = malloc(sizeof(Parameters));
  newParameter->value = parameter;
  newParameter->previous = previous;
  return newParameter;
}

void print_parameters(Parameters *head) {
  Parameters *iterator = head;
  while (iterator) {
    printf("Parameter: %s -> \n", iterator->value->text);
    iterator = iterator->previous;
  }
}

Arguments *create_argument(Arguments *previous, Term *term) {
  struct Arguments *newArgument = malloc(sizeof(Arguments));
  newArgument->value = term;
  newArgument->previous = previous;
  return newArgument;
}

void print_arguments(Arguments *head) {
  // TODO
  Arguments *iterator = head;
  while (iterator) {
    printf("Argument Kind: %d -> \n", iterator->value->kind);
    iterator = iterator->previous;
  }
}
