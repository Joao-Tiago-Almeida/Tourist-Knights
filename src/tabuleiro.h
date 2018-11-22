#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdio.h>
#include <stdbool.h>

#include "vector2.h"

typedef struct tabuleiro_t Tabuleiro;

Tabuleiro* tabuleiro_new(unsigned int w, unsigned int h, char type_passeio);
void tabuleiro_read_matrix_from_file(Tabuleiro* tabuleiro, FILE* fp);
void tabuleiro_read_matrix_from_file_invalid(Tabuleiro* tabuleiro, FILE* fp);

unsigned int tabuleiro_get_width(Tabuleiro* tabuleiro);
unsigned int tabuleiro_get_height(Tabuleiro* tabuleiro);

//FIXME mudar para recber um Vector2 em vez de dois pontos
void tabuleiro_set_cost(Tabuleiro* tabuleiro, unsigned int x, unsigned int y, unsigned char cost);
int tabuleiro_get_cost(Tabuleiro* tabuleiro, unsigned int x, unsigned int y);

char tabuleiro_get_tipo_passeio(Tabuleiro* tabuleiro);

void tabuleiro_write_valid_file(Tabuleiro *tabuleiro, FILE* fp);

void tabuleiro_execute(Tabuleiro *tabuleiro);

void tabuleiro_free(Tabuleiro* tabuleiro);

void print_tabuleiro(Tabuleiro* tabuleiro, int w, int h);


void tabuleiro_read_passeio_from_file(Tabuleiro* tabuleiro, int num_pontos, FILE* fp);

void tabuleiro_set_valid(Tabuleiro* tabuleiro, char valid);
char passeio_get_valid(Tabuleiro* tabuleiro);

void tabuleiro_passeio_set_cost(Tabuleiro* tabuleiro, int cost);
int tabuleiro_passeio_get_cost(Tabuleiro* tabuleiro);

int tabuleiro_get_num_pontos(Tabuleiro* tabuleiro);

Vector2* tabuleiro_passeio_get_pontos(Tabuleiro* tabuleiro);
Vector2 tabuleiro_passeio_get_pos_ini(Tabuleiro* tabuleiro);

#endif
