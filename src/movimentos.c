#include "movimentos.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "passeios.h"

#define NUM_PONTOS_A 1


#define MOVES 8

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
    return tabuleiro_get_cost(tabuleiro, vec.x, vec.y) != 0;
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

    Passeio* passeio = (Passeio*)tabuleiro_get_passeio(tabuleiro);

    // NOTA: a cidade de partida não tem custo
    passeio_B_set_cost(passeio, 0);

    //  Caso da primeira cidade estar fora do mapa ou estievr fecahda
    if(!((inside_board(tabuleiro, passeio_B_get_pontos(passeio)[0]))  &&
            city_valid(tabuleiro, passeio_B_get_pontos(passeio)[0]))){

        //Passeio invalido
        passeio_set_valid(passeio, -1);
        return;
    }


    //  Para cada cidade, verifica-se se a cidade segiunte do caminho é válida
    for(int i = 0; i < passeio_get_num_pontos(passeio) - 1; i++) {
        //printf("Loop do num pontos %d\n", passeio_get_num_pontos(passeio));
        //printf("i: %d\n", i);
        
        Vector2 point1 = passeio_B_get_pontos(passeio)[i];
        Vector2 point2 = passeio_B_get_pontos(passeio)[i+1];
        
        //TODO verificar ordem (inside_board e city_valid)
        if(!(inside_board(tabuleiro, point2) && do_points_make_L(point1, point2) && city_valid(tabuleiro, point2))) {
            //Se os dois pontos não fizerem um L entre si ou não for uma
            //  cidade válida o caminho não é válido
            passeio_B_set_cost(passeio, 0);
            passeio_set_valid(passeio, -1);
            return;
        }

        passeio_B_set_cost(passeio, passeio_B_get_cost(passeio) + (int)tabuleiro_get_cost(tabuleiro, point2.x, point2.y));
        //printf("total: %d\n", passeio_B_get_cost(passeio));
    }

    passeio_set_valid(passeio, 1);
}

/**
 * Identifica o ponto com menor custo à distância de um salto de cavalo
 * @param vec       cidade de partida
 * @param tabuleiro
 */
void best_choice(Tabuleiro *tabuleiro){

    int best = __INT32_MAX__;

    Passeio* passeio = (Passeio*)(tabuleiro_get_passeio(tabuleiro));

    //  Caso da primeiroa cidade estar fora do mapa ou estievr fecahda
    if(!((inside_board(tabuleiro, passeio_get_pos_ini(passeio)))  &&
            city_valid(tabuleiro, passeio_get_pos_ini(passeio)))){
        passeio_B_set_cost(passeio, 0);
        passeio_set_valid(passeio, -1);
        return;
    }
    //  vetor que guarda as coordenadas dos oito movimentos possiveis
    Vector2 knight_L[MOVES] = { {1,2}, {2,1}, {2,-1}, {1,-2},
                            {-1,-2}, {-2,-1}, {-2,1}, {-1,2} };
    // vetor de comparação
    Vector2 movement = {0,0};

    for(int i = 0; i < NUM_PONTOS_A; i++) {
        for(int j = 0; j < MOVES; j++) {
            movement.x = knight_L[j].x + passeio_get_pos_ini(passeio).x;
            movement.y = knight_L[j].y + passeio_get_pos_ini(passeio).y;

            if((inside_board(tabuleiro, movement) && (city_valid(tabuleiro, movement)))){
                best = (best < tabuleiro_get_cost(tabuleiro, movement.x, movement.y) ?
                            best : tabuleiro_get_cost(tabuleiro, movement.x, movement.y));
            }
        }
    }

    if(best == __INT32_MAX__) {
        passeio_B_set_cost(passeio, 0);
        passeio_set_valid(passeio, -1); //TODO criar passeio set invalid
        return;
    }

    passeio_B_set_cost(passeio, best);
    passeio_set_valid(passeio, 1);
}
