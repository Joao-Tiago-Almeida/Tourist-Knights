#include "vector2.h"

/**
 * [vector2_new description]
 * @param  x [description]
 * @param  y [description]
 * @return   [description]
 */
Vector2 vector2_new(int x, int y) {
    Vector2 vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

/**
 * [vector2_read_from_file description]
 * @param  fp [description]
 * @return    [description]
 */
Vector2 vector2_read_from_file(FILE* fp) {
    Vector2 vec;

    fscanf(fp, "%d %d", &vec.x, &vec.y);

    return vec;
}
