#ifndef ACERVO_H
#define ACERVO_H

#include <stdbool.h>

#include "vector2.h"
#include "tabuleiro.h"

typedef struct acervo_t Acervo;

Acervo *new_acervo(int size);
Acervo *new_size(Acervo *acervo);
Vector2 pick_best_city(Acervo *acervo);
int feather_city(Vector2 a, Vector2 b, Tabuleiro *tabuleiro);
int get_insert_pos(Acervo *acervo);
int acervo_get_lower(Acervo *acervo, int v, int n);
unsigned int acervo_get_upper(int v);
bool reach_final_city(Vector2 vec1, Vector2 vec2);
void insert_city(Acervo *f, Vector2 vec, Tabuleiro *tabuleiro);
void bubble_acervo(Acervo *acervo, int p, Tabuleiro *tabuleiro);
void fix_up_acervo(Acervo *acervo, int p, Tabuleiro *tabuleiro);
void switch_cities_acervo(Acervo *acervo, int p);
//NOTE: talvez possam ser precisas
// void sort_acervo();
// void remove_city();
// void concatenate_acervo();
// void free_acervo();

#endif
