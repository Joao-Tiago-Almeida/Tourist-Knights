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
    if(vec.x < 0 || vec.x > tabuleiro_get_width(tabuleiro)-1){

        //fprintf(stderr, "\npoint out of the board\n");
        return false;
    }
    //  Ver se o ponto está entre os eixos horizointais do tabuleiro
    else if(vec.y < 0 || vec.y > tabuleiro_get_height(tabuleiro)-1){

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

//TODO comentario bonito
// Devolve true se dois pontos fizerem um L entre si
bool do_points_make_L(Vector2 point1, Vector2 point2) {
    return (abs(point1.x-point2.x) == 2 && abs(point1.y-point2.y) == 1) ||
        (abs(point1.x-point2.x) == 1 && abs(point1.y-point2.y) == 2);
}

/**
 * Analisa um caminho vê se é válido e calcula o custo desse caminho
 * @param tabuleiro [description]
 */
void possible_moves(Tabuleiro *tabuleiro){
    PasseioTipoB* passeio = (PasseioTipoB*)tabuleiro_get_passeio(tabuleiro);

    // NOTA: a cidade de partida não tem custo
    passeio->cost = 0;

    //  Caso da primeira cidade estar fora do mapa ou estievr fecahda
    if(!((inside_board(tabuleiro, (passeio)->pontos[0]))  &&
            city_valid(tabuleiro, (passeio)->pontos[0]))){
        
        //Passeio invalido
        ((PasseioTipoB*)passeio)->valid = -1;
        return;
    }


    //  Para cada cidade, verifica-se se a cidade segiunte do caminho é válida
    for(int i = 0; i < passeio->num_pontos - 1; i++) {
        Vector2 point1 = passeio->pontos[i];
        Vector2 point2 = passeio->pontos[i+1];

        //TODO verificar ordem (inside_board e city_valid)
        if(!(inside_board(tabuleiro, point2)) && do_points_make_L(point1, point2) && city_valid(tabuleiro, point2)) {
            //Se os dois pontos não fizerem um L entre si ou não for uma
            //  cidade válida o caminho não é válido
            passeio->cost = 0;
            passeio->valid = -1;
            return;
        }

        passeio->cost += tabuleiro_get_cost(tabuleiro, point2.x, point2.y);
    }

    passeio->valid = 1;
}

/**
 * Identifica o ponto com menor custo à distância de um salto de cavalo
 * @param vec       [description]
 * @param tabuleiro [description]
 */
void best_choice(Tabuleiro *tabuleiro, Vector2 vec){
    int best = __INT32_MAX__;

    PasseioTipoA* passeio = (PasseioTipoA*)(tabuleiro_get_passeio(tabuleiro));

    //  Caso da primeiroa cidade estar fora do mapa ou estievr fecahda
    if(!((inside_board(tabuleiro, passeio->pos_ini))  &&
            city_valid(tabuleiro, passeio->pos_ini))){
        passeio->cost = 0;
        passeio->valid = -1;
        return;
    }


    //  Percorre a o mapa
    //Note: posso fazer da outra maneira, mas o código seria gigante, dps podemos mudar
    /*for(int xx = 0; xx < tabuleiro->width ; xx++){
        for(int yy = 0; yy < tabuleiro->height; yy++){

            //  Quando encontra uma cidade numa distância igual à de um moviemnto cavalo  (3 cidades)
            if((abs(xx-vec.x) + abs (yy-vec.y)) ==3 ){

                //  caso uma coordenada seja igual, não é joagda válida,
                    //   pois num salto de cavalo, ambas as coordenadas se alteram
                if((xx == vec.x) || (yy == vec.y))  break;
                best = (best < tabuleiro_get_cost(tabuleiro, xx, yy) ? best : tabuleiro_get_cost(tabuleiro, xx, yy));
            }
        }
    }*/
    Vector2 knight_L[MOVES] = { {1,2}, {2,1}, {2,-1}, {1,-2},
                            {-1,-2}, {-2,-1}, {-2,1}, {-1,2} };
    Vector2 movement = {0,0};

    for(int i = 0; i < NUM_PONTOS_A; i++){
        for(int j = 0; j < MOVES; j++){

            movement.x = knight_L[j].x + passeio->pos_ini.x;
            movement.y = knight_L[j].y + passeio->pos_ini.y;

            if((inside_board(tabuleiro, movement) && (city_valid(tabuleiro, movement)))){
                best = (best < tabuleiro_get_cost(tabuleiro, movement.x, movement.y) ?
                            best : tabuleiro_get_cost(tabuleiro, movement.x, movement.y));
            }
        }
    }


    passeio->cost = best;
    passeio->valid = 1;
}