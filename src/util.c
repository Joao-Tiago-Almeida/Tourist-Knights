#include "util.h"

#include <stdio.h>

/**
 * Alocação limitada de memória
 * @param  size tamanho finito de memória a alocar
 * @return      ponteiro para o espaço de memória alocado
 */
void *checked_malloc(size_t size) {
    void* ptr = malloc(size);

    if(ptr == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(0);
    }

    return ptr;
}
