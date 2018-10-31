#include "passeios.h"

#include "util.h"

PasseioTipoA* passeio_A_new(Vector2 pos_ini) {
    PasseioTipoA* passeio = checked_malloc(sizeof(PasseioTipoA));

    passeio->pos_ini = pos_ini;

    return passeio;
}

PasseioTipoB* passeio_B_new(int num_pontos, FILE* fp, unsigned int w, unsigned int h) {
    PasseioTipoB* passeio = checked_malloc(sizeof(PasseioTipoB));

    passeio->num_pontos = num_pontos;
    passeio->pontos = (Vector2*) checked_malloc(sizeof(Vector2) * num_pontos);

    //escreve no vetor
    for(int i = 0; i < num_pontos; i++)
    {
        (*passeio).pontos[i] = vector2_read_from_file(fp);
    }
    return passeio;
}
