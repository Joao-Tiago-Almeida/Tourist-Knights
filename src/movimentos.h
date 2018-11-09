#ifndef MOVIMENTOS_H
#define MOVIMENTOS_H

#include <stdio.h>
#include <stdbool.h>

#include "vector2.h"
#include "tabuleiro.h"  //TODO nunca sei qunado é .h ou quando é .c

bool inside_board(Vector2 vec, Tabuleiro tabuleiro);
void possible_moves(Tabuleiro tabuleiro);

#endif
