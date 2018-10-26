#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <stdio.h>

typedef struct vector2_t {
    int x, y;
} Vector2;

Vector2 vector2_new(int x, int y);
Vector2 vector2_read_from_file(FILE* fp);

#endif