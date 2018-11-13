#include "passeios.h"

#include "util.h"
#include "movimentos.h"

#define NUM_PONTOS_A 1

struct passeiotipoa_t {
    Vector2 pos_ini;
    //Vector2 pos_fim;
    int num_pontos;
    int cost;   //Só para a primeira parte
    int valid;
};

struct passeiotipob_t {
    int num_pontos;
    Vector2* pontos;
    int cost;  //parte 1, para guardar o custo do camimho se válido
    int valid;
};

/**
 * [passeio_A_new description]
 * @param  pos_ini [description]
 * @return         [description]
 */
PasseioTipoA* passeio_A_new_read_from_file(Vector2 pos_ini) {
    PasseioTipoA* passeio = (PasseioTipoA*)checked_malloc(sizeof(PasseioTipoA));

    passeio->pos_ini = pos_ini;
    passeio->num_pontos = NUM_PONTOS_A;
    //passeio->pos_fim = pos_fim;

    return passeio;
}
/**
 * [passeio_B_new_read_from_file description]
 * @param  num_pontos [description]
 * @param  fp         [description]
 * @param  tabuleiro  [description]
 * @return            [description]
 */
PasseioTipoB* passeio_B_new_read_from_file(int num_pontos, FILE* fp, Tabuleiro tabuleiro) {
    PasseioTipoB* passeio = (PasseioTipoB*) checked_malloc(sizeof(PasseioTipoB));

    passeio->num_pontos = num_pontos;
    passeio->pontos = (Vector2*) checked_malloc(sizeof(Vector2) * num_pontos);

    //escreve no vetor
    for(int i = 0; i < num_pontos; i++)
    {
        (*passeio).pontos[i] = vector2_read_from_file(fp);
        // //FIXME
        // inside_board(tabuleiro, passeio->pontos[i]);
    }
    return passeio;
}

int passeio_A_get_valid(PasseioTipoA* passeio) {
    return passeio->valid;
}

int passeio_B_get_valid(PasseioTipoB* passeio) {
    return passeio->valid;
}

int passeio_A_get_cost(PasseioTipoA* passeio) {
    return passeio->cost;
}

int passeio_B_get_cost(PasseioTipoB* passeio) {
    return passeio->cost;
}

int passeio_A_get_num_pontos(PasseioTipoA* passeio) {
    return passeio->num_pontos;
}
int passeio_B_get_num_pontos(PasseioTipoB* passeio) {
    return passeio->num_pontos;
}
