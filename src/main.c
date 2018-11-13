#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h> //TODO remove

#include "tabuleiro.h"
#include "passeios.h"
#include "util.h"

void print_usage(char* program_name) {
    printf("Usage: %s file.cities\n", program_name);
}

Tabuleiro* read_file_modo_A(FILE* fp, int w, int h, char modo) {
    int num_pts_turisticos;
    Tabuleiro* tabuleiro;
    PasseioTipoA* passeio;

    fscanf(fp, "%d", &num_pts_turisticos);


    //Cria o tabuleiro
    tabuleiro = tabuleiro_new(w, h, modo);
    passeio = passeio_A_new_read_from_file(num_pts_turisticos, vector2_read_from_file(fp));
    tabuleiro_set_passeio(tabuleiro, passeio);
    for(int i = 0; i < num_pts_turisticos -1; i++){
        vector2_read_from_file(fp);
    }
    tabuleiro_read_matrix_from_file(tabuleiro, fp);

    if(num_pts_turisticos != 1 || modo != 'A'){
        passeio_A_set_valid(passeio, -1);
    }

    return tabuleiro;
}

Tabuleiro* read_file_modo_B(FILE* fp, int w, int h, char modo) {
    int num_pts_turisticos;
    Tabuleiro* tabuleiro;
    PasseioTipoB* passeio;

    fscanf(fp, "%d", &num_pts_turisticos);

    //Cria o tabuleiro
    tabuleiro = tabuleiro_new(w, h, modo);
    passeio = passeio_B_new_read_from_file(num_pts_turisticos, fp);
    tabuleiro_set_passeio(tabuleiro, passeio);
    tabuleiro_read_matrix_from_file(tabuleiro, fp);

    if(modo == 'b' || num_pts_turisticos < 2){
        passeio_B_set_valid(passeio, -1);
    }

    return tabuleiro;
}

void read_and_write_files(char* filename) {
    FILE* fp = NULL;
    FILE* file_out = NULL;

    if(!string_ends_with(filename, ".cities")) {
        fprintf(stderr, "%s is not a .cities file\n", filename);
        exit(0);
    }

    fp = fopen(filename, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error reading file %s\n", filename);
        exit(0);
    }

    char* file_out_name = create_dot_valid_filename(filename);
    file_out = fopen(file_out_name, "w");
    
    if(file_out == NULL) {
        fprintf(stderr, "Error reading file %s\n", "saida.valid");
        exit(0);
    }

    free(file_out_name); //TODO posso fazer free do file_out_name aqui ou tem de ser quando faço fclose

    Tabuleiro* tabuleiro = NULL;

    while(true) {
        //Lê cada um dos tabuleiros no ficheiro
        int w, h;
        char modo;

        //Tenta ler o tamanho do tabuleiro e modo
        if(fscanf(fp, "%d %d %c", &h, &w, &modo) != 3) {  //TODO eu troquei a ordem CONFIRMA
            break; //Se não conseguiu ler mais nenhum tabuleiro para a leitura
        }
        if(modo == 'A' || modo == 'a') {
            tabuleiro = read_file_modo_A(fp, w, h, modo);
        }else if(modo == 'B' || modo == 'b') {
            tabuleiro = read_file_modo_B(fp, w, h, modo);
        }else if(modo == 'C' || modo == 'c') {
            fprintf(stderr, "we are not ready for C files");
        } else {
            tabuleiro = read_file_modo_A(fp, w, h, modo);
            //fprintf(stderr, "Erro modo invalido?\n");
        }
        //print_tabuleiro(&tabuleiro, w, h);

        //analisa o tabuleiro como devido
        tabuleiro_execute(tabuleiro, file_out);

        tabuleiro_free(tabuleiro);
        free(tabuleiro);
        tabuleiro = NULL;
    }
    fclose(file_out);
    fclose(fp);
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Invalid number of arguments\n");
        print_usage(argv[0]);
        return 1;
    }
    //  Lê o ficehiro de entrada (argv[1], analisa o(s) tabuleiro(s) e escreve no ficheiro de saída)
    read_and_write_files(argv[1]);

    return 0;
}
