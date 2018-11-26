#include "vector2.h"

#include <stdlib.h>

/**
 * Cria um Vector de 2 dimensões
 * @param  x coordenada relativa ao eixo dos xx
 * @param  y coordenada relativa ao eixo dos yy
 * @return   vetor 2D criado
 */
Vector2 vector2_new(int x, int y) {
    Vector2 vec;
    vec.x = x;
    vec.y = y;
    return vec;
}

/**
 * Lê um vetor do ficehiro
 * @param  fp ficehiro de entrada
 * @return    vertor lido
 */
Vector2 vector2_read_from_file(FILE* fp) {
    Vector2 vec;

    //No formato do ficheiro o Y vem primeiro que o X
    if(fscanf(fp, "%d %d", &vec.y, &vec.x) != 2) {
        fprintf(stderr, "Erro de leitura");
        exit(0);
    }

    return vec;
}

bool vector2_equals(Vector2 a, Vector2 b) {
    return a.x == b.x && a.y == b.y;
}

Vector2 vector2_add(Vector2 a, Vector2 b) {
    Vector2 res;
    res.x = a.x + b.x;
    res.y = a.y + b.y;
    return res;
}