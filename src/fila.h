#ifndef FILA_H
#define FILA_H

#include <stdbool.h>

#include "vector2.h"

typedef struct fila_t Fila;

Fila *new_fila(int size);
Fila *new_size(Fila *f, int *size);
void insert_node();
void pick_best_node();
void remove_node();
bool feather_node(int, int);
void sort_fila();
void concatenate_fila();
// void free_fila();

#endif
