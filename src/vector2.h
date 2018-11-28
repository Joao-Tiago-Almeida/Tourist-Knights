#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <stdio.h>
#include <stdbool.h>

/*
 * Estrutuar auxiliar que guarda dois inteiros como forma de uma coordenada num espaço euclidiano de dimensão
 */
typedef struct vector2_t {
    int x, y;
} Vector2;

Vector2 vector2_new(int x, int y);
Vector2 vector2_read_from_file(FILE* fp);

bool vector2_equals(Vector2 a, Vector2 b);
Vector2 vector2_add(Vector2 a, Vector2 b);
Vector2 vector2_sub(Vector2 a, Vector2 b);

#endif
