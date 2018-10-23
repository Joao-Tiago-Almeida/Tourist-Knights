#ifndef TABULEIRO_H
#define TABULEIRO_H

typedef struct tabuleiro_t {
    unsigned int width, height;
    unsigned char* cost_matrix; //Cada custo tem um byte
    char type_passeio;
    void* passeio;
} Tabuleiro;

Tabuleiro tabuleiro_new(unsigned int w, unsigned int h, char type_passeio);
void tabuleiro_set_cost(Tabuleiro* tabuleiro, unsigned int x, unsigned int y, unsigned char cost);
int tabuleiro_get_cost(Tabuleiro* tabuleiro, unsigned int x, unsigned int y);

void tabuleiro_free(Tabuleiro* tabuleiro);

#endif