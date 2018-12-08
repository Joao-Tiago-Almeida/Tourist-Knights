#include "util.h"

#include <stdio.h>
#include <string.h>

#define DOT_WALKS_LENGTH 5
#define DOT_CITIES_LENGHT 7

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

/**
 * Alocação limitada de memória
 * @param  size tamanho finito de memória a alocar
 * @return      ponteiro para o espaço de memória alocado
 */
void *checked_realloc(void * ptr, size_t size) {
    #ifdef DEBUG
    printf("Realloc!!\n");
    #endif
    ptr = realloc(ptr, size);

    if(ptr == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(0);
    }

    return ptr;
}

/**
 * Alocação e inicialização limitada de memória
 * @param  size tamanho finito de memória a alocar
 * @param  n    contante a multipliciar
 * @return      ponteiro para o espaço de memória alocado
 */
void *checked_calloc(size_t size, size_t n) {
    void* ptr = calloc(size, n);

    if(ptr == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(0);
    }

    return ptr;
}

/**
 * Verifica se a string str acaba de forma igual à string test
 * @param  str  [description]
 * @param  test [description]
 * @return      [description]
 */
bool string_ends_with(char* str, char* test) {
    int len = strlen(str);
    int testlen = strlen(test);

    if(len < testlen) {
        return false;
    }

    return (strcmp(str+(len-testlen), test) == 0);


}

/**
 * Cria a extensão do fihceiro de saída
 * @param  filename nome do ficheiro de entrada
 * @return          nome do ficheiro de saída
 */
char* create_dot_valid_filename(char* filename) {
    size_t filename_size = strlen(filename);
    char* file_out_name = checked_calloc(sizeof(char), (filename_size + DOT_WALKS_LENGTH + 1));
    memcpy(file_out_name, filename, sizeof(char)*(filename_size - DOT_CITIES_LENGHT));
    strcat(file_out_name, ".walks\0");
    return file_out_name;
}
