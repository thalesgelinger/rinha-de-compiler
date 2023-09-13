#include "../models/data.h"
#include "cJSON.h"

#ifndef PRINTER_H
#define PRINTER_H

void print_location(Location *location);
void print_term(Term *term);
void print_file(File *file);
void print_json(cJSON *json);

#endif
