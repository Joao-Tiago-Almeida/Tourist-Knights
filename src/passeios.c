#include "passeios.h"

#include "util.h"
#include "movimentos.h"
#include "vector2.h"

struct passeiotipob_t {
    int num_pontos;
    Vector2* pontos;
    int cost;  //parte 1, para guardar o custo do camimho se válido
    char valid;
};

/**
 * Escreve o número de cidades e lê as coordenadas de cidade a visitar do novo passeio do ficehiro, e escreve na estrutura do novo passeio
 * @param  num_pontos   número de cidades a visitar
 * @param  fp         ficheiro de leitura
 * @param  tabuleiro
 * @return            novo passeio
 */
Passeio* passeio_new_read_from_file(Tabuleiro* tabuleiro, int num_pontos, FILE* fp) {
    Passeio* passeio = (Passeio*) checked_malloc(sizeof(Passeio));

    passeio->num_pontos = num_pontos;
    passeio->pontos = (Vector2*) checked_malloc(sizeof(Vector2) * num_pontos);

    passeio->valid = 1;
    passeio->cost = 0;

    //escreve no vetor
    for(int i = 0; i < num_pontos; i++)
    {
        Vector2 vec = vector2_read_from_file(fp);
        (*passeio).pontos[i] = vec;

        if(!inside_board(tabuleiro, vec)) {
            //Se algum dos pontos estiver fora do tabuleiro marca o passeio como invalido
            passeio->valid = -1;
        }

    }
    return passeio;
}

void passeio_set_valid(Passeio* passeio, char valid) {
    passeio->valid = valid;
}

char passeio_get_valid(Passeio* passeio) {
    return passeio->valid;
}

void passeio_B_set_cost(Passeio* passeio, int cost) {
    passeio->cost = cost;
}

int passeio_B_get_cost(Passeio* passeio) {
    return passeio->cost;
}

int passeio_get_num_pontos(Passeio* passeio) {
    return passeio->num_pontos;
}

Vector2* passeio_B_get_pontos(Passeio* passeio) {
    return passeio->pontos;
}
Vector2 passeio_get_pos_ini(Passeio* passeio) {
    return passeio->pontos[0];
}