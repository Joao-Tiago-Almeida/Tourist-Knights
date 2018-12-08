#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tabuleiro.h"
#include "util.h"


void print_usage(char* program_name) {
    printf("Usage: %s file.cities\n", program_name);
}

Tabuleiro* read_file(FILE* fp, int w, int h, char modo) {
    int num_pts_turisticos = 0;
    Tabuleiro* tabuleiro = NULL;

    //Lê o numero de pontos turisticos do cabeçalho
    if(fscanf(fp, "%d", &num_pts_turisticos) != 1) {
        fprintf(stderr, "Erro de leitura");
        exit(0);
    }

    //Cria o tabuleiro
    tabuleiro = tabuleiro_new(w, h, modo);
    
    //Lê os pontos turisticos. Marca o tabueiro como invalido se algum ponto estiver fora do mapa
    tabuleiro_read_passeio_from_file(tabuleiro, num_pts_turisticos, fp);
    //TODO mudar passeio_get_valid para tabuleiro_get_valid

    //Se o tabuleiro ainda é válido verifica se o modo é válido e tem pontos suficientes também
    if(passeio_get_valid(tabuleiro) && tabuleiro_is_mode_valid(tabuleiro)) {
        //Se for válido lê matriz para o tabuleiro
        tabuleiro_read_matrix_from_file(tabuleiro, fp);

        //Verifica se as cidades do passeio são todas válidas (custo != 0), e se são todas accesiveis
        tabuleiro_check_passeio_invalid(tabuleiro);
    } else {
        //Se for inválido(tem um ponto fora) não vale a pena ler a matriz para o tabuleiro
        
        tabuleiro_read_matrix_from_file_invalid(tabuleiro, fp);
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

    free(file_out_name);

    Tabuleiro* tabuleiro = NULL;

    while(true) {
        int w, h;
        char modo;
        //Lê cada um dos tabuleiros no ficheiro

        //Tenta ler parte do cabeçalho(tamanho do tabuleiro e modo)
        if(fscanf(fp, "%d %d %c", &h, &w, &modo) != 3) {
            break; // Fim do ficheiro (Se não conseguiu ler mais nenhum tabuleiro para a leitura)
        }

        tabuleiro = read_file(fp, w, h, modo);

        // se o tabuleiro e o passio forem válidos
        if(passeio_get_valid(tabuleiro)) {
            //Analisa o tabuleiro como devido
            tabuleiro_execute(tabuleiro);
        }
        //Escreve o resultado
        tabuleiro_write_valid_file(tabuleiro, file_out);

        // se o tabuleiro e o passio forem válidos
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
