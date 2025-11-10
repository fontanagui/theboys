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


// programa principal
int main ()
{
  
  mundo *m;
  m=cria_mundo();
  inicializa_mundo(m);
  int relogio= m->relogio;

 for (int i = 0; i < m->Nherois; i++) {
    struct base *b = m->bases[rand() % m->Nbases]; // ponteiro para base
    int t = rand() % 4321;                          // tempo aleatório

    // cria evento e preenche campos
    struct evento *ev = malloc(sizeof(struct evento *));
    ev->tempo = t;
    ev->h = &m->herois[i];
    ev->b = b;
    ev->tipo = 1;  // CHEGA
    ev->mi = NULL;

    // insere na fila de prioridade
    fprio_insere(m->LEF, ev, ev->tipo, ev->tempo); // prioridade = tempo
}

int type;
  while (relogio <= T_FINAL) {

    struct evento *ev = fprio_retira(m->LEF,&type,&relogio);
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