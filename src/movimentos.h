#ifndef MOVIMENTOS_H
#define MOVIMENTOS_H

#include <stdio.h>
#include <stdbool.h>

#include "vector2.h"
#include "tabuleiro.h"
#include "path.h"

bool inside_board(Tabuleiro *tabuleiro, Vector2 vec);
bool city_valid(Tabuleiro *tabuleiro, Vector2 vec);
void possible_moves(Tabuleiro *tabuleiro);
void best_choice(Tabuleiro *tabuleiro);
Path movimentos_find_path(Tabuleiro* tabuleiro, Vector2 ini, Vector2 dest);

#endif
