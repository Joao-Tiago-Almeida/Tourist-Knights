#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdbool.h>

#define DEBUG

void *checked_malloc(size_t size);
void *checked_calloc(size_t size, size_t n);
void *checked_realloc(void * ptr, size_t size);
bool string_ends_with(char* str, char* test);
char* create_dot_valid_filename(char* filename);

#endif
