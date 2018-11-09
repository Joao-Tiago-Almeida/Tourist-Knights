#include "util.h"

#include <stdio.h>

/**
 * [checked_malloc description]
 * @param  size [description]
 * @return      [description]
 */
void *checked_malloc(size_t size) {
    void* ptr = malloc(size);

    if(ptr == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(1);
    }

    return ptr;
}
