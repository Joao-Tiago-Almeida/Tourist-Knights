#include "passeios.h"

#include "util.h"
#include "movimentos.h"

/**
 * [passeio_A_new description]
 * @param  pos_ini [description]
 * @return         [description]
 */
PasseioTipoA* passeio_A_new_read_from_file(Vector2 pos_ini) {
    PasseioTipoA* passeio = (PasseioTipoA*)checked_malloc(sizeof(PasseioTipoA));

    passeio->pos_ini = pos_ini;
    //passeio->pos_fim = pos_fim;

    return passeio;
}
/**
 * [passeio_B_new_read_from_file description]
 * @param  num_pontos [description]
 * @param  fp         [description]
 * @param  tabuleiro  [description]
 * @return            [description]
 */
PasseioTipoB* passeio_B_new_read_from_file(int num_pontos, FILE* fp, Tabuleiro tabuleiro) {
    PasseioTipoB* passeio = (PasseioTipoB*) checked_malloc(sizeof(PasseioTipoB));

    passeio->num_pontos = num_pontos;
    passeio->pontos = (Vector2*) checked_malloc(sizeof(Vector2) * num_pontos);

    //escreve no vetor
    for(int i = 0; i < num_pontos; i++)
    {
        (*passeio).pontos[i] = vector2_read_from_file(fp);
        //FIXME
        inside_board(passeio->pontos[i], tabuleiro);
    }
    return passeio;
}
