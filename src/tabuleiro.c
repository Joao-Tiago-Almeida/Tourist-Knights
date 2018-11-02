#include "tabuleiro.h"

#include <stdlib.h>

#include "util.h"
#include "passeios.h"
#include "movimentos.h"

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

//Devolve um caracter com o tipo de passeio
char tabuleiro_get_tipo_passeio(Tabuleiro* tabuleiro) {
    return tabuleiro->type_passeio;
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

//Função privada
//Faz as operações e escreve no ficheiro fp
void tabuleiro_execute_tipo_A(Tabuleiro *tabuleiro, FILE* fp) {
    //NOTA não deve ser preciso fazer verificações
    if(!(inside_board(((PasseioTipoA*)tabuleiro->passeio)->pos_ini, tabuleiro->height, tabuleiro->width))){
        fprintf(stderr, "o ponto tá fora do tabuleiro\n");
        return;
    }

    printf("Sou bué fixe e tenho o tabuleiro do tipo A lido :D\n");
}

//Função privada
//Faz as operações e escreve no ficheiro fp
void tabuleiro_execute_tipo_B(Tabuleiro *tabuleiro, FILE* fp) {
    printf("Sou bué fixe e tenho o tabuleiro do tipo B lido :D\n");
}

//Faz as operações e escreve no ficheiro fp
void tabuleiro_execute(Tabuleiro *tabuleiro, FILE* fp) {
    if(tabuleiro->type_passeio == 'A') {   
        tabuleiro_execute_tipo_A(tabuleiro, fp);
    } else if(tabuleiro->type_passeio == 'B') {
        tabuleiro_execute_tipo_B(tabuleiro, fp);
    } else if(tabuleiro->type_passeio == 'C') {
        fprintf(stderr, "we are not ready for C files\n");
        return;
    } else {
        printf("Erro modo invalido?\n");
        exit(1);
    }
}

void tabuleiro_free(Tabuleiro* tabuleiro) {
    if(tabuleiro->type_passeio == 'B')   free(((PasseioTipoB*)tabuleiro->passeio)->pontos);

    free(tabuleiro->passeio);
    free(tabuleiro->cost_matrix);
}

void print_tabuleiro(Tabuleiro* tabuleiro, int w, int h) {
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            printf(" %d", tabuleiro->cost_matrix[j + w*i]);
        }
        printf("\n");
    }
}
