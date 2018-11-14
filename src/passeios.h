#ifndef PASSEIO_H
#define PASSEIO_H

#include "vector2.h"
#include "tabuleiro.h"
typedef struct passeiotipob_t Passeio;

Passeio* passeio_new_read_from_file(Tabuleiro* tabuleiro, int num_pontos, FILE* fp);

void passeio_set_valid(Passeio* passeio, char valid);
char passeio_get_valid(Passeio* passeio);

void passeio_B_set_cost(Passeio* passeio, int cost);
int passeio_B_get_cost(Passeio* passeio);

int passeio_get_num_pontos(Passeio* passeio);

Vector2* passeio_B_get_pontos(Passeio* passeio);
Vector2 passeio_get_pos_ini(Passeio* passeio);

#endif
