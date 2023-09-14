#include "../models/data.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>

void print_location(Location *location) {
  printf("location.filename: %s\n", location->filename);
  printf("location.start: %d\n", location->start);
  printf("location.end: %d\n", location->end);
}

void print_term(Term *term) {
  printf("Kind: %u\n", term->kind);
  print_location(&term->location);
  switch (term->kind) {
  case Print:
    printf("Print");
    print_term(term->data.printTerm.value);
  case Str:
    printf("Str: %s\n", term->data.strTerm.value);
    break;
  case Binary:
    printf("Binary");
    print_term(term->data.binaryTerm.lhs);
    print_term(term->data.binaryTerm.rhs);
    break;
  case Int:
    printf("Int: %d\n", term->data.intTerm.value);
    break;
  case Bool:
    printf("Bool: %d\n", term->data.boolTerm.value);
    break;
  case Call:
    break;
  case Function:
    break;
  case Let:
    break;
  case If:
    break;
  case First:
    break;
  case Second:
    break;
  case Tuple:
    break;
  case Var:
    break;
  }
}

void print_file(File *file) {
  printf("File name: %s\n", file->name);

  print_location(&file->location);
  printf("File expression.location.start %d\n",
         file->expression.location.start);
  printf("File expression.location.end %d\n", file->expression.location.end);
}

void print_json(cJSON *json) {
  if (cJSON_IsObject(json)) {
    cJSON *child = NULL;
    cJSON_ArrayForEach(child, json) {
      printf("YABADABA\n");
      printf("Key: %s, Value: ", child->string);

      // Recursively print the child element
      print_json(child);

      printf("\n");
    }
  } else if (cJSON_IsArray(json)) {
    cJSON *child = NULL;
    int i = 0;
    cJSON_ArrayForEach(child, json) {
      printf("Index: %d, Value: ", i);

      // Recursively print the child element
      print_json(child);

      printf("\n");
      i++;
    }
  } else if (cJSON_IsString(json)) {
    printf("\nSTRING\n");
    printf("%s", json->valuestring);
  } else if (cJSON_IsNumber(json)) {
    printf("\nDOBULE\n");
    printf("%f", json->valuedouble);
  } else if (cJSON_IsBool(json)) {
    printf("\nBOOLEAN\n");
    printf("%s", cJSON_IsTrue(json) ? "true" : "false");
  } else if (cJSON_IsNull(json)) {
    printf("\nNULL\n");
    printf("null");
  }
}
