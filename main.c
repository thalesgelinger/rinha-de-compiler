#include "models/data.h"
#include "utils/cJSON.h"
#include "utils/parser.h"
#include "utils/printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int i = 0;

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
    }
  } break;
  case Str:
    result.type = string_type;
    strcpy(result.value.strValue, term->data.strTerm.value);
    break;
  case Binary: {

    Val lhs = eval(term->data.binaryTerm.lhs);

    Val rhs = eval(term->data.binaryTerm.rhs);

    result.type = int_type;
    switch (term->data.binaryTerm.op) {
    case Add:
      result.value.intValue = lhs.value.intValue + rhs.value.intValue;
      break;
    case Sub:
      result.value.intValue = lhs.value.intValue - rhs.value.intValue;
      break;
    case Mul:
      result.value.intValue = lhs.value.intValue * rhs.value.intValue;
      break;
    case Div:
      result.value.intValue = lhs.value.intValue / rhs.value.intValue;
      break;
    case Rem:
      break;
    case Eq:
      break;
    case Neq:
      break;
    case Lt:
      break;
    case Gt:
      break;
    case Lte:
      break;
    case Gte:
      break;
    case And:
      break;
    case Or:
      break;
    }
  } break;
  case Int:
    result.type = int_type;
    result.value.intValue = term->data.intTerm.value;
    break;
  }

  return result;
}

int main() {

  File *file = malloc(sizeof(File));

  parse_file(file, "examples/prints.json");

  eval(&file->expression);

  free(file);

  return 0;
}
