#ifndef ACERVO_H
#define ACERVO_H

#include <stdbool.h>

#include "vector2.h"
#include "tabuleiro.h"

typedef struct acervo_t Acervo;

Acervo *new_acervo(int size_acervo, int size_idx_matrix);
Vector2 acervo_get_top(Acervo *acervo);
void acervo_insert(Acervo *f, Vector2 vec, Tabuleiro *tabuleiro);
void acervo_update(Acervo* acervo, Vector2 vec, int old_value, Tabuleiro* tabuleiro);
bool acervo_is_empty(Acervo* acervo);
void acervo_remove_top(Acervo *acervo, Tabuleiro* tabuleiro);
void acervo_print(Acervo *acervo);
void acervo_free(Acervo** acervo);
void acervo_init(Acervo *acervo, Tabuleiro* tabuleiro);

//nao sei se é para tar aqui
int acervo_feather_city(Acervo* acervo, int a, int b, Tabuleiro *tabuleiro);

/*int acervo_get_insert_pos(Acervo *acervo);
int acervo_get_lower(Acervo *acervo, int v, int n);
unsigned int acervo_get_upper(int v);
void acervo_fix_up(Acervo *acervo, int p, Tabuleiro *tabuleiro);
void acervo_fix_down(Acervo *acervo, int p, Tabuleiro *tabuleiro);
void exchange_cities_acervo(Acervo *acervo, int p);*/
//NOTE: talvez possam ser precisas
// void sort_acervo();
// void remove_city();
// void concatenate_acervo();
// void free_acervo();

#endif
