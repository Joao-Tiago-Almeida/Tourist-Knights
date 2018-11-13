#ifndef PASSEIO_H
#define PASSEIO_H

#include "vector2.h"
// #include "vecdin.h"
#include "tabuleiro.h"

typedef struct passeiotipoa_t PasseioTipoA;
typedef struct passeiotipob_t PasseioTipoB;

PasseioTipoA* passeio_A_new_read_from_file(int num_pontos, Vector2 pos_ini);
PasseioTipoB* passeio_B_new_read_from_file(int num_pontos, FILE* fp);

void passeio_A_set_valid(PasseioTipoA* passeio, char valid);
char passeio_A_get_valid(PasseioTipoA* passeio);

void passeio_B_set_valid(PasseioTipoB* passeio, char valid);
char passeio_B_get_valid(PasseioTipoB* passeio);

void passeio_A_set_cost(PasseioTipoA* passeio, char cost);
char passeio_A_get_cost(PasseioTipoA* passeio);

void passeio_B_set_cost(PasseioTipoB* passeio, char cost);
char passeio_B_get_cost(PasseioTipoB* passeio);

char passeio_A_get_num_pontos(PasseioTipoA* passeio);
char passeio_B_get_num_pontos(PasseioTipoB* passeio);

Vector2 passeio_A_get_pos_ini(PasseioTipoA* passeio);
Vector2* passeio_B_get_pontos(PasseioTipoB* passeio);

#endif
