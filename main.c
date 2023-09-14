#include "models/data.h"
#include "utils/cJSON.h"
#include "utils/parser.h"
#include "utils/printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Val eval(Term *term) {
  Val result;

  switch (term->kind) {
  case Print: {
    Val printValue = eval(term->data.printTerm.value);
    switch (printValue.type) {
    case int_type:
      printf("%d", printValue.value.intValue);
      break;
    case string_type:
      printf("%s", printValue.value.strValue);
      break;
    case bool_type:
      printf("%s", printValue.value.boolValue ? "true" : "false");
      break;
    }
  } break;
  case Str:
    result.type = string_type;
    strcpy(result.value.strValue, term->data.strTerm.value);
    break;
  case Binary: {

    Val lhs = eval(term->data.binaryTerm.lhs);

    Val rhs = eval(term->data.binaryTerm.rhs);

    switch (term->data.binaryTerm.op) {
    case Add:
      if (lhs.type == string_type || rhs.type == string_type) {
        result.type = string_type;
        char *lhs_str = lhs.value.strValue;
        char *rhs_str = rhs.value.strValue;
        if (lhs.type == int_type) {
          sprintf(lhs_str, "%d", lhs.value.intValue);
        } else if (rhs.type == int_type) {
          sprintf(rhs_str, "%d", rhs.value.intValue);
        }
        strcpy(result.value.strValue, strcat(lhs_str, rhs_str));
      } else {
        result.type = int_type;
        result.value.intValue = lhs.value.intValue + rhs.value.intValue;
      }
      break;
    case Sub:
      result.type = int_type;
      result.value.intValue = lhs.value.intValue - rhs.value.intValue;
      break;
    case Mul:
      result.type = int_type;
      result.value.intValue = lhs.value.intValue * rhs.value.intValue;
      break;
    case Div:
      result.type = int_type;
      result.value.intValue = lhs.value.intValue / rhs.value.intValue;
      break;
    case Rem:
      result.type = int_type;
      result.value.intValue = lhs.value.boolValue % rhs.value.intValue;
      break;
    case Eq:
      result.type = bool_type;
      if (lhs.type == rhs.type) {
        switch (lhs.type) {
        case int_type:
          result.value.boolValue = lhs.value.intValue == rhs.value.intValue;
          break;
        case bool_type:
          result.value.boolValue = lhs.value.boolValue == rhs.value.boolValue;
          break;
        case string_type:
          result.value.boolValue =
              strcmp(lhs.value.strValue, rhs.value.strValue) == 0;
          break;
        }
      }
      break;
    case Neq:
      if (lhs.type == rhs.type) {
        switch (lhs.type) {
        case int_type:
          result.value.boolValue = lhs.value.intValue != rhs.value.intValue;
          break;
        case bool_type:
          result.value.boolValue = lhs.value.boolValue != rhs.value.boolValue;
          break;
        case string_type:
          result.value.boolValue =
              strcmp(lhs.value.strValue, rhs.value.strValue) != 0;
          break;
        }
      }
      break;
    case Lt:
      result.type = bool_type;
      result.value.boolValue = lhs.value.intValue < rhs.value.intValue;
      break;
    case Gt:
      result.type = bool_type;
      result.value.boolValue = lhs.value.intValue > rhs.value.intValue;
      break;
    case Lte:
      result.type = bool_type;
      result.value.boolValue = lhs.value.intValue <= rhs.value.intValue;
      break;
    case Gte:
      result.type = bool_type;
      result.value.boolValue = lhs.value.intValue >= rhs.value.intValue;
      break;
    case And:
      result.type = bool_type;
      result.value.boolValue = lhs.value.boolValue && rhs.value.boolValue;
      break;
    case Or:
      result.type = bool_type;
      result.value.boolValue = lhs.value.boolValue || rhs.value.boolValue;
      break;
    }
  } break;
  case Int:
    result.type = int_type;
    result.value.intValue = term->data.intTerm.value;
    break;
  case Bool:
    result.type = bool_type;
    result.value.boolValue = term->data.boolTerm.value;
    break;
  case Call:
    break;
  case Function:
    break;
  case Let:
    break;
  case If: {
    Val condition = eval(term->data.ifTerm.condition);
    if (condition.value.boolValue) {
      result = eval(term->data.ifTerm.then);
    } else {
      result = eval(term->data.ifTerm.otherwise);
    }
  } break;
  case First:
    break;
  case Second:
    break;
  case Tuple:
    break;
  case Var:
    break;
  }

  return result;
}

int main() {

  File *file = malloc(sizeof(File));

  parse_file(file, "examples/if.json");

  eval(&file->expression);

  free(file);

  return 0;
}
