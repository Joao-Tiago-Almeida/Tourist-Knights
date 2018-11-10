#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdio.h>
#include <stdbool.h>

typedef struct tabuleiro_t Tabuleiro;

//TODO Mudar para o .c --só mudas ao pé de mim, para eu ver as alterações que se tem de fazer
struct tabuleiro_t {
    unsigned int width, height;
    char* cost_matrix; //Cada custo tem um byte
    char type_passeio;
    void* passeio;
};


Tabuleiro tabuleiro_new(unsigned int w, unsigned int h, char type_passeio);
void tabuleiro_read_matrix_from_file(Tabuleiro* tabuleiro, FILE* fp);

//FIXME mudar para recber um Vector2 em vez de dois pontos
void tabuleiro_set_cost(Tabuleiro* tabuleiro, unsigned int x, unsigned int y, unsigned char cost);
int tabuleiro_get_cost(Tabuleiro* tabuleiro, unsigned int x, unsigned int y);

void tabuleiro_set_passeio(Tabuleiro* tabuleiro, void* passeio);
void* tabuleiro_get_passeio(Tabuleiro* tabuleiro);

char tabuleiro_get_tipo_passeio(Tabuleiro* tabuleiro);


void tabuleiro_execute(Tabuleiro *tabuleiro, FILE* fp);

void tabuleiro_free(Tabuleiro* tabuleiro);

void print_tabuleiro(Tabuleiro* tabuleiro, int w, int h);

#endif
