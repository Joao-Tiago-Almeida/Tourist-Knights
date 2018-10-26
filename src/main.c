#include <stdio.h>

#include "tabuleiro.h"

int main() {
    Tabuleiro tabuleiro = tabuleiro_new(1, 2, 'A');

    tabuleiro_free(&tabuleiro);
}