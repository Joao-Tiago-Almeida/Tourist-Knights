#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tabuleiro.h"
#include "passeios.h"

void print_usage(char* program_name) {
    printf("Usage: %s file.cities\n", program_name);
}

Tabuleiro* read_file_modo_A(FILE* fp, int w, int h, char modo, bool* do_not_execute) {
    int num_pts_turisticos;
    Tabuleiro* tabuleiro;

    fscanf(fp, "%d", &num_pts_turisticos);

    if(num_pts_turisticos == 1 ){
        *do_not_execute = true;
    }

    //Cria o tabuleiro
    tabuleiro = tabuleiro_new(w, h, modo);
    tabuleiro_set_passeio(tabuleiro, passeio_A_new_read_from_file(vector2_read_from_file(fp)));
    tabuleiro_read_matrix_from_file(tabuleiro, fp);

    return tabuleiro;
}

Tabuleiro* read_file_modo_B(FILE* fp, int w, int h, char modo) {
    int num_pts_turisticos;
    Tabuleiro* tabuleiro;

    fscanf(fp, "%d", &num_pts_turisticos);

    //Cria o tabuleiro
    tabuleiro = tabuleiro_new(w, h, modo);
    tabuleiro_set_passeio(tabuleiro, passeio_B_new_read_from_file(num_pts_turisticos, fp));
    tabuleiro_read_matrix_from_file(tabuleiro, fp);

    return tabuleiro;
}

void read_and_write_files(char* filename) {
    FILE* fp = NULL;
    FILE* file_out = NULL;

    fp = fopen(filename, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error reading file %s\n", filename);
        exit(0);
    }
    file_out = fopen("saida.valid", "w");
    if(file_out == NULL) {
        fprintf(stderr, "Error reading file %s\n", "saida.valid");
        exit(0);
    }

    Tabuleiro* tabuleiro;

    while(true) {
        //Lê cada um dos tabuleiros no ficheiro
        int w, h;
        char modo;
        bool do_not_execute = false;

        //Tenta ler o tamanho do tabuleiro e modo
        if(fscanf(fp, "%d %d %c", &h, &w, &modo) != 3) {  //TODO eu troquei a ordem CONFIRMA
            break; //Se não conseguiu ler mais nenhum tabuleiro para a leitura
        }

        if(modo == 'A') {
            tabuleiro = read_file_modo_A(fp, w, h, modo, &do_not_execute);
        }else if(modo == 'B') {
            tabuleiro = read_file_modo_B(fp, w, h, modo);
        }else if(modo == 'C') {
            fprintf(stderr, "we are not ready for C files");
        } else {
            printf("Erro modo invalido?\n");
            exit(0);
        }
        //print_tabuleiro(&tabuleiro, w, h);

        //Na primeira fase de submissão o modo A apenas deve ter um ponto
        if(!do_not_execute) {
            //analisa o tabuleiro como devido
            tabuleiro_execute(tabuleiro, file_out);
        }
        tabuleiro_free(tabuleiro);
        free(tabuleiro);
    }
    fclose(file_out);
    fclose(fp);
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Invalid number of arguments\n");
        print_usage(argv[0]);
        return 1;
    }
    //  Lê o ficehiro de entrada (argv[1], analisa o(s) tabuleiro(s) e escreve no ficheiro de saída)
    read_and_write_files(argv[1]);

    return 0;
}
