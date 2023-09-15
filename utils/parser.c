
#include "../models/data.h"
#include "cJSON.h"
#include "printer.h"
#include <stdbool.h>
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

void parse_parameter(Parameter *parameter, cJSON *jsonExpression) {

  cJSON *child = NULL;

  cJSON_ArrayForEach(child, jsonExpression) {
    char *key = child->string;
    if (strcmp(key, "text") == 0) {
      strcpy(parameter->text, child->valuestring);
    } else if (strcmp(key, "location") == 0) {
      parse_location(&parameter->location, child);
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
      } else if (strcmp(child->valuestring, "Bool") == 0) {
        term->kind = Bool;
      } else if (strcmp(child->valuestring, "If") == 0) {
        term->kind = If;
      } else if (strcmp(child->valuestring, "Tuple") == 0) {
        term->kind = Tuple;
      } else if (strcmp(child->valuestring, "Let") == 0) {
        term->kind = Let;
      } else if (strcmp(child->valuestring, "Var") == 0) {
        term->kind = Var;
      } else if (strcmp(child->valuestring, "First") == 0) {
        term->kind = First;
      } else if (strcmp(child->valuestring, "Second") == 0) {
        term->kind = Second;
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
        } else if (strcmp(value, "Rem") == 0) {
          term->data.binaryTerm.op = Rem;
        } else if (strcmp(value, "Eq") == 0) {
          term->data.binaryTerm.op = Eq;
        } else if (strcmp(value, "Neq") == 0) {
          term->data.binaryTerm.op = Neq;
        } else if (strcmp(value, "Lt") == 0) {
          term->data.binaryTerm.op = Lt;
        } else if (strcmp(value, "Gt") == 0) {
          term->data.binaryTerm.op = Gt;
        } else if (strcmp(value, "Lte") == 0) {
          term->data.binaryTerm.op = Lte;
        } else if (strcmp(value, "Gte") == 0) {
          term->data.binaryTerm.op = Gte;
        } else if (strcmp(value, "And") == 0) {
          term->data.binaryTerm.op = And;
        } else if (strcmp(value, "Or") == 0) {
          term->data.binaryTerm.op = Or;
        }
      }

    } break;
    case Int:
      if (strcmp(key, "value") == 0) {
        term->data.intTerm.value = child->valueint;
      }
      break;
    case Bool:
      if (strcmp(key, "value") == 0) {
        term->data.boolTerm.value = cJSON_IsTrue(child) ? true : false;
      }
      break;
    case Let: {
      Term *auxTerm = malloc(sizeof(Term));
      Parameter *parameter = malloc(sizeof(Parameter));
      if (strcmp(key, "name") == 0) {
        parse_parameter(parameter, child);
        term->data.letTerm.name = parameter;
      } else if (strcmp(key, "value") == 0) {
        parse_expression(auxTerm, child);
        term->data.letTerm.value = auxTerm;
      } else if (strcmp(key, "next") == 0) {
        parse_expression(auxTerm, child);
        term->data.letTerm.next = auxTerm;
      }
    } break;
    case Var: {
      Term *auxTerm = malloc(sizeof(Term));
      if (strcmp(key, "text") == 0) {
        strcpy(term->data.varTerm.text, child->valuestring);
      }
    } break;
    case If: {
      Term *auxTerm = malloc(sizeof(Term));
      if (strcmp(key, "condition") == 0) {
        parse_expression(auxTerm, child);
        term->data.ifTerm.condition = auxTerm;
      } else if (strcmp(key, "then") == 0) {
        parse_expression(auxTerm, child);
        term->data.ifTerm.then = auxTerm;
      } else if (strcmp(key, "otherwise") == 0) {
        parse_expression(auxTerm, child);
        term->data.ifTerm.otherwise = auxTerm;
      }
    } break;
    case First:
      if (strcmp(key, "value") == 0) {
        Term *auxTerm = malloc(sizeof(Term));
        parse_expression(auxTerm, child);
        term->data.firstTerm.value = auxTerm;
      }
      break;
    case Second:
      if (strcmp(key, "value") == 0) {
        Term *auxTerm = malloc(sizeof(Term));
        parse_expression(auxTerm, child);
        term->data.secondTerm.value = auxTerm;
      }
      break;
    case Tuple: {
      Term *auxTerm = malloc(sizeof(Term));
      if (strcmp(key, "first") == 0) {
        parse_expression(auxTerm, child);
        term->data.tupleTerm.first = auxTerm;
      } else if (strcmp(key, "second") == 0) {
        parse_expression(auxTerm, child);
        term->data.tupleTerm.second = auxTerm;
      }
    } break;
    case Call:
      break;
    case Function:
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
