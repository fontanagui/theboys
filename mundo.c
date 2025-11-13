#include "conjunto.h"
#include <stdio.h>
#include  "fila.h"
#include "fprio.h"
#include "mundo.h"
#include <string.h>
#define N_HABILIDADES 10
#define N_PRESENTES 10
#define MUNDO_TAM 100
#define TEMPO_MAX 100000
#define TAM_MUNDO 100000
#define T_INICIAL 0
#define N_HEROIS 20
#define N_BASES 5
#define N_MISSOES 100
#define COMPOSTO_V 30

int aleatoria (int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}


 struct heroi *cria_heroi(int id) {
    struct  heroi *h = (struct heroi *)malloc(sizeof( struct heroi));
    if (!h) return NULL;
    h->id = id;
    h->paciencia = aleatoria(0,100) ;
    h->velocidade = aleatoria(50,5000);
    h->experiencia = 0;
    h->base = -1; // sem base inicial
    int tamanho = aleatoria(0,3); // tamanho aleatório entre 0 e 3
    h->habilidades = cjto_aleat(tamanho, N_HABILIDADES );
    h->vivo = 1; // herói começa vivo
    return  h;
}


 struct base *cria_base(int id) {
    struct base *b = (struct base *)malloc(sizeof( struct base));
    if (!b) return NULL;
    b->id = id;
    b->lotacao = aleatoria(3,10); //(entre 3 e 10)
    b->presentes = cjto_cria(N_HEROIS); // 
    b->espera = fila_cria();
    b->local.x = aleatoria(0,MUNDO_TAM);
    b->local.y = aleatoria(0,MUNDO_TAM);
    b->missoes = 0;
    b->filamax=0;
    return b;
}


 struct missao *cria_missao(int id) {
    struct missao *missao= (struct missao *)malloc(sizeof( struct  missao));
    missao->id=id;
    int habilidades=rand()%(10-6+1)+6;
    missao->requisitos=cjto_aleat(habilidades,N_HABILIDADES);
    missao->local.x= rand()% MUNDO_TAM;
    missao->local.y=rand()% MUNDO_TAM;
    missao->status=0;
    missao->tentativas=0;
    missao->tempo=aleatoria(0,TEMPO_MAX-1);
    return missao;
}


mundo* cria_mundo() {
    mundo *m=( mundo *) malloc (sizeof ( mundo));
    if (!m) return NULL;
    m->dimensao.x = TAM_MUNDO;
    m->dimensao.y = TAM_MUNDO;
    m->relogio = T_INICIAL;
    m->Nherois = N_HEROIS;
    m->Nbases = N_BASES;
    m->Nmissoes = N_MISSOES;
    m->Nhabilidades = N_HABILIDADES;
    m->NCompostosV = COMPOSTO_V;
    m->herois= malloc(N_HEROIS* sizeof(struct heroi *));
    m->bases =malloc (N_BASES * sizeof(struct base *));
    m->missao=malloc(N_MISSOES* sizeof(struct missao *));
    m->eventos=0;
    m->missoes_completas=0;
    m->LEF= fprio_cria();

    // inicialize heróis, bases e missões conforme necessário
    for (int i = 0; i < N_HEROIS; i++) {
       m->herois[i] =cria_heroi(i);
    }
    for (int i=0;i<  N_BASES; i++) {
      m->bases[i]= cria_base(i);
    }
    for (int i =0;i<  N_MISSOES;i++){
      m->missao[i]= cria_missao(i);
    }
    return m;
}



void destroi_mundo(mundo *m) {
    if (!m) return;

    // --- heróis ---
    for (int i = 0; i < m->Nherois; i++) {
        if (m->herois[i]) {
            m->herois[i]->habilidades = cjto_destroi(m->herois[i]->habilidades);
            free(m->herois[i]);
        }
    }
    free(m->herois);

    // --- bases ---
    for (int i = 0; i < m->Nbases; i++) {
        if (m->bases[i]) {
            cjto_destroi(m->bases[i]->presentes);
            m->bases[i]->espera = fila_destroi(m->bases[i]->espera);
            free(m->bases[i]);
        }
    }
    free(m->bases);

    // --- missões ---
    for (int i = 0; i < m->Nmissoes; i++) {
        if (m->missao[i]) {
            m->missao[i]->requisitos = cjto_destroi(m->missao[i]->requisitos);
            free(m->missao[i]);
        }
    }
    free(m->missao);

    // --- fila de eventos (LEF) ---
    if (m->LEF)
        fprio_destroi(m->LEF);

    free(m);
}



