#include "fila.h"

#include "util.h"
#include "tabuleiro.h"

#define CONSTANTE    1.1 // 110% de um número

struct fila_t
{
    Vector2 ponto;
};

/**size = CONSTANTE * tabuleiro_get_width(tabuleiro) * tabuleiro_get_height(tabuleiro)
TODO passar para o main.c*/
/**
 * Cria uma fila prioritária
 * @param  size tamanho da fila
 * @return      fila
 */
Fila *new_fila(int size){
    Fila *new = (Fila*)checked_malloc(size);
    return new;
}

/**
 * Realloca a fila prioritária
 * @param f       ponteiros da fila
 * @param size    tamanho da fila
 */
Fila* new_size(Fila *f, int *size){
    *size = (*size) * CONSTANTE;
    return f = (Fila*) realloc(f, *size);
}

void insert_node(Fila *f){

}

/**
 * Compara dois elementos
 * @param  a elemento 1
 * @param  b elemento 2
 * @return   true se (a<b)
 */
bool feather_node(int a, int b){
    return  (a < b ? true : false); //posso fazer isto num define?
}
