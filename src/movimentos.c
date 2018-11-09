#include "movimentos.h"

#include <stdio.h>
#include <stdbool.h>

#include "vector2.h"

/**
 * [inside_board description]
 * @param  vec [description]
 * @param  h   [description]
 * @param  w   [description]
 * @return     [description]
 */
bool inside_board(Vector2 vec, unsigned int h, unsigned int w){

    if(vec.x < 0 || vec.x > w-1){
        fprintf(stderr, "\npoint out of the board\n");
        return false;
    }
    else if(vec.y < 0 || vec.y > h-1){
        fprintf(stderr, "\npoint out of the board\n");
        return false;
    }
    return true;
}

void possible_moves()
