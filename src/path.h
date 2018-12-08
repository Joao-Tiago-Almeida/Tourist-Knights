#ifndef PATH_H
#define PATH_H

#include "vector2.h"

typedef struct path_t {
    unsigned short length;
    unsigned short cost;
    Vector2* points;
    Vector2 orig, dest;
} Path;

Path path_new(Vector2 orig, Vector2 dest);
void path_set_single_point(Path* path, unsigned short cost, Vector2 single_point);

#endif
