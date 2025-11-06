#include "mundo.h"
#include "inicia_entidades.h"
#include <stdlib.h>
#include <stdio.h>
#include "fprio.h"
#include "fila.h"

#define N_HABILIDADES 10
#define N_PRESENTES 10
#define MUNDO_TAM 100
#define TMP_MAX 100000

struct heroi *cria_heroi(int id) {
    struct heroi *h = (struct heroi *)malloc(sizeof(struct heroi));
    if (!h) return NULL;
    h->id = id;
    h->paciencia = rand() % 101;  
    h->velocidade = (rand() % (5000 - 50 + 1)) + 50;
    h->experiencia = 0;
    h->base = -1; // sem base inicial
    int tamanho = rand() % 4; // tamanho aleatório entre 0 e 3
    h->habilidades = cjto_aleat(tamanho, N_HABILIDADES );
    h->vivo = 1; // herói começa vivo
    return  h;
}


struct base *cria_base(int id) {
    struct base *b = (struct base *)malloc(sizeof(struct base));
    if (!b) return NULL;
    b->id = id;
    b->lotacao = rand() % (10 - 3 + 1) + 3; //(entre 3 e 10)
    b->presentes = cjto_cria(N_PRESENTES); // 
    b->espera = fila_cria();
    b->local.x = rand() % MUNDO_TAM; 
    b->local.y = rand() %  MUNDO_TAM;
    b->missoes = 0;
    return b;
}


struct missao *cria_missao(int id) {
    struct missao *missao= (struct missao *)malloc(sizeof(struct missao));
    missao->id=id;
    int habilidades=rand()%(10-6+1)+6;
    missao->requisitos=cjto_aleat(habilidades,N_HABILIDADES);
    missao->local.x= rand()% MUNDO_TAM;
    missao->local.y=rand()% MUNDO_TAM;
    missao->status=0;
    missao->tempo= rand()% TMP_MAX +3420;
    return missao;
}