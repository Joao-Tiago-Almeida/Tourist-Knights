#ifndef CITIES_FILE_H
#define CITIES_FILE_H

typedef struct Tabuleiro;

typedef struct citiesfile_t {
    int num_tabuleiros;
    Tabuleiro* tabuleiros;
} CitiesFile;

CitiesFile citiesfile_new(int num_tabuleiros, Tabuleiro* tabuleiros);

#endif
