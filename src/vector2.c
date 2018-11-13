#include "vector2.h"

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
    fscanf(fp, "%d %d", &vec.y, &vec.x);

    return vec;
}
