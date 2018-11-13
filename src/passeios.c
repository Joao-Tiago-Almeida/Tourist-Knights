#include "passeios.h"

#include "util.h"
#include "movimentos.h"
#include "vector2.h"

struct passeiotipoa_t {
    Vector2 pos_ini;
    //Vector2 pos_fim;
    int num_pontos;
    int cost;   //Só para a primeira parte
    char valid;
};

struct passeiotipob_t {
    int num_pontos;
    Vector2* pontos;
    int cost;  //parte 1, para guardar o custo do camimho se válido
    char valid;
};

/**
 * Escreve a posição inicial do caminho na estrutura do novo passeio
 * @param  pos_ini  posição inicial do caminho
 * @return         novo passeio
 */
PasseioTipoA* passeio_A_new_read_from_file(int num_pontos, Vector2 pos_ini) {
    PasseioTipoA* passeio = (PasseioTipoA*)checked_malloc(sizeof(PasseioTipoA));

    passeio->pos_ini = pos_ini;
    passeio->num_pontos = num_pontos;
    //passeio->pos_fim = pos_fim;
    passeio->valid = 1;
    passeio->cost = 0;

    return passeio;
}
/**
 * Escreve o número de cidades e lê as coordenadas de cidade a visitar do novo passeio do ficehiro, e escreve na estrutura do novo passeio
 * @param  num_pontos   número de cidades a visitar
 * @param  fp         ficheiro de leitura
 * @param  tabuleiro
 * @return            novo passeio
 */
PasseioTipoB* passeio_B_new_read_from_file(int num_pontos, FILE* fp) {
    PasseioTipoB* passeio = (PasseioTipoB*) checked_malloc(sizeof(PasseioTipoB));

    passeio->num_pontos = num_pontos;
    passeio->pontos = (Vector2*) checked_malloc(sizeof(Vector2) * num_pontos);

    passeio->valid = 1;
    passeio->cost = 0;

    //escreve no vetor
    for(int i = 0; i < num_pontos; i++)
    {
        (*passeio).pontos[i] = vector2_read_from_file(fp);
    }
    return passeio;
}

void passeio_A_set_valid(PasseioTipoA* passeio, char valid) {
    passeio->valid = valid;
}

char passeio_A_get_valid(PasseioTipoA* passeio) {
    return passeio->valid;
}

void passeio_B_set_valid(PasseioTipoB* passeio, char valid) {
    passeio->valid = valid;
}

char passeio_B_get_valid(PasseioTipoB* passeio) {
    return passeio->valid;
}

void passeio_A_set_cost(PasseioTipoA* passeio, int cost) {
    passeio->cost = cost;
}

int passeio_A_get_cost(PasseioTipoA* passeio) {
    return passeio->cost;
}

void passeio_B_set_cost(PasseioTipoB* passeio, int cost) {
    passeio->cost = cost;
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

Vector2 passeio_A_get_pos_ini(PasseioTipoA* passeio) {
    return passeio->pos_ini;
}

Vector2* passeio_B_get_pontos(PasseioTipoB* passeio) {
    return passeio->pontos;
}
