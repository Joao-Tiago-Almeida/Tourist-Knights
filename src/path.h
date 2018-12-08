#ifndef PATH_H
#define PATH_H

#include "vector2.h"

typedef struct path_t {
    unsigned short length;
    unsigned short cost;
    Vector2* points;
    Vector2 orig, dest;
} Path;

#endif
