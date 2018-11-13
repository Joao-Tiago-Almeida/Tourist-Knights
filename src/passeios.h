#ifndef PASSEIO_H
#define PASSEIO_H

#include "vector2.h"
// #include "vecdin.h"
#include "tabuleiro.h"

//  TODO mudar para o.c
typedef struct passeiotipoa_t PasseioTipoA;
typedef struct passeiotipob_t PasseioTipoB;

PasseioTipoA* passeio_A_new_read_from_file(Vector2 pos_ini);
PasseioTipoB* passeio_B_new_read_from_file(int num_pontos, FILE* fp, Tabuleiro tabuleiro);

int passeio_A_get_valid(PasseioTipoA* passeio);
int passeio_B_get_valid(PasseioTipoB* passeio);
int passeio_A_get_cost(PasseioTipoB* passeio)
int passeio_B_get_cost(PasseioTipoB* passeio);
int passeio_A_get_num_pontos(PasseioTipoA* passeio);
int passeio_B_get_num_pontos(PasseioTipoB* passeio);

#endif
