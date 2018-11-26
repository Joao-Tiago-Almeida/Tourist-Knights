#include "movimentos.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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

/**
 * Analisa um caminho vê se é válido e calcula o custo desse caminho
 * @param tabuleiro
 */
void possible_moves(Tabuleiro *tabuleiro){
    // NOTA: a cidade de partida não tem custo
    tabuleiro_passeio_set_cost(tabuleiro, 0);

    //  Caso da primeira cidade estar fora do mapa ou estievr fecahda
    if(!((inside_board(tabuleiro, tabuleiro_passeio_get_pontos(tabuleiro)[0]))  &&
            city_valid(tabuleiro, tabuleiro_passeio_get_pontos(tabuleiro)[0]))){

        //Passeio invalido
        tabuleiro_set_valid(tabuleiro, -1);
        return;
    }


    //  Para cada cidade, verifica-se se a cidade segiunte do caminho é válida
    for(int i = 0; i < tabuleiro_get_num_pontos(tabuleiro) - 1; i++) {
        //printf("Loop do num pontos %d\n", tabuleiro_get_num_pontos(passeio));
        //printf("i: %d\n", i);

        Vector2 point1 = tabuleiro_passeio_get_pontos(tabuleiro)[i];
        Vector2 point2 = tabuleiro_passeio_get_pontos(tabuleiro)[i+1];

        //TODO verificar ordem (inside_board e city_valid)
        if(!(inside_board(tabuleiro, point2) && do_points_make_L(point1, point2) && city_valid(tabuleiro, point2))) {
            //Se os dois pontos não fizerem um L entre si ou não for uma
            //  cidade válida o caminho não é válido
            tabuleiro_passeio_set_cost(tabuleiro, 0);
            tabuleiro_set_valid(tabuleiro, -1);
            return;
        }

        tabuleiro_passeio_set_cost(tabuleiro, tabuleiro_passeio_get_cost(tabuleiro) + (int)tabuleiro_get_cost(tabuleiro, point2));
        //printf("total: %d\n", tabuleiro_passeio_get_cost(passeio));
    }

    tabuleiro_set_valid(tabuleiro, 1);
}

/**
 * Identifica o ponto com menor custo à distância de um salto de cavalo
 * @param vec       cidade de partida
 * @param tabuleiro
 */
void best_choice(Tabuleiro *tabuleiro){

    int best = __INT32_MAX__;

    //  Caso da primeiroa cidade estar fora do mapa ou estievr fecahda
    if(!((inside_board(tabuleiro, tabuleiro_passeio_get_pos_ini(tabuleiro)))  &&
            city_valid(tabuleiro, tabuleiro_passeio_get_pos_ini(tabuleiro)))){
        tabuleiro_passeio_set_cost(tabuleiro, 0);
        tabuleiro_set_valid(tabuleiro, -1);
        return;
    }
    // vetor de comparação
    Vector2 movement = {0,0};

    for(int i = 0; i < NUM_PONTOS_A; i++) {
        for(int j = 0; j < MOVES; j++) {
            movement.x = knight_L[j].x + tabuleiro_passeio_get_pos_ini(tabuleiro).x;
            movement.y = knight_L[j].y + tabuleiro_passeio_get_pos_ini(tabuleiro).y;

            if((inside_board(tabuleiro, movement) && (city_valid(tabuleiro, movement)))){
                best = (best < tabuleiro_get_cost(tabuleiro, movement) ?
                            best : tabuleiro_get_cost(tabuleiro, movement));
            }
        }
    }

    if(best == __INT32_MAX__) {
        tabuleiro_passeio_set_cost(tabuleiro, 0);
        tabuleiro_set_valid(tabuleiro, -1); //TODO criar passeio set invalid
        return;
    }

    tabuleiro_passeio_set_cost(tabuleiro, best);
    tabuleiro_set_valid(tabuleiro, 1);
}


void movimentos_find_path(Tabuleiro* tabuleiro, Vector2 ini, Vector2 dest) {
    Acervo* fila = new_acervo(100); //TODO valor inicial

    //Init matrizes
    tabuleiro_init_st_wt(tabuleiro);

    //Init algoritmo
    tabuleiro_set_wt_val(tabuleiro, ini, 0);
    insert_city(fila, ini, tabuleiro);

    while(!acervo_is_empty(fila)) {
        Vector2 v = pick_best_city(fila);
        int wt_value = tabuleiro_get_wt_val(tabuleiro, v);
        if(wt_value != -1) {
            acervo_remove(fila, tabuleiro, v);
        }

        for(int mov_rel = 0; mov_rel<8; mov_rel++) {
            //Vizinhos de v
            Vector2 pos_to_try = vector2_add(v, knight_L[mov_rel]);

            if(!inside_board(tabuleiro, pos_to_try)) {
                //Se for uma posicao invalida, passa à frente
                continue;
            }

            int old_wt_val = tabuleiro_get_wt_val(tabuleiro, pos_to_try);
            if(old_wt_val != -1) {
                //Se o custo não for infinito
                int cost = tabuleiro_get_cost(tabuleiro, pos_to_try);
                int new_wt_val = wt_value + cost;

                if(old_wt_val > new_wt_val) {
                    //Se agora o custo for menor até lá, mudar a sua posicao na fila
                    
                    tabuleiro_set_wt_val(tabuleiro, pos_to_try, new_wt_val);
                    acervo_update_or_insert(fila, tabuleiro, pos_to_try);  // (atualiza se já tiver no acervo/ insere ordenado)

                    //Fica ao contrario (em vez de dizer que subiu 2, tá a dizer que desceu dois)
                    //Pq o st é suposto dizer a direção para ir do pos_to_try ao v, e não o contrario
                    tabuleiro_set_st_val(tabuleiro, pos_to_try, mov_rel);
                }
            }
        }
    }


    //Percorrer o caminho ao contrario
    Vector2 p = dest;
    int st;
    while(true) {
        st = tabuleiro_get_st_val(tabuleiro, p);
        printf("%d %d\n", p.x, p.y);

        if(st == -1)
            break; //Se chegou ao fim

        p.x = p.x - knight_L[st].x;
        p.y = p.y - knight_L[st].y;
    }


    tabuleiro_free_st_wt(tabuleiro);
    free(fila);
}