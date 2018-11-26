#include "tabuleiro.h"

#include <stdlib.h>

#include "util.h"
#include "movimentos.h"

struct tabuleiro_t {
    unsigned int width, height;
    unsigned char* cost_matrix; //Cada custo tem um byte
    char type_passeio;

    int* wt;
    char* st;

    int num_pontos;
    Vector2* pontos;
    int cost;  //parte 1, para guardar o custo do camimho se válido
    char valid;
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

    tab->cost_matrix = NULL; // Nao aloca ainda a matriz porque pode ser que o passeio seja invalido

    return tab;
}

unsigned int tabuleiro_get_width(Tabuleiro* tabuleiro) {
    return tabuleiro->width;
}

unsigned int tabuleiro_get_height(Tabuleiro* tabuleiro) {
    return tabuleiro->height;
}

void tabuleiro_set_cost(Tabuleiro* tabuleiro, Vector2 vec, unsigned char cost) {
    tabuleiro->cost_matrix[vec.x + vec.y*tabuleiro->width] = cost;
}

int tabuleiro_get_cost(Tabuleiro* tabuleiro, Vector2 vec) {
    return tabuleiro->cost_matrix[vec.x + vec.y*tabuleiro->width];
}

void tabuleiro_set_wt_val(Tabuleiro* tabuleiro, Vector2 vec, int value) {
    tabuleiro->wt[vec.x + vec.y*tabuleiro->width] = value;
}
int tabuleiro_get_wt_val(Tabuleiro* tabuleiro, Vector2 vec) {
    return tabuleiro->wt[vec.x + vec.y*tabuleiro->width];
}
void tabuleiro_set_st_val(Tabuleiro* tabuleiro, Vector2 vec, char value) {
    tabuleiro->st[vec.x + vec.y*tabuleiro->width] = value;
}
char tabuleiro_get_st_val(Tabuleiro* tabuleiro, Vector2 vec) {
    return tabuleiro->st[vec.x + vec.y*tabuleiro->width];
}

void tabuleiro_init_st_wt(Tabuleiro* tabuleiro) {
    int w, h;
    w = tabuleiro_get_width(tabuleiro);
    h = tabuleiro_get_height(tabuleiro);
    
    tabuleiro->wt = (int*)checked_malloc(w * h * sizeof(int));
    tabuleiro->st = (char*)checked_malloc(w * h * sizeof(char));
    for(int j = 0; j<h; j++) {
        for(int i = 0; i<w; i++) {
            tabuleiro->wt[i + j*w] = -1;
            tabuleiro->st[i + j*w] = -1;
        }
    }
}

void tabuleiro_free_st_wt(Tabuleiro* tabuleiro) {
    free(tabuleiro->wt);
    tabuleiro->wt = NULL;
    free(tabuleiro->st);
    tabuleiro->st = NULL;
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
    tabuleiro->cost_matrix = (unsigned char*) checked_malloc(sizeof(unsigned char) * (tabuleiro->width * tabuleiro->height));

    Vector2 vec;

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
            vec = vector2_new(i,j);
            tabuleiro_set_cost(tabuleiro, vec, (unsigned char) cost);
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
    if(passeio_get_valid(tabuleiro) != 1)
        return;

    movimentos_find_path(tabuleiro,
        tabuleiro_passeio_get_pos_ini(tabuleiro),
        tabuleiro_passeio_get_pontos(tabuleiro)[1]);
}

/**
 * Função privada; Faz as operações e escreve no ficheiro fp
 * @param tabuleiro
 * @param fp        ficheiro de sáida
 */
void tabuleiro_execute_tipo_B(Tabuleiro *tabuleiro) {
    if(passeio_get_valid(tabuleiro) != 1)
        return;

    possible_moves(tabuleiro);
}

/**
 * Faz as operações e escreve no ficheiro fp
 * @param tabuleiro
 * @param fp        ficheiro de sáida
 */
void tabuleiro_execute(Tabuleiro *tabuleiro) {
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
    free(tabuleiro_passeio_get_pontos(tabuleiro/*(Passeio*)tabuleiro_get_passeio(tabuleiro)*/));

    if(tabuleiro->type_passeio == 'A' || tabuleiro->type_passeio == 'B' || tabuleiro->type_passeio == 'C')
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
    //  Escreve no ficehiro
    fprintf(fp, "%d %d %c %d %d %d\n\n", tabuleiro->height, tabuleiro->width,
                                tabuleiro->type_passeio,
                                tabuleiro->num_pontos,
                                tabuleiro->valid,
                                tabuleiro->cost);
}

/**
 * Escreve o número de cidades e lê as coordenadas de cidade a visitar do novo passeio do ficehiro
 * @param  tabuleiro
 * @param  num_pontos   número de cidades a visitar
 * @param  fp         ficheiro de leitura
 */
void tabuleiro_read_passeio_from_file(Tabuleiro* tabuleiro, int num_pontos, FILE* fp) {
    tabuleiro->num_pontos = num_pontos;
    tabuleiro->pontos = (Vector2*) checked_malloc(sizeof(Vector2) * num_pontos);

    tabuleiro->valid = 1;
    tabuleiro->cost = 0;

    //escreve no vetor
    for(int i = 0; i < num_pontos; i++)
    {
        Vector2 vec = vector2_read_from_file(fp);
        tabuleiro->pontos[i] = vec;

        if(!inside_board(tabuleiro, vec)) {
            //Se algum dos pontos estiver fora do tabuleiro marca o passeio como invalido
            tabuleiro->valid = -1;
        }

    }
}

void tabuleiro_set_valid(Tabuleiro* tabuleiro, char valid) {
    tabuleiro->valid = valid;
}

char passeio_get_valid(Tabuleiro* tabuleiro) {
    return tabuleiro->valid;
}

void tabuleiro_passeio_set_cost(Tabuleiro* tabuleiro, int cost) {
    tabuleiro->cost = cost;
}

int tabuleiro_passeio_get_cost(Tabuleiro* tabuleiro) {
    return tabuleiro->cost;
}

int tabuleiro_get_num_pontos(Tabuleiro* tabuleiro) {
    return tabuleiro->num_pontos;
}

Vector2* tabuleiro_passeio_get_pontos(Tabuleiro* tabuleiro) {
    return tabuleiro->pontos;
}

Vector2 tabuleiro_passeio_get_pos_ini(Tabuleiro* tabuleiro) {
    return tabuleiro->pontos[0];
}
