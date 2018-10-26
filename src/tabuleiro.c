#include <stdlib.h>
#include "tabuleiro.h"

#include "util.h"

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

void tabuleiro_set_passeio(Tabuleiro* tabuleiro, void* passeio) {
    tabuleiro->passeio = passeio;
}

void* tabuleiro_get_passeio(Tabuleiro* tabuleiro) {
    return tabuleiro->passeio;
}

void tabuleiro_read_matrix_from_file(Tabuleiro* tabuleiro, FILE* fp) {
    for(unsigned int j = 0; j<tabuleiro->height; j++) {
        for(unsigned int i = 0; i<tabuleiro->width; i++) {
            int cost;
            fscanf(fp, "%d", &cost);

            if(cost > 255 || cost < 0) {
                printf("Custo não cabe num byte %d\n", cost);//TODO retirar na versão a entregar para melhor performance
                exit(1);
            }

            tabuleiro_set_cost(tabuleiro, i, j, (char) cost);
        }
    }
}

void tabuleiro_execute(Tabuleiro *tabuleiro) {
    printf("Sou bué fixe e tenho o tabuleiro do tipo %c lido :D\n", tabuleiro->type_passeio);
}

void tabuleiro_free(Tabuleiro* tabuleiro) {
    free(tabuleiro->passeio);
    free(tabuleiro->cost_matrix);
    //free(tabuleiro);
}