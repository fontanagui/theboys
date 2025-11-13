#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"

int main(void)
{
    printf("=== Teste do módulo MUNDO ===\n\n");

    // cria o mundo
    mundo *m = cria_mundo();
    if (!m) {
        fprintf(stderr, "Erro: falha ao criar o mundo.\n");
        return 1;
    }

    printf("Mundo criado com sucesso!\n");
    printf("Dimensão: %d x %d\n", m->dimensao.x, m->dimensao.y);
    printf("Heróis: %d, Bases: %d, Missões: %d\n", m->Nherois, m->Nbases, m->Nmissoes);
    printf("------------------------------------\n");

    // imprime alguns heróis
    for (int i = 0; i < m->Nherois; i++) {
        struct heroi *h = m->herois[i];
        printf("Herói %2d -> paciência: %3d | velocidade: %4d | vivo: %d\n",
               h->id, h->paciencia, h->velocidade, h->vivo);
    }
    printf("------------------------------------\n");

    // imprime algumas bases
    for (int i = 0; i < m->Nbases; i++) {
        struct base *b = m->bases[i];
        printf("Base %2d -> lotação: %2d | local: (%d,%d)\n",
               b->id, b->lotacao, b->local.x, b->local.y);
    }
    printf("------------------------------------\n");

    // imprime algumas missões
    for (int i = 0; i < 5 && i < m->Nmissoes; i++) {
        struct missao *mi = m->missao[i];
        printf("Missão %3d -> status: %d | tempo: %d | local: (%d,%d)\n",
               mi->id, mi->status, mi->tempo, mi->local.x, mi->local.y);
    }
    printf("------------------------------------\n");

    // destrói tudo
    printf("Destruindo mundo...\n");
    destroi_mundo(m);
    printf("Mundo destruído com sucesso!\n");

    return 0;
}
