#include "util.h"

#include <stdio.h>

void *checked_malloc(size_t size) {
    void* ptr = malloc(size);

    if(ptr == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(1);
    }

    return ptr;
}