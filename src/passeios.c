#include "passeios.h"

PasseioTipoA passeio_A_new(Vector2 pos_ini) {
    PasseioTipoA passeio;

    passeio.pos_ini = pos_ini;

    return passeio;
}

PasseioTipoB passeio_B_new(int num_pontos) {
    PasseioTipoB passeio;

    passeio.num_pontos = num_pontos;
    passeio.pontos = (Vector2*) checked_malloc(sizeof(Vector2) * num_pontos);

    return passeio;
}
