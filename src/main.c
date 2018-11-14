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

bool tabuleiro_and_passeio_is_valid(Tabuleiro* tab) {
    char modo = tabuleiro_get_tipo_passeio(tab);
    Passeio* passeio = (Passeio*)tabuleiro_get_passeio(tab);
    return ((modo == 'A' && passeio_get_num_pontos(passeio) == 1) || (modo == 'B' && passeio_get_num_pontos(passeio) >= 2))
        && passeio_get_valid((Passeio*)tabuleiro_get_passeio(tab));
}

Tabuleiro* read_file(FILE* fp, int w, int h, char modo) {
    int num_pts_turisticos;
    Tabuleiro* tabuleiro;
    Passeio* passeio;

    if(fscanf(fp, "%d", &num_pts_turisticos) != 1) {
        fprintf(stderr, "Erro de leitura");
        exit(0);
    }

    //Cria o tabuleiro
    tabuleiro = tabuleiro_new(w, h, modo);
    passeio = passeio_new_read_from_file(tabuleiro, num_pts_turisticos, fp);
    tabuleiro_set_passeio(tabuleiro, passeio);
    
    if(!tabuleiro_and_passeio_is_valid(tabuleiro)) {
        //Se for inválido não vale a pena ler a matriz para o tabuleiro
        passeio_set_valid(passeio, -1);
        tabuleiro_read_matrix_from_file_invalid(tabuleiro, fp);
    } else {
        //Se for válido lê matriz para o tabuleiro
        tabuleiro_read_matrix_from_file(tabuleiro, fp);
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
        int w, h;
        char modo;
        //Lê cada um dos tabuleiros no ficheiro

        //Tenta ler o cabeçalho(tamanho do tabuleiro e modo)
        if(fscanf(fp, "%d %d %c", &h, &w, &modo) != 3) {
            break; // Fim do ficheiro (Se não conseguiu ler mais nenhum tabuleiro para a leitura)
        }

        if(modo == 'C' || modo == 'c') {
            fprintf(stderr, "we are not ready for C files");
            continue;
        } else {
            tabuleiro = read_file(fp, w, h, modo);
        }

        //Analisa o tabuleiro como devido
        tabuleiro_execute(tabuleiro, file_out);
        //Escreve o resultado
        tabuleiro_write_valid_file(tabuleiro, file_out);

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
