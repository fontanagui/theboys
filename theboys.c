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
    // inicialize os atributos do mundo aqui
    if (!m) return -1;
    m->dimensao.x = TAM_MUNDO;
    m->dimensao.y = TAM_MUNDO;
    m->relogio = T_INICIAL;
    m->Nherois = N_HEROIS;
    m->Nbases = N_BASES;
    m->Nmissoes = N_MISSOES;
    m->Nhabilidades = N_HABILIDADES;
    m->NCompostosV = COMPOSTO_V;

    // inicialize heróis, bases e missões conforme necessário
    for (int i = 0; i < N_HEROIS; i++) {
        m->herois[i].id = i;
        m->herois[i].paciencia = rand() % 101;  // exemplo de valor
        m->herois[i].velocidade = (rand() % (5000 - 50 + 1)) + 50;
        m->herois[i].experiencia = 0;
        m->herois[i].base = -1; // sem base inicial
        int tamanho = rand() % 4; // tamanho aleatório entre 0 e 3
        m->herois[i].habilidades = cjto_cria(tamanho);
        m->herois[i].vivo = 1; // herói começa vivo

        while (cjto_cardinalidade(m->herois[i].habilidades) < tamanho) {
          int valor = rand() % N_HABILIDADES + 1; // número entre 1 e N_HABILIDADES
          cjto_insere(m->herois[i].habilidades, valor);
}

    }
    for (int i = 0; i < N_BASES; i++) {
        m->bases[i].id = i;
        m->bases[i].lotacao = rand() % (10 - 3 + 1) + 3; //(entre 3 e 10)
        m->bases[i].presentes = cjto_cria(0); // conjunto vazio inicialmente
        m->bases[i].espera = fila_cria();
        m->bases[i].local.x = rand() % TAM_MUNDO;
        m->bases[i].local.y = rand() % TAM_MUNDO;
    }

    for (int i = 0; i < N_MISSOES; i++) {
        m->missao[i].id = i;
        int tamanho = rand() % 3 + 2; // Gera 2, 3 ou 4
        m->missao[i].requisitos = cjto_cria(tamanho);

        while (cjto_card(m->missao[i].requisitos) < tamanho) {
          int valor = rand() % 4 + 1; // Gera número de 1 a 4
          cjto_insere(m->missao[i].requisitos, valor);
      }

        m->missao[i].local.x = rand() % TAM_MUNDO;
        m->missao[i].local.y = rand() % TAM_MUNDO;
        m->missao[i].status = 0; // missão não cumprida
    }
    return 0;
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
      chega(ev->tempo, ev->h, ev->b);
      break;
    case 2  :
      espera(ev->tempo, ev->h, ev->b);
      break;
    case 3  :
      desiste(ev->tempo, ev->h, ev->b);
      break;
    case 4  :
      avisa(ev->tempo, ev->b);
      break;
    case 5  :
      entra(ev->tempo, ev->h, ev->b);
      break;
    case 6  :
      sai(ev->tempo, ev->h, ev->b);
      break;
    case 7  : 
      viaja(ev->tempo, ev->h, ev->b);
      break;
    case 8  :
      morre(ev->tempo, ev->h, ev->b);
      break;
    case 9  :
      missao(ev->tempo, ev->mi);
      break;
    case 10 :
      fim(ev->tempo);
      break;
  }
  }
  return 0;
}