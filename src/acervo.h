#ifndef ACERVO_H
#define ACERVO_H

#include <stdbool.h>

#include "vector2.h"
#include "tabuleiro.h"

typedef struct acervo_t Acervo;

Acervo *new_acervo(int size);
Acervo *new_size(Acervo *acervo);
void insert_city(Acervo *f, Vector2 vec);
Vector2 pick_best_city(Acervo *acervo);
int feather_city(Vector2 a, Vector2 b, Tabuleiro *tabuleiro);
unsigned int acervo_get_upper(int v);
int acervo_get_lower(Acervo *acervo, int v, int n);
void bubble_down_acervo(Acervo *acervo, int p, Tabuleiro *tabuleiro);
void bubble_up_acervo(Acervo *acervo, int p, Tabuleiro *tabuleiro);
// void remove_city();
void sort_acervo();
// void concatenate_acervo();
// void free_acervo();
int get_insert_pos(Acervo *acervo);
void switch_cities_acervo(Acervo *acervo, int p);
#endif
