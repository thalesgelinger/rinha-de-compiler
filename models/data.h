
#include <stdbool.h>
#include <stdint.h>

#ifndef DATA_H
#define DATA_H

#define FILENAME_SIZE 30

typedef enum TermKind {
  Print,
  Str,
  Binary,
  Int,
  Bool,
} TermKind;

typedef struct Location {
  char filename[FILENAME_SIZE];
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

typedef struct BoolTerm {
  TermKind kind;
  bool value;
} BoolTerm;

typedef enum BinaryOp {
  Add,
  Sub,
  Mul,
  Div,
  Rem,
  Eq,
  Neq,
  Lt,
  Gt,
  Lte,
  Gte,
  And,
  Or
} BinaryOp;

typedef struct BinaryTerm {
  TermKind kind;
  struct Term *lhs;
  BinaryOp op;
  struct Term *rhs;
  Location location;
} BinaryTerm;

typedef struct IntTerm {
  TermKind kind;
  int32_t value;
} IntTerm;

typedef struct Term {
  TermKind kind;
  union {
    PrintTerm printTerm;
    StrTerm strTerm;
    BinaryTerm binaryTerm;
    IntTerm intTerm;
    BoolTerm boolTerm;
  } data;
  Location location;
} Term;

typedef struct File {
  char name[FILENAME_SIZE];
  Term expression;
  Location location;
} File;

typedef union {
  int32_t intValue;
  char strValue[50];
  bool boolValue;
} Value;

typedef enum Type {
  int_type,
  string_type,
  bool_type,
} Type;

typedef struct {
  Type type;
  Value value;
} Val;

#endif
