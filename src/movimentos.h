#ifndef MOVIMENTOS_H
#define MOVIMENTOS_H

#include <stdio.h>
#include <stdbool.h>

#include "vector2.h"
#include "tabuleiro.h"
#include "path.h"

bool inside_board(Tabuleiro *tabuleiro, Vector2 vec);
bool city_valid(Tabuleiro *tabuleiro, Vector2 vec);
bool check_if_city_accessible(Tabuleiro* tabuleiro, Vector2 c);
bool do_points_make_L(Vector2 point1, Vector2 point2);

Path inverte_caminho(Tabuleiro* tabuleiro, Path path);

Path* movimentos_find_path_to_multiple_cities(Tabuleiro* tabuleiro, Vector2 ini, Vector2* destinos, int num_destinos);
Path movimentos_find_path_to_city(Tabuleiro* tabuleiro, Vector2 ini, Vector2 dest);

#endif
