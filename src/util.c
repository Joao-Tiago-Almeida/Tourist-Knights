#include "util.h"

#include <stdio.h>
#include <string.h>

/**
 * Alocação limitada de memória
 * @param  size tamanho finito de memória a alocar
 * @return      ponteiro para o espaço de memória alocado
 */
void *checked_malloc(size_t size) {
    void* ptr = malloc(size);

    if(ptr == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(1);
    }

    return ptr;
}

//TODO comentario bonito
//Verifica se a string str acaba de forma igual à string test
bool string_ends_with(char* str, char* test) {
    char* pos = strstr(str, test);

    if(pos == NULL) {
        return false;
    }

    if(str + (strlen(pos) - strlen(test)) == pos) {
        return true;
    }
    return false;
}