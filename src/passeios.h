#ifndef PASSEIO_H
#define PASSEIO_H

#include "vector2.h"
#include "vecdin.h"
#include "tabuleiro.h"

typedef struct passeiotipoa_t {
    Vector2 pos_ini;
    Vector2 pos_fim;
} PasseioTipoA;

PasseioTipoA* passeio_A_new(Vector2 pos_ini);

typedef struct passeiotipob_t {
    int num_pontos;
    Vector2* pontos;
} PasseioTipoB;

PasseioTipoB* passeio_B_new_read_from_file(int num_pontos, FILE* fp, Tabuleiro tabuleiro);

#endif
