#ifndef VECTOR_2_H
#define VECTOR_2_H

typedef struct vector2_t {
    int x, y;
} Vector2;

Vector2 vector2_new(int x, int y);

#endif