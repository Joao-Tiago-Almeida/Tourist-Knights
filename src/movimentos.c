#include "movimentos.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "util.h"
#include "acervo.h"

#define NUM_PONTOS_A 1

#define MOVES 8

//  vetor que guarda as coordenadas dos oito movimentos possiveis
const Vector2 knight_L[MOVES] = { {1,2}, {2,1}, {2,-1}, {1,-2},
                        {-1,-2}, {-2,-1}, {-2,1}, {-1,2} };

//TODO definir qual a descrição de tabuleiro a usar
/**
 * Verifica quando um ponto está dentro dos limites do tabuleiro
 * @param  vec       vetor de pontos a analisar
 * @param  tabuleiro [description]
 * @return           Retorna true quando um ponto está dentro do tabuleiro,
 *                          e false quando se encontra fora do tabuleiros
 */
bool inside_board(Tabuleiro* tabuleiro, Vector2 vec){

    //  Ver se o ponto está entre os eixos verticais do tabuleiro
    if(vec.x < 0 || vec.x > (int)tabuleiro_get_width(tabuleiro)-1){

        //fprintf(stderr, "\npoint out of the board\n");
        return false;
    }
    //  Ver se o ponto está entre os eixos horizointais do tabuleiro
    else if(vec.y < 0 || vec.y > (int)tabuleiro_get_height(tabuleiro)-1){

        //fprintf(stderr, "\npoint out of the board\n");
        return false;
    }
    return true;
}

/**
 * Verifica se a cidade com coordenadas vec é válida (custo != 0)
 * @param  tabuleiro [description]
 * @param  vec       [description]
 * @return           retorna true se a cidade não estiver fechada
 */
bool city_valid(Tabuleiro* tabuleiro, Vector2 vec) {
    return tabuleiro_get_cost(tabuleiro, vec) != 0;
}

/*
 * Devolve true se dois pontos fizerem um L entre si
 */
bool do_points_make_L(Vector2 point1, Vector2 point2) {
    return (abs(point1.x-point2.x) == 2 && abs(point1.y-point2.y) == 1) ||
        (abs(point1.x-point2.x) == 1 && abs(point1.y-point2.y) == 2);
}

//Devolve o caminho mais curto entre dois pontos
//apenas escreve na estrutura Path o vetor de pontos de passagem alloc_vec_pontos se for true
static Path dijkstra(Tabuleiro* tabuleiro, Vector2 ini, Vector2 dest, bool alloc_vec_pontos) {
    Path path;
    path.length = 0;
    path.cost = 0;
    path.points = NULL;

    if(vector2_equals(ini, dest)) {
        return path;
    }

    //Init matrizes (inicializa-se tudo a -1, (wt -- representa infinito; st -- representa que não tem ajdência))
    tabuleiro_init_st_wt(tabuleiro);

    //Init algoritmo
    tabuleiro_set_wt_val(tabuleiro, ini, 0);
    acervo_insert((Acervo *)(Acervo *)tabuleiro_get_fila(tabuleiro), ini, tabuleiro);
    #ifdef DEBUG
    printf("Insere %d,%d\n", ini.x, ini.y);
    #endif

    while(!acervo_is_empty((Acervo *)tabuleiro_get_fila(tabuleiro))) { //TODO se encontrar o destino parar
        Vector2 v = acervo_get_top((Acervo *)tabuleiro_get_fila(tabuleiro));
        #ifdef DEBUG
        printf("\nV <- (%d,%d)\n", v.x, v.y);
        acervo_print((Acervo *)tabuleiro_get_fila(tabuleiro));
        #endif
        acervo_remove_top((Acervo *)tabuleiro_get_fila(tabuleiro), tabuleiro);

        int wt_value = tabuleiro_get_wt_val(tabuleiro, v);
        /*if(wt_value != -1) {
            acervo_remove((Acervo *)tabuleiro_get_fila(tabuleiro), v, tabuleiro);
        }*/

        for(int mov_rel = 0; mov_rel<8; mov_rel++) {
            //Vizinhos de v
            Vector2 pos_to_try = vector2_add(v, knight_L[mov_rel]);

            if(!(inside_board(tabuleiro, pos_to_try) && tabuleiro_get_cost(tabuleiro, pos_to_try) != 0)) {
                //Se for uma posicao invalida, passa à frente
                continue;
            }

            int old_wt_val = tabuleiro_get_wt_val(tabuleiro, pos_to_try);
            int new_wt_val = wt_value + tabuleiro_get_cost(tabuleiro, pos_to_try);

            if(old_wt_val != -1) {
                //Se o custo não for infinito

                if(old_wt_val > new_wt_val) {
                    //Se agora o custo for menor até lá, mudar a sua posicao na fila

                    tabuleiro_set_wt_val(tabuleiro, pos_to_try, new_wt_val);
                    //atualiza se já tiver no acervo/insere ordenado
                    acervo_update_or_insert((Acervo *)tabuleiro_get_fila(tabuleiro), pos_to_try, old_wt_val, tabuleiro);
                    #ifdef DEBUG
                    printf("Insere/Atualiza %d,%d\n", pos_to_try.x, pos_to_try.y);
                    #endif


                    //Fica ao contrario (em vez de dizer que subiu 2, tá a dizer que desceu dois)
                    //Pq o st é suposto dizer a direção para ir do pos_to_try ao v, e não o contrario
                    tabuleiro_set_st_val(tabuleiro, pos_to_try, mov_rel);
                }
            } else {
                //Se for a primeira vez que a casa é considerada
                tabuleiro_set_wt_val(tabuleiro, pos_to_try, new_wt_val);
                acervo_insert((Acervo *)tabuleiro_get_fila(tabuleiro), pos_to_try, tabuleiro);
                #ifdef DEBUG
                printf("Insere %d,%d\n", pos_to_try.x, pos_to_try.y);
                #endif
                tabuleiro_set_st_val(tabuleiro, pos_to_try, mov_rel);
            }
        }
    }

    //TODO parar quando todos na fila tiverem maior wt que o destino (nao há caminho melhor possivel)

    if(tabuleiro_get_st_val(tabuleiro, dest) == -1) {
        //Se não há caminho para o ponto de destino, define o problema como invalido
        tabuleiro_set_valid(tabuleiro, false);
        return path;
    }

    //Percorrer o caminho ao contrario para saber o tamanho necessario para o vetor
    Vector2 p = dest;
    int st;
    while(true) {
        st = tabuleiro_get_st_val(tabuleiro, p);
        if(st == -1)
            break; //Se chegou ao fim

        p = vector2_sub(p, knight_L[st]);

        path.length++;
    }

    int i = 0;
    
    if(alloc_vec_pontos)
        path.points = (Vector2*)checked_malloc(sizeof(Vector2) * path.length);

    p = dest;
    while(true) {
        st = tabuleiro_get_st_val(tabuleiro, p);

        if(st == -1)
            break; //Se chegou ao fim

        if(alloc_vec_pontos)
            path.points[(path.length-1) - i] = p; //Esreve no vetor do inicio para o fim

        path.cost += tabuleiro_get_cost(tabuleiro, p);

        p = vector2_sub(p, knight_L[st]);
        i++;
    }

    return path;
}

unsigned short movimentos_find_better_path_cost(Tabuleiro* tabuleiro, Vector2 ini, Vector2 dest) {
    int cost = dijkstra(tabuleiro, ini, dest, false).cost;
    if(cost > 65535) {
        fprintf(stderr, "Valor maior que unsigned short!!\n");
        exit(1); //TODO tirar
    }
    return cost;
}

Path movimentos_find_path(Tabuleiro* tabuleiro, Vector2 ini, Vector2 dest) {
    return dijkstra(tabuleiro, ini, dest, true);
}
