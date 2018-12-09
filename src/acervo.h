#ifndef ACERVO_H
#define ACERVO_H

#include <stdbool.h>

#include "vector2.h"
#include "tabuleiro.h"

typedef struct acervo_t Acervo;

Acervo *new_acervo(int size_acervo, int size_idx_matrix);
Vector2 acervo_get_top(Acervo *acervo);
void acervo_insert(Acervo *f, Vector2 vec, Tabuleiro *tabuleiro);
void acervo_update(Acervo* acervo, Vector2 vec, int old_value, Tabuleiro* tabuleiro);
bool acervo_is_empty(Acervo* acervo);
void acervo_remove_top(Acervo *acervo, Tabuleiro* tabuleiro);
void acervo_print(Acervo *acervo);
void acervo_free(Acervo** acervo);
void acervo_init(Acervo *acervo, Tabuleiro* tabuleiro);

#endif
