#include "utils/cJSON.h" // Include the cJSON library header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_json(cJSON *json) {
  if (cJSON_IsObject(json)) {
    cJSON *child = NULL;
    cJSON_ArrayForEach(child, json) {
      printf("YABADABA\n");
      printf("Key: %s, Value: ", child->string);

      // Recursively print the child element
      print_json(child);

      printf("\n");
    }
  } else if (cJSON_IsArray(json)) {
    cJSON *child = NULL;
    int i = 0;
    cJSON_ArrayForEach(child, json) {
      printf("Index: %d, Value: ", i);

      // Recursively print the child element
      print_json(child);

      printf("\n");
      i++;
    }
  } else if (cJSON_IsString(json)) {
    printf("\nSTRING\n");
    printf("%s", json->valuestring);
  } else if (cJSON_IsNumber(json)) {
    printf("\nDOBULE\n");
    printf("%f", json->valuedouble);
  } else if (cJSON_IsBool(json)) {
    printf("\nBOOLEAN\n");
    printf("%s", cJSON_IsTrue(json) ? "true" : "false");
  } else if (cJSON_IsNull(json)) {
    printf("\nNULL\n");
    printf("null");
  }
}

cJSON *parse_json(char *filepath) {

  FILE *file = fopen(filepath, "r"); // Open the JSON file in read mode

  if (file == NULL) {
    perror("Error opening file");
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file); // Get the size of the file
  fseek(file, 0, SEEK_SET);

  char *json_data =
      (char *)malloc(file_size + 1); // Allocate memory to store the JSON data
  if (json_data == NULL) {
    perror("Memory allocation error");
    fclose(file);
  }

  fread(json_data, 1, file_size, file); // Read the JSON data from the file
  fclose(file);
  json_data[file_size] = '\0'; // Null-terminate the data

  // Parse the JSON data using cJSON
  cJSON *root = cJSON_Parse(json_data);

  if (root == NULL) {
    const char *error_ptr = cJSON_GetErrorPtr();
    if (error_ptr != NULL) {
      fprintf(stderr, "Error before: %s\n", error_ptr);
    }
    cJSON_Delete(root); // Clean up and free allocated memory
    free(json_data);
  }

  free(json_data);

  return root;
}

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
  char *value;
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

    printf("\n");
  }
}

File *parse_file(char *filepath) {

  File *file = malloc(sizeof(File));
  cJSON *json = parse_json(filepath);

  if (cJSON_IsObject(json)) {
    cJSON *child = NULL;
    cJSON_ArrayForEach(child, json) {
      char *key = child->string;
      if (strcmp(key, "name") == 0) {
        strcpy(file->name, child->valuestring);
      } else if (strcmp(key, "expression") == 0) {
        printf("EXPRESSION\n");
      } else if (strcmp(key, "location") == 0) {
        parse_location(&file->location, child);
      }
    }
  }

  cJSON_Delete(json);
  return file;
}

void print_file(File *file) {
  printf("File name: %s\n", file->name);
  printf("File location.filename: %s\n", file->location.filename);
  printf("File location.start: %d\n", file->location.start);
  printf("File location.end: %d\n", file->location.end);
}

int main() {

  File *file = parse_file("examples/main.json");
  print_file(file);

  free(file);

  return 0;
}
