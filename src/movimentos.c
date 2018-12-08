#include "movimentos.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "acervo.h"

#define NUM_PONTOS_A 1

#define MOVES 8

//  vetor que guarda as coordenadas dos oito movimentos possiveis
const Vector2 knight_L[MOVES] = { {-1,2}, {1,2}, {2,1}, {2,-1}, {1,-2},
                        {-1,-2}, {-2,-1}, {-2,1} };

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
        return false;
    }
    //  Ver se o ponto está entre os eixos horizointais do tabuleiro
    else if(vec.y < 0 || vec.y > (int)tabuleiro_get_height(tabuleiro)-1){
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

bool check_if_city_accessible(Tabuleiro* tabuleiro, Vector2 c) {
    for(int mov_rel = 0; mov_rel<8; mov_rel++) {
        //Vizinhos de c
        Vector2 pos_to_try = vector2_add(c, knight_L[mov_rel]);
        if(inside_board(tabuleiro, pos_to_try) && tabuleiro_get_cost(tabuleiro, pos_to_try) != 0)
            return true;
    }

    return false;
}
/*
 * Devolve true se dois pontos fizerem um L entre si
 */
bool do_points_make_L(Vector2 point1, Vector2 point2) {
    return (abs(point1.x-point2.x) == 2 && abs(point1.y-point2.y) == 1) ||
        (abs(point1.x-point2.x) == 1 && abs(point1.y-point2.y) == 2);
}

//Testa se há caminhos em que para encontrar não é preciso o dijsktra
// 
static void dijkstra_test_easy_paths(Tabuleiro* tabuleiro, Vector2 ini, Vector2* destinos, int num_destinos,
                                        bool *path_found, int *num_paths_left) {
    for(int i = 0; i<num_destinos; i++) {
        if(vector2_equals(ini, destinos[i])) {
            path_found[i] = true;
            num_paths_left--;
        }
        if(do_points_make_L(ini, destinos[i])) {
            path_found[i] = true;
            (*num_paths_left)--;
            for(int mov_rel = 0; mov_rel<8; mov_rel++) {
                if(vector2_equals(vector2_add(ini, knight_L[mov_rel]), destinos[i])) {
                    tabuleiro_set_wt_val(tabuleiro, destinos[i], tabuleiro_get_cost(tabuleiro, destinos[i]));
                    tabuleiro_set_st_val(tabuleiro, destinos[i], mov_rel);
                }
            }       
        }
    }
}

//Executa o algoritmo dijkstra, determinando os valores das matrizes wt e st
//Para quando for encontrado o caminho para cada um dos destinos, ou quando isso não for possivel
//Devolve true ou false consoante foram encontrados os caminhos com sucesso
static bool dijkstra_geral(Tabuleiro* tabuleiro, Vector2 ini, Vector2* destinos, int num_destinos) {
    bool *path_found = checked_calloc(sizeof(bool), num_destinos);
    //Numero de caminhos ainda por determinar
    int num_paths_left = num_destinos;

    //Init matrizes (inicializa-se tudo a -1, (wt -- representa infinito; st -- representa que não tem ajdência))
    tabuleiro_init_st_wt(tabuleiro);
    acervo_init((Acervo *)tabuleiro_get_fila(tabuleiro), tabuleiro);


    //Verifica se não precisa de calcular o caminho para algum dos destinos
    dijkstra_test_easy_paths(tabuleiro, ini, destinos, num_destinos, path_found, &num_paths_left);

    if(num_paths_left != 0) {
        //Aidna precisa de fazer o dijkstra

        //Init algoritmo
        tabuleiro_set_wt_val(tabuleiro, ini, 0);
        acervo_insert((Acervo *)tabuleiro_get_fila(tabuleiro), ini, tabuleiro);

        while(!acervo_is_empty((Acervo *)tabuleiro_get_fila(tabuleiro))) {
            Vector2 v = acervo_get_top((Acervo *)tabuleiro_get_fila(tabuleiro));

            for(int i = 0; i<num_destinos; i++) {
                if(!path_found[i] && vector2_equals(v, destinos[i])) {
                    //Encontrou um caminho para o destino i
                    path_found[i] = true;
                    num_paths_left--;

                }
            }
            if(num_destinos == 0) {
                //Todos os caminhos foram encontrados
                break;
            }

            acervo_remove_top((Acervo *)tabuleiro_get_fila(tabuleiro), tabuleiro);

            int wt_value = tabuleiro_get_wt_val(tabuleiro, v);

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
                        acervo_update((Acervo *)tabuleiro_get_fila(tabuleiro), pos_to_try, old_wt_val, tabuleiro);

                        //Fica ao contrario (em vez de dizer que subiu 2, tá a dizer que desceu dois)
                        //Pq o st é suposto dizer a direção para ir do pos_to_try ao v, e não o contrario
                        tabuleiro_set_st_val(tabuleiro, pos_to_try, mov_rel);
                    }
                } else {
                    //Se for a primeira vez que a casa é considerada
                    tabuleiro_set_wt_val(tabuleiro, pos_to_try, new_wt_val);
                    acervo_insert((Acervo *)tabuleiro_get_fila(tabuleiro), pos_to_try, tabuleiro);
                    tabuleiro_set_st_val(tabuleiro, pos_to_try, mov_rel);
                }
            }
        }
    }


    //Virifica se não há caminho para um dos pontos de destino, define o problema como invalido
    for(int i = 0; i<num_destinos; i++) {
        if(tabuleiro_get_st_val(tabuleiro, destinos[i]) == -1) {
            tabuleiro_set_valid(tabuleiro, false);
            free(path_found);
            return false;
        }
    }
    free(path_found);

    return true;
}

static void get_path_from_dijkstra(Tabuleiro* tabuleiro, Path* path) {
    //Percorrer o caminho ao contrario para saber o tamanho necessario para o vetor
    Vector2 p = (*path).dest;
    int i = 0;
    int st;
    (*path).cost = 0;

    
    if(vector2_equals((*path).orig, (*path).dest)) {
        //Não aloca memória para um caminho que é de um ponto para sí próprio
        return;
    }
    
    //Percorre o st até à origem para saber o numero de pontos a alocar
    while(true) {
        st = tabuleiro_get_st_val(tabuleiro, p);
        if(st == -1)
            break; //Se chegou ao fim

        p = vector2_sub(p, knight_L[st]);

        (*path).length++;
    }

    //Percorre o st até à origem para escrever os pontos de passagem no path
    (*path).points = (Vector2*)checked_malloc(sizeof(Vector2) * (*path).length);
    p = (*path).dest;
    while(true) {
        st = tabuleiro_get_st_val(tabuleiro, p);

        if(st == -1)
            break; //Se chegou ao fim

        //Copia os pontos de passagem para o path
        (*path).points[((*path).length-1) - i] = p; //Esreve no vetor do inicio para o fim
        (*path).cost += tabuleiro_get_cost(tabuleiro, p);

        //P é o próximo ponto a analisar
        p = vector2_sub(p, knight_L[st]);
        i++;
    }
}

Path inverte_caminho(Tabuleiro* tabuleiro, Path path) {
    Path new_path;

    new_path.length = path.length;
    new_path.dest = path.orig;
    new_path.orig = path.dest;
    new_path.cost = path.cost - tabuleiro_get_cost(tabuleiro, path.dest) + tabuleiro_get_cost(tabuleiro, new_path.dest);
    new_path.points = (Vector2*) checked_malloc(sizeof(Vector2) * new_path.length);
    for(int i = 0; i<new_path.length-1; i++) {
        new_path.points[new_path.length-1 - i -1] = path.points[i];
    }
    new_path.points[new_path.length-1] = new_path.dest;


    return new_path;
}

//Devolve true se um caminho igual ainda não foi calculado, nem um inverso
// Escreve no path o caminho a ser usado caso não seja necessário fazer o dijkstra
// O i indica o numero de caminhos já calculados
static bool path_has_to_be_calculated(Tabuleiro* tabuleiro, Vector2 orig, Vector2 dest, Path* path) {
    for(int j = 0; j<tabuleiro_get_num_path_calculated(tabuleiro); j++) {
        Path path_j = tabuleiro_get_path(tabuleiro, j);
        if(vector2_equals(path_j.orig, orig) && vector2_equals(path_j.dest, dest)) {
            //Se o caminho já foi calculado basta copia-lo
            // (A para B já foi calculado, e é para fazer de novo de A para B)
            *path = path_j;
            (*path).points = (Vector2*) checked_malloc(sizeof(Vector2) * (*path).length);
            memcpy((*path).points, path_j.points, sizeof(Vector2) * (*path).length);
            #ifdef DEBUG
            printf("copia\n");
            #endif
            return false;
        } else if(vector2_equals(path_j.orig, dest) && vector2_equals(path_j.dest, orig)) {
            //Se o caminho já foi calculado ao contrario basta inverte-lo
            // (A para B já foi calculado, e é para fazer B para A)
            *path = inverte_caminho(tabuleiro, path_j);
            #ifdef DEBUG
            printf("inverte\n");
            #endif
            return false;
        }
    }
    return true;
}

/**
 * Devolve o caminho mais curto entre dois pontos
 * @param  tabuleiro
 * @param  ini          Ponto de inicio
 * @param  dest         Ponto de destino
 * @return              Caminho
 * Efeitos: Se não conseguiu encontrar um caminhos torna tabuleiro invalido(valid = false)
 */
Path movimentos_find_path_to_city(Tabuleiro* tabuleiro, Vector2 ini, Vector2 dest) {
    Path path;
    //Escreve o path caso não seja preciso fazer o dijkstra
    bool do_dijkstra = path_has_to_be_calculated(tabuleiro, ini, dest, &path);
    if(!do_dijkstra)
        return path;

    //Se não foi encontrado um caminho igual no tabuleiro->paths, executa o dijkstra
    path = path_new(ini, dest);

    if(vector2_equals(ini, dest))
        return path;

    if(do_points_make_L(ini, dest)) {
        path_set_single_point(&path, tabuleiro_get_cost(tabuleiro, dest), dest);
        return path;
    }

    //Executa o dijkstra apena com o destino dest
    if(!dijkstra_geral(tabuleiro, ini, &dest, 1)) {
        //Se o dijkstra não encontrou um caminho para o destino sai da função
        return path;
    }

    //Escreve o caminho no path
    get_path_from_dijkstra(tabuleiro, &path);
    return path;
}

/**
 * Devolve os caminho mais curtos entre o ponto inicial e os vários destinos
 * @param  tabuleiro
 * @param  ini          Ponto de inicio
 * @param  destinos     Vetor com as coordenadas de cada destino
 * @param  num_destinos Tamanho do vetor destinos
 * @return              Vetor com os caminhos correspondentes ao destinos        
 *                        NULL se não conseguiu encontrar caminhos 
 * Efeitos: Se não conseguiu encontrar um dos caminhos torna tabuleiro invalido(valid = false)
 */
Path* movimentos_find_path_to_multiple_cities(Tabuleiro* tabuleiro, Vector2 ini, Vector2* destinos, int num_destinos) {
    Path *paths = checked_calloc(sizeof(Path), num_destinos);
    bool *path_found = checked_calloc(sizeof(bool), num_destinos);
    //Numero de caminhos ainda por determinar
    int num_paths_left = num_destinos;

    //Inicializa os caminhos para cada ponto
    for(int i = 0; i<num_destinos; i++) {
        paths[i] = path_new(ini, destinos[i]);

        //Se não precisar de calcular o caminho
        if(vector2_equals(ini, destinos[i])) {
            path_found[i] = true;
            num_paths_left--;
        }
        if(do_points_make_L(ini, destinos[i])) {
            path_set_single_point(&paths[i], tabuleiro_get_cost(tabuleiro, destinos[i]), destinos[i]);
            num_paths_left--;
            path_found[i] = true;
        }
    }

    if(!dijkstra_geral(tabuleiro, ini, destinos, num_destinos)) {
        //Se o dijkstra não encontrou um caminho para cada destino sai da função
        free(path_found);
        free(paths);
        return NULL;
    }

    //Para cada destino escreve os pontos de passagem do caminho correspondente
    for(int path_i = 0; path_i<num_destinos; path_i++) {
        //Percorrer o caminho ao contrario para saber o tamanho necessario para o vetor
        get_path_from_dijkstra(tabuleiro, &paths[path_i]);
    }

    free(path_found);
    return paths;
}