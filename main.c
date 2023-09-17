#include "models/data.h"
#include "utils/cJSON.h"
#include "utils/hash.h"
#include "utils/parser.h"
#include "utils/printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void raise(char *message) {
  fprintf(stderr, "%s", message);
  exit(EXIT_FAILURE);
}

Val eval(Term *term, Hash *variables) {
  Val result;

  switch (term->kind) {
  case Print: {
    Val printValue = eval(term->data.printTerm.value, variables);
    result = printValue;
    switch (printValue.type) {
    case int_type:
      printf("%d\n", printValue.value.intValue);
      break;
    case string_type:
      printf("%s\n", printValue.value.strValue);
      break;
    case bool_type:
      printf("%s\n", printValue.value.boolValue ? "true" : "false");
      break;
    case tuple_type: {
      char *firstTerm = malloc(sizeof(char));
      char *secondTerm = malloc(sizeof(char));
      Val firstVal = eval(printValue.value.tupleValue.first, variables);
      Val secondVal = eval(printValue.value.tupleValue.second, variables);

      switch (firstVal.type) {
      case int_type:
        sprintf(firstTerm, "%d", firstVal.value.intValue);
        break;
      case string_type:
        strcpy(firstTerm, firstVal.value.strValue);
        break;
      case bool_type:
        strcpy(firstTerm, firstVal.value.boolValue ? "true" : "false");
        break;
      case tuple_type:
        // TODO
        break;
      case clojure_type:
        // TODO
        break;
      }

      switch (secondVal.type) {
      case int_type:
        sprintf(secondTerm, "%d", secondVal.value.intValue);
        break;
      case string_type:
        strcpy(secondTerm, secondVal.value.strValue);
        break;
      case bool_type:
        strcpy(secondTerm, secondVal.value.boolValue ? "true" : "false");
        break;
      case tuple_type:
        // TODO
        break;
      case clojure_type:
        // TODO
        break;
      }

      printf("(%s, %s)\n", firstTerm, secondTerm);
    } break;
    case clojure_type:
      // TODO
      printf("<clojure#>");
      break;
    }
  } break;
  case Str:
    result.type = string_type;
    strcpy(result.value.strValue, term->data.strTerm.value);
    break;
  case Binary: {

    Val lhs = eval(term->data.binaryTerm.lhs, variables);

    Val rhs = eval(term->data.binaryTerm.rhs, variables);

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
      printf("\n");
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
        case tuple_type:
          result.value.boolValue = false;
          break;
        case clojure_type:
          // FIXME
          result.value.boolValue = false;
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
        case tuple_type:
          result.value.boolValue = true;
          break;
        case clojure_type:
          // FIXME
          result.value.boolValue = true;
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
  case Call: {
    Val fnVal = eval(term->data.callTerm.callee, variables);
    switch (fnVal.type) {
    case clojure_type: {
      FunctionTerm fn = fnVal.value.clojureValue.fn;
      Hash *fn_scope = malloc(sizeof(Hash));
      Parameters *paramsIterator = fn.parameters;
      Arguments *argsIterator = term->data.callTerm.arguments;
      while (paramsIterator) {
        Val val = eval(argsIterator->value, variables);
        char *key = paramsIterator->value->text;
        insert_node(fn_scope, key, &val);
        paramsIterator = paramsIterator->previous;
        argsIterator = argsIterator->previous;
      }

      result = eval(fn.value, fn_scope);
    } break;
    default:
      printf("Invalid, this is not a function");
      break;
    }
  } break;
  case Function:
    result.type = clojure_type;
    result.value.clojureValue.fn = term->data.functionTerm;
    result.value.clojureValue.env = variables;
    break;
  case Let: {
    Val val = eval(term->data.letTerm.value, variables);
    insert_node(variables, term->data.letTerm.name->text, &val);
    eval(term->data.letTerm.next, variables);
  } break;
  case Var: {
    Val *var = search(variables, term->data.varTerm.text);
    if (var == NULL) {
      // TODO: Throw Error
    } else {
      result.type = var->type;
      result.value = var->value;
    }
  } break;
  case If: {
    Val condition = eval(term->data.ifTerm.condition, variables);
    if (condition.value.boolValue) {
      result = eval(term->data.ifTerm.then, variables);
    } else {
      result = eval(term->data.ifTerm.otherwise, variables);
    }
  } break;
  case First: {
    Val val = eval(term->data.firstTerm.value, variables);
    if (val.type == tuple_type) {
      Term *first = val.value.tupleValue.first;
      result = eval(first, variables);
    } else {
      // TODO: throw Error
    }
  } break;
  case Second: {
    Val val = eval(term->data.secondTerm.value, variables);
    if (val.type == tuple_type) {
      result = eval(val.value.tupleValue.second, variables);
    } else {
      // TODO: throw Error
    }
  } break;
  case Tuple:
    result.type = tuple_type;
    result.value.tupleValue.first = term->data.tupleTerm.first;
    result.value.tupleValue.second = term->data.tupleTerm.second;
    break;
  }

  return result;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Missing rinha ast path as <argument>\n");
    return 1;
  }

  char *path = argv[1];

  File *file = malloc(sizeof(File));
  Hash *variables = malloc(sizeof(Hash));

  parse_file(file, path);

  eval(&file->expression, variables);

  free(file);

  return 1;
}
