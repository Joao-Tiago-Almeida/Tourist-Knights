#include <stdlib.h>
#include "tabuleiro.h"

Tabuleiro tabuleiro_new(unsigned int w, unsigned int h, char type_passeio) {
    Tabuleiro tab;
    
    tab.width = w;
    tab.height = h;
    tab.type_passeio = type_passeio;
    tab.passeio = NULL;

    tab.cost_matrix = (char*) checked_malloc(sizeof(char) * (w * h));

    return tab;
}

void tabuleiro_set_cost(Tabuleiro* tabuleiro, unsigned int x, unsigned int y, unsigned char cost) {
    tabuleiro->cost_matrix[x + y*tabuleiro->width] = cost;
}
int tabuleiro_get_cost(Tabuleiro* tabuleiro, unsigned int x, unsigned int y) {
    return tabuleiro->cost_matrix[x + y*tabuleiro->width];
}

void tabuleiro_free(Tabuleiro* tabuleiro) {
    free(tabuleiro->passeio);
    free(tabuleiro->cost_matrix);
    free(tabuleiro);
}