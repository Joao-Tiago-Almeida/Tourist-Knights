#include "tabuleiro.h"

#include <stdlib.h>

#include "util.h"
#include "passeios.h"
#include "movimentos.h"

struct tabuleiro_t {
    unsigned int width, height;
    char* cost_matrix; //Cada custo tem um byte
    char type_passeio;
    void* passeio;

    int num_pontos;
};

/**
 * Cria uma estrutura que guarda as informações relativas ao tabuleiro a analisar
 * @param  w            largura do tabueleiro
 * @param  h            alrura do tabueleiro
 * @param  type_passeio tipo de passeio{A,B,C}
 * @return              retorna a estrutura criada
 */
Tabuleiro* tabuleiro_new(unsigned int w, unsigned int h, char type_passeio) {
    Tabuleiro* tab = checked_malloc(sizeof(Tabuleiro));
    tab->width = w;
    tab->height = h;
    tab->type_passeio = type_passeio;
    tab->passeio = NULL;

    tab->cost_matrix = NULL; // Nao aloca ainda a matriz porque pode ser que o passeio seja invalido

    return tab;
}

unsigned int tabuleiro_get_width(Tabuleiro* tabuleiro) {
    return tabuleiro->width;
}

unsigned int tabuleiro_get_height(Tabuleiro* tabuleiro) {
    return tabuleiro->height;
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

char tabuleiro_get_tipo_passeio(Tabuleiro* tabuleiro) {
    return tabuleiro->type_passeio;
}

/**
 * Lê o mapa o analisar e passa-o para um vetor
 * @param tabuleiro
 * @param fp        ficheiro de entrada
 */
void tabuleiro_read_matrix_from_file(Tabuleiro* tabuleiro, FILE* fp) {
    tabuleiro->cost_matrix = (char*) checked_malloc(sizeof(char) * (tabuleiro->width * tabuleiro->height));

    for(unsigned int j = 0; j<tabuleiro->height; j++) {
        for(unsigned int i = 0; i<tabuleiro->width; i++) {
            int cost;
            if(fscanf(fp, "%d", &cost) != 1) {
                fprintf(stderr, "Erro de leitura");
                exit(0);
            }

            if(cost > 255 || cost < 0) {
                fprintf(stderr, "Custo não cabe num byte %d\n", cost);//TODO retirar na versão a entregar para melhor performance
                exit(0);
            }
            //  Escrita no vetor
            tabuleiro_set_cost(tabuleiro, i, j, (char) cost);
        }
    }
}

/**
 * Lê o mapa mas apenas para avançar o cursor do ficheiro, porque o tabuleiro é invalido,
 *      logo não vale a pena guardar os valores
 * @param tabuleiro
 * @param fp        ficheiro de entrada
 */
void tabuleiro_read_matrix_from_file_invalid(Tabuleiro* tabuleiro, FILE* fp) {
    for(unsigned int j = 0; j<tabuleiro->height; j++) {
        for(unsigned int i = 0; i<tabuleiro->width; i++) {
            int cost;
            if(fscanf(fp, "%d", &cost) != 1) {
                fprintf(stderr, "Erro de leitura");
                exit(0);
            }
        }
    }
}

/**
 * Função privada; Faz as operações e escreve no ficheiro fp
 * @param tabuleiro
 * @param fp        ficheiro de saída
 */
void tabuleiro_execute_tipo_A(Tabuleiro *tabuleiro) {
    if(passeio_get_valid((Passeio*)tabuleiro->passeio) == 1 ) {
        best_choice(tabuleiro);
    }
}
/**
 * Função privada; Faz as operações e escreve no ficheiro fp
 * @param tabuleiro
 * @param fp        ficheiro de sáida
 */
void tabuleiro_execute_tipo_B(Tabuleiro *tabuleiro) {
    if(passeio_get_valid((Passeio*)tabuleiro->passeio) == 1 ) {
        possible_moves(tabuleiro);
    }
}

/**
 * Faz as operações e escreve no ficheiro fp
 * @param tabuleiro
 * @param fp        ficheiro de sáida
 */
void tabuleiro_execute(Tabuleiro *tabuleiro, FILE* fp) {
    if(tabuleiro->type_passeio == 'A') {
        tabuleiro_execute_tipo_A(tabuleiro);
    } else if(tabuleiro->type_passeio == 'B') {
        tabuleiro_execute_tipo_B(tabuleiro);
    } else if(tabuleiro->type_passeio == 'C') {
        fprintf(stderr, "we are not ready for C files\n");
        
    }
}

/**
 * Libertação de toda a memória alocada
 * @param tabuleiro
 */
void tabuleiro_free(Tabuleiro* tabuleiro) {
    free(passeio_B_get_pontos((Passeio*)tabuleiro_get_passeio(tabuleiro)));

    free(tabuleiro->passeio);
    free(tabuleiro->cost_matrix);
}

//TODO apagar
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

/**
 * Cria um ficheiro *.valid e escreve os resultados do tipo B
 * @param tabuleiro
 * @param fp        ficheiro de saída
 */
void tabuleiro_write_valid_file(Tabuleiro *tabuleiro, FILE* fp){
    Passeio* passeio = (Passeio*)tabuleiro_get_passeio(tabuleiro);

    //  Escreve no ficehiro
    fprintf(fp, "%d %d %c %d %d %d\n\n", tabuleiro->height, tabuleiro->width,
                                tabuleiro->type_passeio,
                                passeio_get_num_pontos(passeio),
                                passeio_get_valid(passeio),
                                passeio_B_get_cost(passeio));
}
