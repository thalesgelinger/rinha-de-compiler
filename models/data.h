
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
  Call,
  Function,
  Let,
  If,
  First,
  Second,
  Tuple,
  Var
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

typedef struct ArgsList {
  struct Term **items;
  int size;
} ArgsList;

typedef struct CallTerm {
  TermKind kind;
  struct Term *callee;
  ArgsList *arguments;
} CallTerm;

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

typedef struct Parameter {
  char text[20];
  Location location;
} Parameter;

typedef struct ParametersList {
  Parameter **items;
  int size;
} ParametersList;

typedef struct FunctionTerm {
  TermKind kind;
  ParametersList *parameters;
  struct Term *value;
} FunctionTerm;

typedef struct LetTerm {
  TermKind kind;
  struct Parameter *name;
  struct Term *value;
  struct Term *next;
} LetTerm;

typedef struct IfTerm {
  TermKind kind;
  struct Term *condition;
  struct Term *then;
  struct Term *otherwise;
} IfTerm;

typedef struct FirstTerm {
  TermKind kind;
  struct Term *value;
} FirstTerm;

typedef struct SecondTerm {
  TermKind kind;
  struct Term *value;
} SecondTerm;

typedef struct VarTerm {
  TermKind kind;
  char text[20];
} VarTerm;

typedef struct TupleTerm {
  TermKind kind;
  struct Term *first;
  struct Term *second;
} TupleTerm;

typedef struct Term {
  TermKind kind;
  union {
    PrintTerm printTerm;
    StrTerm strTerm;
    BinaryTerm binaryTerm;
    IntTerm intTerm;
    BoolTerm boolTerm;

    CallTerm callTerm;
    FunctionTerm functionTerm;
    LetTerm letTerm;
    IfTerm ifTerm;
    FirstTerm firstTerm;
    SecondTerm secondTerm;
    TupleTerm tupleTerm;
    VarTerm varTerm;
  } data;
  Location location;
} Term;

typedef struct File {
  char name[FILENAME_SIZE];
  Term expression;
  Location location;
} File;

typedef union Value {
  int32_t intValue;
  char strValue[50];
  bool boolValue;
  struct {
    Term *first;
    Term *second;
  } tupleValue;
  struct {
    FunctionTerm fn;
    struct Hash *env;
  } clojureValue;
} Value;

typedef enum Type {
  int_type,
  string_type,
  bool_type,
  tuple_type,
  clojure_type,
} Type;

typedef struct {
  Type type;
  Value value;
} Val;

ParametersList *init_params_list();
void add_param(ParametersList *list, Parameter *element);
void free_params(ParametersList *list);
void print_params(ParametersList *list);

ArgsList *init_args_list();
void add_arg(ArgsList *list, Term *element);
void free_args(ArgsList *list);
void print_args(ArgsList *list);

#endif
