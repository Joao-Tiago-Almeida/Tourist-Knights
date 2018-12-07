#include "tabuleiro.h"

#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "movimentos.h"
#include "acervo.h"
#include "path.h"

#define PERCENTAGEM_PRE_ALOCADA_DO_ACERVO 0.5

struct tabuleiro_t {
    unsigned int width, height;
    char type_passeio;

    unsigned char* cost_matrix; //Cada custo tem um byte
    unsigned short* wt;
    char* st;
    //TODO criar matriz com o indice

    Acervo *fila;

    Path* paths;
    int num_paths_calculated; //Numero de caminhos que já estão no vetor paths

    int num_pontos;
    Vector2* pontos;
    int cost;  //parte 1, para guardar o custo do camimho se válido
    bool valid;
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

    tab->wt = NULL;
    tab->st = NULL;

    tab->num_paths_calculated = 0;

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
    if(value > 65535) {
        fprintf(stderr, "Valor maior que unsigned short!!\n");
        exit(1); //TODO tirar
    }
    tabuleiro->wt[vec.x + vec.y*tabuleiro->width] = (unsigned short)value;
}
int tabuleiro_get_wt_val(Tabuleiro* tabuleiro, Vector2 vec) {
    return (int)tabuleiro->wt[vec.x + vec.y*tabuleiro->width];
}
void tabuleiro_set_st_val(Tabuleiro* tabuleiro, Vector2 vec, char value) {
    tabuleiro->st[vec.x + vec.y*tabuleiro->width] = value;
}
char tabuleiro_get_st_val(Tabuleiro* tabuleiro, Vector2 vec) {
    return tabuleiro->st[vec.x + vec.y*tabuleiro->width];
}

Path tabuleiro_get_path(Tabuleiro* tabuleiro, int i) {
    return tabuleiro->paths[i];
}
int tabuleiro_get_num_path_calculated(Tabuleiro* tabuleiro) {
    return tabuleiro->num_paths_calculated;
}

void *tabuleiro_get_fila(Tabuleiro* tabuleiro) {
    return tabuleiro->fila;
}

void tabuleiro_init_st_wt(Tabuleiro* tabuleiro) {
    int w, h;
    w = tabuleiro_get_width(tabuleiro);
    h = tabuleiro_get_height(tabuleiro);

    if(tabuleiro->wt == NULL) {
        tabuleiro->wt = (unsigned short*)checked_malloc(tabuleiro->width * tabuleiro->height * sizeof(unsigned short));
        tabuleiro->st = (char*)checked_malloc(tabuleiro->width * tabuleiro->height * sizeof(char));
    }

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

    // Expressão inicial de alocação de memómira
    int index_matrix_size = tabuleiro->width * tabuleiro->height;
    tabuleiro->fila = new_acervo(index_matrix_size * PERCENTAGEM_PRE_ALOCADA_DO_ACERVO, index_matrix_size);
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

//Verifica se alguma cidade do passeio é invalida, se for torna o problema invalido
void tabuleiro_check_passeio_invalid(Tabuleiro* tabuleiro) {
    for(int i = 0; i<tabuleiro->num_pontos; i++) {
        if(!(tabuleiro_get_cost(tabuleiro, tabuleiro->pontos[i]) != 0 &&
                check_if_city_accessible(tabuleiro, tabuleiro->pontos[i]))) {
            tabuleiro_set_valid(tabuleiro, false);
            return;
        }
    }
}

/**
 * Função privada; Faz as operações e escreve no ficheiro fp
 * @param tabuleiro
 * @param fp        ficheiro de saída
 */
static void tabuleiro_execute_tipo_A(Tabuleiro *tabuleiro) {
    tabuleiro->paths[0] = movimentos_find_path(tabuleiro,
        tabuleiro_passeio_get_pos_ini(tabuleiro),
        tabuleiro_passeio_get_pontos(tabuleiro)[1]);

    tabuleiro_passeio_set_cost(tabuleiro, tabuleiro->paths[0].cost);
}


/**
 * Função privada; Faz as operações e escreve no ficheiro fp
 * @param tabuleiro
 * @param fp        ficheiro de sáida
 */
static void tabuleiro_execute_tipo_B(Tabuleiro *tabuleiro) {
    int cost = 0;

    for(int i = 0; i<tabuleiro->num_pontos-1; i++) {
        Vector2 orig = tabuleiro_passeio_get_pontos(tabuleiro)[i];
        Vector2 dest = tabuleiro_passeio_get_pontos(tabuleiro)[i+1];

        tabuleiro->paths[i] = movimentos_find_path(tabuleiro, orig, dest);

        cost += tabuleiro->paths[i].cost;
        tabuleiro->num_paths_calculated++;
    }

    tabuleiro_passeio_set_cost(tabuleiro, cost);
}


void trocar(unsigned short *vec_cidades_tmp, int a, int b) {
    int temp = vec_cidades_tmp[b];
    vec_cidades_tmp[b] = vec_cidades_tmp[a];
    vec_cidades_tmp[a] = temp;
}

void calcula_melhor_caminho(unsigned short *vec_cidades_tmp, int num_cidades, int inicio, Path* matriz_custo_caminhos,
    int* min_custo, unsigned short *vec_cidades_min) {

    if (num_cidades == inicio) {
        //Se chegou ao fim e já não há elementos para permutar, temos o
        unsigned short cost = 0;
        //Calcula o custo de percorrer as cidades na ordem atual do vec_cidades_tmp
        for(int i = 0; i<num_cidades-1; i++) {
            int orig_idx = vec_cidades_tmp[i];
            int dest_idx = vec_cidades_tmp[i+1];

            //Soma o custo de ir da cidade orig_idx para a cidade dest_idx
            //printf("a %d, %d\n", orig_idx, dest_idx);
            cost += matriz_custo_caminhos[orig_idx + (dest_idx-1)*num_cidades].cost; //Num cidades é a largura(w) do tabuleiro
            if(cost >= *min_custo && *min_custo != -1)
                return;
        }

        //Encontrou-se um caminho melhor, ou é o primeiro caminho testado (-1 representa infinito)
            #ifdef DEBUG
                printf("[");
                for(int i = 0; i<num_cidades; i++) {
                    printf("%d ", vec_cidades_tmp[i]);
                }
                printf("] custo: %d\n", cost);
            #endif
        if(*min_custo == -1 || cost < *min_custo) {
            *min_custo = cost;


            memcpy(vec_cidades_min, vec_cidades_tmp, num_cidades*sizeof(unsigned short));
        }
        return;
    }

    for (int j = inicio; j < num_cidades; j++) {
        if(inicio == j) {
            calcula_melhor_caminho(vec_cidades_tmp, num_cidades, inicio+1, matriz_custo_caminhos,
                min_custo, vec_cidades_min);
        } else {
            trocar(vec_cidades_tmp, inicio, j);
            calcula_melhor_caminho(vec_cidades_tmp, num_cidades, inicio+1, matriz_custo_caminhos,
                min_custo, vec_cidades_min);
            trocar(vec_cidades_tmp, inicio, j);
        }
    }
}

static void calcula_matriz_custo_caminhos(Tabuleiro *tabuleiro, Path* matriz_custo_caminhos) {
    int w = tabuleiro->num_pontos;

    //Inicializa toda a matriz para que o vetor dos points em sitios invalidos da matriz não seja libertado
    for(int i = 0; i<tabuleiro->num_pontos; i++) {
        for(int j = 1; j<tabuleiro->num_pontos; j++) { //J numero da cidade, não o numero da linha (é j-1)
            matriz_custo_caminhos[i + (j-1)*w].points = NULL;
        }
    }

    for(int i = 0; i<tabuleiro->num_pontos; i++) {
        for(int j = 1; j<tabuleiro->num_pontos; j++) { //J numero da cidade, não o numero da linha (é j-1)

            //Calcular o caminho do ponto i ao j
            if(i >= j) {
                //Não calcular a trigular superior da matriz
                //  Não calcular caminho entre a mesma cidade e caminhos com cidades extremos iguais
                continue;
            }

            Vector2 orig = tabuleiro_passeio_get_pontos(tabuleiro)[i];
            Vector2 dest = tabuleiro_passeio_get_pontos(tabuleiro)[j];

            /*unsigned short cost = movimentos_find_better_path_cost(tabuleiro,
                orig,
                dest);*/
            Path path = movimentos_find_path(tabuleiro, orig, dest);

            //Custo de ir de i(orig) para j(dest)
            matriz_custo_caminhos[i + (j-1)*w] = path;
            //printf("%d, %d\n", i, j);


            //Cidade de partida é a cidade inicial do caminho, logo não calcula o caminho inverso
            if(i > 0) {
                //Calcula o custo do caminho inverso a partir do calculo anterior
                //  (custo anterior - destino anteiror(origem do inverso) + origem anterior(destino do inverso))
                //matriz_custo_caminhos[j + (i-1)*w] = (unsigned short)(cost - tabuleiro_get_cost(tabuleiro, dest) + tabuleiro_get_cost(tabuleiro, orig));
                matriz_custo_caminhos[j + (i-1)*w] = inverte_caminho(tabuleiro, path);
                //printf("\t %d, %d\n", i+1, j-1);
            }
        }
    }
}

/**
 * Função privada; Faz as operações e escreve no ficheiro fp
 * @param tabuleiro
 * @param fp        ficheiro de sáida
 */
void tabuleiro_execute_tipo_C(Tabuleiro *tabuleiro) {
    //Se tiver N pontos, o de origem é fixo, por isso vai-se coiserar as permutacoes entre 3 pontos
    //int num_caminhos = tabuleiro->num_pontos - 1;

    int w = tabuleiro->num_pontos;
    int h = w - 1;

    //Matriz que tem o custo de ir de um ponto para o outro (Não tem uma linha nunca se vai para o ponto inicial)
    //Linha - origem, coluna - destino
    Path* matriz_custo_caminhos = (Path*)checked_malloc(sizeof(Path) * w * h);
    calcula_matriz_custo_caminhos(tabuleiro, matriz_custo_caminhos);

    //Com os custos de cada trajeto possivel calculados, calcula-se as permutacoes
    unsigned short *vec_cidades_tmp = (unsigned short*) checked_malloc(sizeof(unsigned short) * tabuleiro->num_pontos);
    unsigned short *vec_cidades_final = (unsigned short*) checked_malloc(sizeof(unsigned short) * tabuleiro->num_pontos);
    int min_cost = -1;

    //Prenche o vetor de indices de cidades com a ordem inicial [x0, x1, x2, ..., xn]
    for(int i = 0; i<tabuleiro->num_pontos; i++) {
        vec_cidades_tmp[i] = i;
    }

    //Calcula o melhor caminho, fazendo permutacoes a partir do primeiro indice
    //  (primeira cidade x0 é fixa)
    calcula_melhor_caminho(vec_cidades_tmp, tabuleiro->num_pontos, 1,
                                matriz_custo_caminhos, &min_cost, vec_cidades_final);
    free(vec_cidades_tmp);

    //vec_cidades_final vai ter melhor ordem de cidades para realizar o caminho

    //Calcula os caminhos a realizar, com a ordem do vec_cidades_final e escreve-os no tabuleiro->paths
    int cost = 0;

    //printf("[");
    for(int i = 0; i<tabuleiro->num_pontos-1; i++) {
        //printf("%d ", vec_cidades_final[i]);

        /*tabuleiro->paths[i] = movimentos_find_path(tabuleiro,
            tabuleiro_passeio_get_pontos(tabuleiro)[vec_cidades_final[i]],
            tabuleiro_passeio_get_pontos(tabuleiro)[vec_cidades_final[i+1]]);*/

        int orig_idx = vec_cidades_final[i];
        int dest_idx = vec_cidades_final[i+1];
        tabuleiro->paths[i] = matriz_custo_caminhos[orig_idx + (dest_idx-1)*w];
        //Coloca o caminho a NULL porque esse vetor agora está no tabuleiro->paths, assim não será libertado a seguir
        matriz_custo_caminhos[orig_idx + (dest_idx-1)*w].points = NULL;
        cost += tabuleiro->paths[i].cost;
    }
    for(int i = 0; i<w; i++) {
        for(int j = 0; j<h; j++) {
            free(matriz_custo_caminhos[i + j*w].points);
        }
    }
    //printf("%d]", vec_cidades_final[tabuleiro->num_pontos-1]);

    tabuleiro_passeio_set_cost(tabuleiro, cost);

    free(vec_cidades_final);
    free(matriz_custo_caminhos);
}

/**
 * Faz as operações e escreve no ficheiro fp
 * @param tabuleiro
 * @param fp        ficheiro de sáida
 */
void tabuleiro_execute(Tabuleiro *tabuleiro) {
    if(!passeio_get_valid(tabuleiro))
        return;

    if(tabuleiro->type_passeio == 'A') {
        tabuleiro_execute_tipo_A(tabuleiro);
    } else if(tabuleiro->type_passeio == 'B') {
        tabuleiro_execute_tipo_B(tabuleiro);
    } else if(tabuleiro->type_passeio == 'C') {
        tabuleiro_execute_tipo_C(tabuleiro);

    }
}

/**
 * Libertação de toda a memória alocada
 * @param tabuleiro
 */
void tabuleiro_free(Tabuleiro* tabuleiro) {
    free(tabuleiro_passeio_get_pontos(tabuleiro/*(Passeio*)tabuleiro_get_passeio(tabuleiro)*/));
    tabuleiro_free_st_wt(tabuleiro);

    if(tabuleiro->fila != NULL)
        acervo_free(&(tabuleiro->fila));

    if(tabuleiro->type_passeio == 'A' || tabuleiro->type_passeio == 'B' || tabuleiro->type_passeio == 'C')
        free(tabuleiro->cost_matrix);

    for(int path_i = 0; path_i<tabuleiro->num_pontos-1; path_i++) {
        //Liberta cada caminho
        free(tabuleiro->paths[path_i].points);
    }
    free(tabuleiro->paths);
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

static void imprime_caminho(Tabuleiro* tabuleiro, Path path, FILE * fp) {
    for(int i = 0; i<path.length; i++) {
        Vector2 p = path.points[i];
        fprintf(fp, "%d %d %d\n", p.y, p.x, tabuleiro_get_cost(tabuleiro, p));
    }
}


/**
 * Cria um ficheiro *.valid e escreve os resultados do tipo B
 * @param tabuleiro
 * @param fp        ficheiro de saída
 */
void tabuleiro_write_valid_file(Tabuleiro *tabuleiro, FILE* fp) {
    int num_pontos_passagem = 0;
    bool valid = tabuleiro->valid;

    if(valid) {
        for(int path_i = 0; path_i<tabuleiro->num_pontos-1; path_i++) {
            num_pontos_passagem += tabuleiro->paths[path_i].length;
        }
    }

    //  Escreve no ficehiro
    fprintf(fp, "%d %d %c %d ", tabuleiro->height, tabuleiro->width,
                                tabuleiro->type_passeio,
                                tabuleiro->num_pontos);

    if(valid) {
        fprintf(fp, "%d %d\n", tabuleiro->cost,
                            num_pontos_passagem);
    } else {
        fprintf(fp, "-1 0\n");
    }

    if(valid) {
        for(int path_i = 0; path_i<tabuleiro->num_pontos-1; path_i++) {
            //Imprime cada caminho
            imprime_caminho(tabuleiro, tabuleiro->paths[path_i], fp);
        }
    }
    fprintf(fp, "\n");
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

    //Vão haver num_pontos-1 caminhos, porque vai haver um caminho entre cada dois pontos
    //Calloc coloca os apontadores para os vetores de pontos todos a null
    tabuleiro->paths = (Path*) checked_calloc(sizeof(Path), num_pontos-1);

    tabuleiro->valid = true;
    tabuleiro->cost = 0;

    //escreve no vetor
    for(int i = 0; i < num_pontos; i++)
    {
        Vector2 vec = vector2_read_from_file(fp);
        tabuleiro->pontos[i] = vec;

        if(!inside_board(tabuleiro, vec)) {
            //Se algum dos pontos estiver fora do tabuleiro marca o passeio como invalido
            tabuleiro->valid = false;
        }

    }
}

void tabuleiro_set_valid(Tabuleiro* tabuleiro, bool valid) {
    tabuleiro->valid = valid;
}

bool passeio_get_valid(Tabuleiro* tabuleiro) {
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
