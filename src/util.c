#include "util.h"

#include <stdio.h>
#include <string.h>

#define DOT_VALID_LENGTH 6
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

void *checked_calloc(size_t size, size_t n) {
    void* ptr = calloc(size, n);

    if(ptr == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(0);
    }

    return ptr;
}

//TODO comentario bonito
//Verifica se a string str acaba de forma igual à string test
bool string_ends_with(char* str, char* test) {
    int len = strlen(str);
    int testlen = strlen(test);

    if(len < testlen) {
        return false;
    }

    return (strcmp(str+(len-testlen), test) == 0);

    /*char* pos = strstr(str, test); //apontador para o inicio do test no str

    if(pos == NULL) {
        return false;
    }

    if(str + (strlen(str) - strlen(test)) == pos) {
        return true;
    }
    return false;*/
}

char* create_dot_valid_filename(char* filename) {
    size_t filename_size = strlen(filename);
    char* file_out_name = checked_calloc(sizeof(char), (filename_size + DOT_VALID_LENGTH + 1));
    memcpy(file_out_name, filename, sizeof(char)*(filename_size - DOT_CITIES_LENGHT));
    strcat(file_out_name, ".valid\0");
    return file_out_name;
}
