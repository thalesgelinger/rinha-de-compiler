#include "models/data.h"
#include "utils/cJSON.h"
#include "utils/parser.h"
#include "utils/printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void eval(Term *term) {
  switch (term->kind) {
  case Print:
    eval(term->data.printTerm.value);
    break;
  case Str:
    printf("%s", term->data.strTerm.value);
    break;
  }
}

int main() {
  File *file = parse_file("examples/main.json");

  eval(&file->expression);

  free(file);

  return 0;
}
