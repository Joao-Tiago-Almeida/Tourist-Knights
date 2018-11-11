#ifndef PASSEIO_H
#define PASSEIO_H

#include "vector2.h"
// #include "vecdin.h"
#include "tabuleiro.h"

//  TODO mudar para o.c
typedef struct passeiotipoa_t {
    Vector2 pos_ini;
    Vector2 pos_fim;
} PasseioTipoA;

PasseioTipoA* passeio_A_new_read_from_file(Vector2 pos_ini);

typedef struct passeiotipob_t {
    int num_pontos;
    Vector2* pontos;
    int cost;  //parte 1, para guardar o custo do camimho se válido
} PasseioTipoB;

PasseioTipoB* passeio_B_new_read_from_file(int num_pontos, FILE* fp, Tabuleiro tabuleiro);

#endif
