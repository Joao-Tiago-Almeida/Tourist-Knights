#ifndef MOVIMENTOS_H
#define MOVIMENTOS_H

#include <stdio.h>
#include <stdbool.h>

#include "vector2.h"
#include "tabuleiro.h"  //TODO nunca sei qunado é .h ou quando é .c

bool inside_board(Tabuleiro tabuleiro, Vector2 vec);
bool valid_start(Tabuleiro tabuleiro, Vector2 vec);
void possible_moves(Tabuleiro *tabuleiro);
void best_choice(Tabuleiro *tabuleiro, Vector2 vec);

void resultados_b(Tabuleiro tabuleiro, int valido, int cost );
void resultados_a(Tabuleiro tabuleiro, int valido, int cost );

#endif
