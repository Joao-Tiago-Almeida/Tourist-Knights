#ifndef PATH_H
#define PATH_H

#include "vector2.h"

typedef struct path_t {
    int length;
    int cost;
    Vector2* points;
} Path;

#endif
