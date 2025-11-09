// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h> 
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "mundo.h"
#include "eventos.h"
#include "inicia_entidades.h"

// seus #defines vão aqui
#define T_INICIAL 0
#define T_FINAL 10000
#define TAM_MUNDO 1000
#define N_HABILIDADES 4
#define N_HEROIS 20
#define N_BASES 4
#define N_MISSOES 10
#define COMPOSTO_V 12

// minimize o uso de variáveis globais

int inicializa_mundo (mundo *m) {
    // inicializa os atributos do mundo aqui
    if (!m) return -1;
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

    // inicialize heróis, bases e missões conforme necessário
    for (int i = 0; i < N_HEROIS; i++) {
       m->herois[i] =cria_heroi(i);
    }
    for (int i=0;i<  N_BASES; i++) {
      m->bases[i]= cria_base(i);
    }
    for (int i =0;i<  N_MISSOES;i++){
      m->missao= cria_missao(i);
    }
}

// programa principal
int main ()
{
  struct fprio_t *LEF;    // cria a LEF de fato
  LEF = fprio_cria();
  mundo *m;
  m=cria_mundo();
  inicializa_mundo(m);
  int relogio= m->relogio;

 for (int i = 0; i < m->Nherois; i++) {
    struct base *b = &m->bases[rand() % m->Nbases]; // ponteiro para base
    int t = rand() % 4321;                          // tempo aleatório

    // cria evento e preenche campos
    evento *ev = malloc(sizeof(evento));
    ev->tempo = t;
    ev->h = &m->herois[i];
    ev->b = b;
    ev->tipo = 1;  // CHEGA
    ev->mi = NULL;

    // insere na fila de prioridade
    fprio_insere(LEF, ev, ev->tipo, ev->tempo); // prioridade = tempo
}

int type;
  while (relogio <= T_FINAL) {

    evento *ev = fprio_retira(LEF,&type,&relogio);
    if (!ev) break;
    switch (type)
    {
    case 1  :
      chega(ev);
      break;
    case 2  :
      espera(ev);
      break;
    case 3  :
      desiste(ev, m);
      break;
    case 4  :
      avisa(ev);
      break;
    case 5  :
      entra(ev);
      break;
    case 6  :
      sai(ev,m);
      break;
    case 7  : 
      viaja(ev,m);
      break;
    case 8  :
      morre(ev, m);
      break;
    case 9  :
      missao(ev,m);
      break;
    case 10 :
      fim(ev);
      break;
  }
  }
  return 0;
}