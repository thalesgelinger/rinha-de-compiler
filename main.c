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

    result.type = int_type;
    int lhs = eval(term->data.binaryTerm.lhs).value.intValue;
    int rhs = eval(term->data.binaryTerm.rhs).value.intValue;
    switch (term->data.binaryTerm.op) {
    case Add:
      result.value.intValue = lhs + rhs;
    case Sub:
    case Mul:
      result.value.intValue = lhs * rhs;
    case Div:
    case Rem:
    case Eq:
    case Neq:
    case Lt:
    case Gt:
    case Lte:
    case Gte:
    case And:
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
  File *file = parse_file("examples/main.json");

  eval(&file->expression);

  free(file);

  return 0;
}
