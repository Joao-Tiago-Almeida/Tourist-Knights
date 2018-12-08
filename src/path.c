#include "path.h"
#include "util.h"

Path path_new(Vector2 orig, Vector2 dest) {
    Path path;
    path.length = 0;
    path.cost = 0;
    path.points = NULL;
    path.orig = orig;
    path.dest = dest;
    return path;
}

void path_set_single_point(Path* path, unsigned short cost, Vector2 single_point) {
    (*path).length = 1;
    (*path).cost = cost;
    (*path).points = (Vector2*)checked_malloc(sizeof(Vector2) * 1);
    (*path).points[0] = single_point;
}