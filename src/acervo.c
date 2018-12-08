#include "acervo.h"

#include "util.h"

#define CONSTANTE    1.1 // 110% de um número
#define NUMERO_DE_ESPACO_INICIALMENTE_ALOCADO   1

static int acervo_get_priority(Acervo* acervo, int position, Tabuleiro* tabuleiro);
static unsigned int acervo_get_upper(int v);
static void acervo_fix_up(Acervo *acervo, int p, Tabuleiro *tabuleiro);
static void acervo_fix_down(Acervo *acervo, int p, Tabuleiro *tabuleiro);
static void exchange_cities_acervo(Acervo *acervo, int p, Tabuleiro *tabuleiro);

struct acervo_t{
    Vector2* vetor;
    unsigned short size;   // tamanho do nível, posso retirar dps
    //int free;   // aponta sempre para o próximo sítio do nível onde escrever
    unsigned short num_elems;
    short* index_table;  //   matrix com o índice do acervo de cada ponto
};

//Devolve o valor da prioridade (neste caso menor é mais prioritario)
int acervo_get_priority(Acervo* acervo, int position, Tabuleiro* tabuleiro) {
    return tabuleiro_get_wt_val(tabuleiro, acervo->vetor[position]);
}

/**
 * Cria um acervo prioritária
 * @param  size tamanho da acervo
 * @return      acervo
 */
Acervo *new_acervo(int size_acervo, int size_idx_matrix) {
    // size =  NUMERO_DE_ESPACO_INICIALMENTE_ALOCADO;
    Acervo *new = (Acervo*) checked_malloc(sizeof(Acervo));
    new->vetor = (Vector2*) checked_malloc(size_acervo * sizeof(Vector2));
    new->index_table = (short*) checked_malloc(size_idx_matrix * sizeof(short));
    new->size = size_acervo;
    //new->free = 0;
    new->num_elems = 0;

    for(int i =0 ; i < size_idx_matrix; i++)
        new->index_table[i] = -1;

    return new;
}

void acervo_print(Acervo *acervo) {
    fprintf(stderr, "Acervo: %d elementos %d de capacidade\n", acervo->num_elems, acervo->size);
    if(acervo->num_elems > 0)
    {
        fprintf(stderr, "Primeiro: %d,%d\n", acervo->vetor[0].x, acervo->vetor[0].y);
            if(acervo->num_elems > 1)
        fprintf(stderr, "Filho 1: %d,%d\n", acervo->vetor[1].x, acervo->vetor[1].y);
        if(acervo->num_elems > 2)
            fprintf(stderr, "Filho 2: %d,%d\n", acervo->vetor[2].x, acervo->vetor[2].y);
    }
}

/**
 * insire uma cidade da acervo
 * @param acervo
 * @param vec coordenadas na cidade
 * @param tabuleiro
 */
void acervo_insert(Acervo *acervo, Vector2 vec, Tabuleiro *tabuleiro) {
    // realocar vetor quando está totalmente preenchido
    if(acervo->num_elems+1 > acervo->size) {
        //  aloca espaço para todo nível seguinte
        acervo->size = 2 * acervo->size + 1;
        acervo->vetor = (Vector2*) checked_realloc(acervo->vetor, acervo->size * sizeof(Vector2));
    }


    // inserir a casa na acervo
    acervo->vetor[acervo->num_elems] = vec;
    // coloca na matriz dos índices, o incide desse ponto no acervo
    acervo->index_table[vec.x + vec.y*(tabuleiro_get_width(tabuleiro))] = acervo->num_elems;
    acervo->num_elems++;
    acervo_fix_up(acervo, acervo->num_elems-1, tabuleiro);

    //(acervo->free)++;
}

/**
 * Reposiciona o elemento vec no acervo ou insere se não existir
 * @param acervo    [description]
 * @param vec       [description]
 * @param old_value [description]
 * @param tabuleiro [description]
 */
void acervo_update(Acervo* acervo, Vector2 vec, int old_value, Tabuleiro* tabuleiro) {
    int index = acervo->index_table[vec.x + vec.y*(tabuleiro_get_width(tabuleiro))];

    if(index == -1) {
        //Inserir no acervo
        acervo_insert(acervo, vec, tabuleiro);
    } else {
        //Atualizar a sua posicao
        int new_value = acervo_get_priority(acervo, index, tabuleiro);
        if(old_value == new_value) {
            //Não mudou de posição
        } else if(old_value > new_value) {
            //Se ficou mais prioritario (menor custo)
            acervo_fix_up(acervo, index, tabuleiro);
        } else {
            //Se ficou menos prioritario (maior custo)
            acervo_fix_down(acervo, index, tabuleiro);
        }
    }
}

/**
* Devolve a cidade com menor custo, //chamar a seguir a função bubble_acervo
* param acervo
* @return   primeiro elemento da acervo
*/
Vector2 acervo_get_top(Acervo *acervo) {
    return acervo->vetor[0];
}

void acervo_remove_top(Acervo *acervo, Tabuleiro* tabuleiro) {
    //  escreve na mtriz dos índice '-1', como símbolo de remoção
    Vector2 vec = acervo->vetor[0];
    acervo->index_table[vec.x + vec.y * (tabuleiro_get_width(tabuleiro))] = -1;
    acervo->vetor[0] = acervo->vetor[--acervo->num_elems];
    acervo_fix_down(acervo, 0, tabuleiro);
}

bool acervo_is_empty(Acervo* acervo) {
    return acervo->num_elems == 0;
}

/**
* Compara o custo de duas cidades
* @param  a cidade 1
* @param  b cidade 2
* @tabuleiro
* @return   cidade com menor custo
*/
int acervo_feather_city(Acervo* acervo, int a, int b, Tabuleiro *tabuleiro) {
    if(acervo_get_priority(acervo, a, tabuleiro) < acervo_get_priority(acervo, b, tabuleiro))
        return 1;

    return 2;
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
 * [acervo_fix_up description]
 * @param acervo    [description]
 * @param p         [description]
 * @param tabuleiro [description]
 */
void acervo_fix_up(Acervo *acervo, int p, Tabuleiro *tabuleiro) {
    // cidade mais leve
    if(p == 0)  return;

    // se o a cidade "filha" tiver um custo menor que a cidade "mãe"
    if(acervo_feather_city(acervo, acervo_get_upper(p), p, tabuleiro) == 2) {
        exchange_cities_acervo(acervo, p, tabuleiro);

        if(p == 0)
            return; //Chegou ao topo

        p = acervo_get_upper(p);
        acervo_fix_up(acervo, p, tabuleiro);
    }
}

/**
 * [acervo_fix_down description]
 * @param acervo    [description]
 * @param p         [description]
 * @param tabuleiro [description]
 */
void acervo_fix_down(Acervo *acervo, int p, Tabuleiro *tabuleiro) {
    int child;

    while(2*p < acervo->num_elems-1) {
        child = 2*p + 1; //primeiro filho de p

        if(child < acervo->num_elems-1) {
            //Se o pai tiver dois filhos, seleciona o com menor prioridade

            //se o child+1 tiver maior prioridade(menor custo) é selecionado o child+1
            child += acervo_feather_city(acervo, child, child+1, tabuleiro)-1;
        }

        if(acervo_feather_city(acervo, p, child, tabuleiro) == 1) {
            //Se o p for mais leve que o mais leve dos filho
            //Condicao de acervo restablecida
            break;
        }

        //Troca o pai com filho mais prioritario (mais leve)
        exchange_cities_acervo(acervo, child, tabuleiro);
        p = child;
    }
}

/**
 * Troca duas cidades no acervo
 * @param acervo [description]
 * @param p    [description]
 */
void exchange_cities_acervo(Acervo *acervo, int p, Tabuleiro *tabuleiro) {
    // troco o indice das cidades
    int width = tabuleiro_get_width(tabuleiro);
    unsigned int parent = acervo_get_upper(p);

    Vector2 parent_vec = acervo->vetor[parent];
    Vector2 vec = acervo->vetor[p];

    int a = vec.x + vec.y * width;
    int a2 = parent_vec.x + parent_vec.y * width;

    int idx_aux_pai = acervo->index_table[a2];
    acervo->index_table[a2] = acervo->index_table[a];
    acervo->index_table[a] = idx_aux_pai;

    acervo->vetor[parent] = vec;
    acervo->vetor[p] = parent_vec;
}

void acervo_free(Acervo** acervo) {
    free((*acervo)->index_table);
    free((*acervo)->vetor);
    free(*acervo);
    *acervo = NULL;
}

void acervo_init(Acervo *acervo, Tabuleiro* tabuleiro) {
    for(int i = 0; i<acervo->num_elems; i++) {
        Vector2 vec = acervo->vetor[i];
        acervo->index_table[vec.x + vec.y*(tabuleiro_get_width(tabuleiro))] = -1;
    }
    for(unsigned int i = 0; i<tabuleiro_get_width(tabuleiro); i++) {
        for(unsigned int j = 0; j<tabuleiro_get_height(tabuleiro); j++) {
            if(acervo->index_table[i + j*(tabuleiro_get_width(tabuleiro))] != -1) {
                printf("Acervo idnex table não totalmente limpo bro\n");
            }
        }
    }
    acervo->num_elems = 0;
}
