#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tabuleiro.h"
#include "passeios.h"

void print_usage(char* program_name) {
    printf("Usage: %s file.cities\n", program_name);
}

Tabuleiro read_file_modo_A(FILE* fp, int w, int h, char modo) {
    int num_pts_turisticos;
    Tabuleiro tabuleiro;

    fscanf(fp, "%d", &num_pts_turisticos);

    //Na primeira fase de submissão o modo A apenas deve ter um ponto
    if(num_pts_turisticos != 1) {
        printf("Erro num_pts_turisticos != 1?\n");
        exit(1);
    }

    //Cria o tabuleiro
    tabuleiro = tabuleiro_new(w, h, modo);
    tabuleiro_set_passeio(&tabuleiro, passeio_A_new(vector2_read_from_file(fp)));
    tabuleiro_read_matrix_from_file(&tabuleiro, fp);

    return tabuleiro;
}

void read_file(char* filename) {
    FILE* fp = fopen(filename, "r");

    if(fp == NULL) {
        fprintf(stderr, "Error reading file %s\n", filename);
        exit(1);
    }

    while(true) {
        //Lê cada um dos tabuleiros no ficheiro
        int w, h;
        char modo;
        Tabuleiro tabuleiro;

        //Tenta ler o tamanho do tabuleiro e modo
        if(fscanf(fp, "%d %d %c", &h, &w, &modo) != 3) {
            break; //Se não conseguiu ler mais nenhum tabuleiro para a leitura
        }

        if(modo == 'A') {
            tabuleiro = read_file_modo_A(fp, w, h, modo);
        } else {
            printf("Erro modo invalido?\n");
            break;
        }

        tabuleiro_execute(&tabuleiro);

        tabuleiro_free(&tabuleiro);
    }

    fclose(fp);
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Invalid number of arguments\n");
        print_usage(argv[0]);
        return 1;
    }

    read_file(argv[1]);

    return 0;
}
