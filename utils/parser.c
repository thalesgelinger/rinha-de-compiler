
#include "../models/data.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void parse_expression(Term *term, cJSON *jsonExpression) {

  cJSON *child = NULL;

  cJSON_ArrayForEach(child, jsonExpression) {
    char *key = child->string;
    if (strcmp(key, "kind") == 0) {
      if (strcmp(child->valuestring, "Print") == 0) {
        term->kind = Print;
      } else if (strcmp(child->valuestring, "Str") == 0) {
        term->kind = Str;
      }
    } else if (strcmp(key, "value") == 0) {
      if (term->kind == Print) {
        Term *printTerm = malloc(sizeof(Term));
        parse_expression(printTerm, child);
        term->data.printTerm.value = printTerm;
      } else if (term->kind == Str) {
        strcpy(term->data.strTerm.value, child->valuestring);
      }

    } else if (strcmp(key, "location") == 0) {
      parse_location(&term->location, child);
    }
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
        parse_expression(&file->expression, child);
      } else if (strcmp(key, "location") == 0) {
        parse_location(&file->location, child);
      }
    }
  }

  cJSON_Delete(json);
  return file;
}
