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
bool inside_board(Vector2 vec, Tabuleiro tabuleiro){

    //  Ver se o ponto está entre os eixos verticais do tabuleiro
    if(vec.x < 0 || vec.x > tabuleiro.width-1){

        //fprintf(stderr, "\npoint out of the board\n");
        return false;
    }
    //  Ver se o ponto está entre os eixos horizointais do tabuleiro
    else if(vec.y < 0 || vec.y > tabuleiro.height-1){

        //fprintf(stderr, "\npoint out of the board\n");
        return false;
    }
    return true;
}

//TODO
/*
provavelmente a função irá receber dois pontos e confirmar se estão à distância do cavalo
    mas para a parte um podemos mandar o caminho e ela vê se o caminho é válido ou não
 */
/**
 * Analisa um caminho vê se é válido e calcula o custo desse caminho
 * @param tabuleiro [description]
 */
void possible_moves(Tabuleiro *tabuleiro){

    Vector2 knight_L[MOVES] = { {1,2}, {2,1}, {2,-1}, {1,-2},
                            {-1,-2}, {-2,-1}, {-2,1}, {-1,2} };
    Vector2 movement[MOVES] = {{0}};

    // NOTA: a cidade de partida não tem custo
    ((PasseioTipoB*)tabuleiro->passeio)->cost = 0;

    if(tabuleiro_get_cost(tabuleiro, ((PasseioTipoB*)tabuleiro->passeio)->pontos[0].x,
                                    ((PasseioTipoB*)tabuleiro->passeio)->pontos[0].y) == 0){
        resultados_b( *tabuleiro, -1, ((PasseioTipoB*)tabuleiro->passeio)->cost );
        return;
    }


    //  Para cada cidade, verifica-se se a cidade segiunte do caminho é válida
    for(int i = 0; i < ((PasseioTipoB*)tabuleiro->passeio)->num_pontos - 1; i++)
    {


        for(int j = 0; j < MOVES; j++){

            //  Caso a coordenada xx da próxima cidade seja válida
            movement[j].x = knight_L[j].x + ((PasseioTipoB*)tabuleiro->passeio)->pontos[i].x;
            if(movement[j].x == ((PasseioTipoB*)tabuleiro->passeio)->pontos[i+1].x){

                //  Caso a coordenada yy da próxima cidade seja válida
                movement[j].y = knight_L[j].y + ((PasseioTipoB*)tabuleiro->passeio)->pontos[i].y;
                if(movement[j].y == ((PasseioTipoB*)tabuleiro->passeio)->pontos[i+1].y)
                {
                    //  Caso o próximo a próxima cidade esteja fechada
                    if(tabuleiro_get_cost(tabuleiro, movement[j].x,movement[j].y) == 0){
                        ((PasseioTipoB*)tabuleiro->passeio)->cost = 0;
                        resultados_b( *tabuleiro, -1, ((PasseioTipoB*)tabuleiro->passeio)->cost );
                        return;
                    }

                    //  Caso a cidade seguinte reúna condições para ser visita: Let´s Go!
                    ((PasseioTipoB*)tabuleiro->passeio)->cost += tabuleiro_get_cost(tabuleiro, movement[j].x,
                                                                                            movement[j].y);
                    break;
                }
            }
            //  Caso em que o movimento do turista não foi em L
            if(j + 1 == MOVES){
                resultados_b( *tabuleiro, -1, ((PasseioTipoB*)tabuleiro->passeio)->cost );
                return;
            }
        }
    }
    //REMOVE-ME, função com as soluções
    resultados_b( *tabuleiro, 1, ((PasseioTipoB*)tabuleiro->passeio)->cost );
}

void resultados_b(Tabuleiro tabuleiro, int valido, int cost ){
    printf("%d %d %c %d %d %d\n\n", tabuleiro.height, tabuleiro.width,
                                tabuleiro.type_passeio, ((PasseioTipoB*)tabuleiro.passeio)->num_pontos,
                                valido, cost);
}

/**
 * Identifica o ponto com menor custo à distância de um salto de cavalo
 * @param vec       [description]
 * @param tabuleiro [description]
 */
void best_choice(Vector2 vec, Tabuleiro tabuleiro){
    int best = __INT32_MAX__;

    //  Caso do ponto inicial estar fora do mapa
    if(!(inside_board(((PasseioTipoA*)tabuleiro.passeio)->pos_ini, tabuleiro))){
        resultados_a( tabuleiro, -1, best );
        return;
    }

    //  Percorre a o mapa
    for(int xx = 0; xx < tabuleiro.width ; xx++){
        for(int yy = 0; yy < tabuleiro.height; yy++){

            //  Quando encontra uma cidade numa distância igual à de um moviemnto cavalo  (3 cidades)
            if((abs(xx-vec.x) + abs (yy-vec.y)) ==3 ){

                //  caso uma coordenada seja igual, não é joagda válida,
                    //   pois num salto de cavalo, ambas as coordenadas se alteram
                if((xx == vec.x) || (yy == vec.y))  break;
                best = (best < tabuleiro_get_cost(&tabuleiro, xx, yy) ? best : tabuleiro_get_cost(&tabuleiro, xx, yy));
            }
        }
    }
    //REMOVE-ME, função com as soluções
    resultados_a( tabuleiro, 1, best);
}

void resultados_a(Tabuleiro tabuleiro, int valido, int cost ){
    if(cost == __INT32_MAX__)  cost = 0, valido = -1;
    printf("%d %d %c %d %d %d\n\n", tabuleiro.height, tabuleiro.width,
                                tabuleiro.type_passeio, NUM_PONTOS_A,
                                valido, cost);
}
