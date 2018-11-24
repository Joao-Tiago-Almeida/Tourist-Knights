#include "acervo.h"

#include "util.h"

#define CONSTANTE    1.1 // 110% de um número

struct acervo_t{
    Vector2* vetor;
    int size;   // tamanho do nível, posso retirar dps
    int free;   // aponta sempre para o próximo sítio do nível onde escrever
};

/**size = CONSTANTE * tabuleiro_get_width(tabuleiro) * tabuleiro_get_height(tabuleiro)
TODO passar para o main.c*/
/**
 * Cria uma acervo prioritária
 * @param  size tamanho da acervo
 * @return      acervo
 */
Acervo *new_acervo(int size) {
    Acervo *new = (Acervo*)checked_malloc(size);
    new->size = size;
    return new;
}

/**
 * Realloca a acervo prioritária
 * @param acervo       ponteiro da acervo
 */
Acervo *new_size(Acervo *acervo) {

    // qunado passa para p níel seguinte, alloca memória para todo o nível
    acervo->size = (acervo->size ) * 2;
    // reinicia a poisção a escrever no nível
    acervo->free = 0;
    return acervo = (Acervo*) realloc(acervo, acervo->size);
}

/**
 * insire uma cidade da acervo
 * @param acervo
 * @param vec coordenadas na cidade
 */
void insert_city(Acervo *acervo, Vector2 vec) {

    // realocar vetor quando está totalmente preenchido
    if((acervo->free + 1) > acervo->size){
        new_size(acervo);
    }
    // inserir a casa na acervo
    acervo->vetor[get_insert_pos(acervo)] = vec;
    (acervo->free)++;
}

/**
 * Devolve a cidade com menor custo
 * param acervo
 * @return   primeiro elemento da acervo
 */
Vector2 pick_best_city(Acervo *acervo) {
    return acervo->vetor[0];
}

/**
 * Compara o custo de duas cidades
 * @param  a cidade 1
 * @param  b cidade 2
 * @tabuleiro
 * @return   cidade com menor custo
 */
int feather_city(Vector2 a, Vector2 b, Tabuleiro *tabuleiro) {
    if(tabuleiro_get_cost(tabuleiro, b) < tabuleiro_get_cost(tabuleiro, a))
        return 2;
    return 1;
}

/**
 * Procura o "pai"
 * @param  v cidade
 * @return   a cidade acima no acervo
 */
unsigned int acervo_get_upper(int v) {
    return (v -1)/2;
}

/**
 * Procura o "filho" com menor peso
 * @param  acervo
 * @param  v    cidade
 * @param  n    qual o "filho a procurar" {1,2}
 * @return      a cidade abaixo no acervo
 */
int acervo_get_lower(Acervo *acervo, int v, int n) {

    int a;
    // se a posição do filho for maior que a próxima posição de escrita no acervo
    if((a = 2*v + n) > get_insert_pos(acervo)) {
        // não há filho
        return -1;
    }
    return a;
}

void bubble_down_acervo(Acervo *acervo, int p, Tabuleiro *tabuleiro) {

    int feather;
    // a cidade não tem filhos
    if((feather = acervo_get_lower(acervo, p, 1)) == -1) {
    // a cidade só tem um filho
    }else if(acervo_get_lower(acervo, p, 2) == -1) {
        acervo->vetor[p] = acervo->vetor[feather];
    // a cidade tem dois filhos
    }else {
        // Procura do "filho" com menor custo
        feather = acervo_get_lower(acervo, p, feather_city(acervo->vetor[acervo_get_lower(acervo, p, 1)], acervo->vetor[acervo_get_lower(acervo, p, 2)], tabuleiro));
        // bubble
        acervo->vetor[p] = acervo->vetor[feather];
        bubble_down_acervo(acervo, feather, tabuleiro);
    }
    // escreve o último elemento da acervo no filho que subiu e retrocede a próxima posição de escrita
    if(feather != -1) {
        acervo->vetor[feather] = acervo->vetor[--(acervo->free)];
    }
}

void bubble_up_acervo(Acervo *acervo, int p, Tabuleiro *tabuleiro) {
    if(feather_city(acervo->vetor[acervo_get_upper(p)], acervo->vetor[p], tabuleiro) == 2) {
        switch_cities_acervo(acervo, p);
    }
}

/**
 * Calcula a próxima posição vazia
 * @param  acervo
 * @return   posição vazia
 */
int get_insert_pos(Acervo *acervo) {
    int a = 0, b;
    do {
        b = (acervo->size)/2;
        a += b;
    } while( b != 1 );
    return  a + acervo->free;
}

/**
 * Troca duas cidades no acervo
 * @param acervo [description]
 * @param p    [description]
 */
void switch_cities_acervo(Acervo *acervo, int p) {

    Vector2 vec = acervo->vetor[acervo_get_upper(p)];
    acervo->vetor[acervo_get_upper(p)] = acervo->vetor[p];
    acervo->vetor[p] = vec;
}
