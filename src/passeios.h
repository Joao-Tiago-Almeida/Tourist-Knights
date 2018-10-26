#ifndef PASSEIO_H
#define PASSEIO_H

#include "vector2.h"
#include "vecdin.h"

typedef struct passeiotipoa_t {
    Vector2 pos_ini;
} PasseioTipoA;

PasseioTipoA* passeio_A_new(Vector2 pos_ini);

typedef struct passeiotipob_t {
    int num_pontos;
    Vector2* pontos;
} PasseioTipoB;

PasseioTipoB* passeio_B_new(int num_pontos);

#endif