#include "vector2.h"

Vector2 vector2_new(int x, int y) {
    Vector2 vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

Vector2 vector2_read_from_file(FILE* fp) {
    Vector2 vec;

    fscanf(fp, "%d %d", &vec.x, &vec.y);

    return vec;
}