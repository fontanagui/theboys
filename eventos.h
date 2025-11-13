#ifndef EVENTOS_H
#define EVENTOS_H

#include "mundo.h"
#include "fila.h"
#include "conjunto.h"

// estrutura do evento
struct evento {
    int tempo;
    int tipo;
    struct heroi *h;
    struct base *b;
    struct missao *mi;
};

void chega(struct evento *ev, mundo *m ); //1

void espera(struct evento *ev, mundo*m ); //2

void desiste(struct evento *ev, mundo *m ); //3

void avisa(struct evento *ev, mundo *m ); //4

void entra (struct evento *ev, mundo *m);//5

void sai(struct evento *ev, mundo *m);//6

void viaja(struct evento *ev,mundo *m );//7

void morre (struct evento *ev,mundo *m);//8


void missao(struct evento *ev, mundo *m );//9
 

void fim (struct evento *ev, mundo *m);//10


struct evento *cria_evento(int t, struct heroi *h, struct base *b, int type, struct missao *mi);
#endif