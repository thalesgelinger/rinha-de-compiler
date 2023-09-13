
#ifndef DATA_H
#define DATA_H

typedef enum TermKind {
  Print,
  Str,
} TermKind;

typedef struct Location {
  char filename[20];
  int start;
  int end;
} Location;

typedef struct PrintTerm {
  TermKind kind;
  struct Term *value;
} PrintTerm;

typedef struct StrTerm {
  TermKind kind;
  char value[50];
} StrTerm;

typedef struct Term {
  TermKind kind;
  union {
    PrintTerm printTerm;
    StrTerm strTerm;
  } data;
  Location location;
} Term;

typedef struct File {
  char name[20];
  Term expression;
  Location location;
} File;

#endif
