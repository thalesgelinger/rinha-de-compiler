
#include "../models/data.h"
#include "cJSON.h"
#include "printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cJSON *parse_json(char *json_data) {

  cJSON *root = cJSON_Parse(json_data);

  if (root == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
      fprintf(stderr, "Error before: %s\n", error_ptr);
    }
    cJSON_Delete(root);
    free(json_data);
  }

  free(json_data);

  return root;
}

void parse_location(Location *location, cJSON *jsonLocation) {

  cJSON *child = NULL;

  cJSON_ArrayForEach(child, jsonLocation) {
    if (strcmp(child->string, "start") == 0) {
      location->start = child->valueint;
    } else if (strcmp(child->string, "end") == 0) {
      location->end = child->valueint;
    } else if (strcmp(child->string, "filename") == 0) {
      strcpy(location->filename, child->valuestring);
    }
  }
}

void parse_expression(Term *term, cJSON *jsonExpression) {

  cJSON *child = NULL;

  cJSON_ArrayForEach(child, jsonExpression) {
    char *key = child->string;
    if (strcmp(key, "kind") == 0) {
      if (strcmp(child->valuestring, "Print") == 0) {
        term->kind = Print;
      } else if (strcmp(child->valuestring, "Str") == 0) {
        term->kind = Str;
      } else if (strcmp(child->valuestring, "Binary") == 0) {
        term->kind = Binary;
      } else if (strcmp(child->valuestring, "Int") == 0) {
        term->kind = Int;
      }
    } else if (strcmp(key, "location") == 0) {
      parse_location(&term->location, child);
    }

    switch (term->kind) {

    case Print: {
      if (strcmp(key, "value") == 0) {
        Term *printTerm = malloc(sizeof(Term));
        parse_expression(printTerm, child);
        term->data.printTerm.value = printTerm;
      }
    } break;

    case Str:
      if (strcmp(key, "value") == 0) {
        strcpy(term->data.strTerm.value, child->valuestring);
      }
      break;

    case Binary: {
      if (strcmp(key, "lhs") == 0) {
        Term *lhs = malloc(sizeof(Term));
        parse_expression(lhs, child);
        term->data.binaryTerm.lhs = lhs;
      } else if (strcmp(key, "rhs") == 0) {
        Term *rhs = malloc(sizeof(Term));
        parse_expression(rhs, child);
        term->data.binaryTerm.rhs = rhs;
      } else if (strcmp(key, "op") == 0) {
        char *value = child->valuestring;
        if (strcmp(value, "Add") == 0) {
          term->data.binaryTerm.op = Add;
        } else if (strcmp(value, "Mul") == 0) {
          term->data.binaryTerm.op = Mul;
        } else if (strcmp(value, "Sub") == 0) {
          term->data.binaryTerm.op = Sub;
        } else if (strcmp(value, "Div") == 0) {
          term->data.binaryTerm.op = Div;
        }
      }

    } break;
    case Int:
      if (strcmp(key, "value") == 0) {
        term->data.intTerm.value = child->valueint;
      }
      break;
    }
  }
}

void parse_program(File *file, char *json_data) {

  cJSON *json = parse_json(json_data);

  if (cJSON_IsObject(json)) {
    cJSON *child = NULL;
    cJSON_ArrayForEach(child, json) {
      char *key = child->string;

      if (strcmp(key, "name") == 0) {
        strcpy(file->name, child->valuestring);
      } else if (strcmp(key, "expression") == 0) {
        parse_expression(&file->expression, child);
      } else if (strcmp(key, "location") == 0) {
        parse_location(&file->location, child);
      }
    }
  }

  cJSON_Delete(json);
};

void parse_file(File *file, char *filepath) {

  FILE *code = fopen(filepath, "r");

  if (code == NULL) {
    perror("Error opening file");
  }

  fseek(code, 0, SEEK_END);

  long file_size = ftell(code);
  fseek(code, 0, SEEK_SET);

  char *json_data = (char *)malloc(file_size + 1);

  if (json_data == NULL) {
    perror("Memory allocation error");
    fclose(code);
  }

  fread(json_data, 1, file_size, code);
  fclose(code);

  json_data[file_size] = '\0';
  parse_program(file, json_data);
}
