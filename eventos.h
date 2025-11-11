#include "mundo.h"


#ifndef EVENTOS
#define EVENTOS


 struct evento {
    int tempo;                                  // tempo do evento
    struct heroi *h;                            // herói envolvido
    struct base *b;                             // base envolvida
    int tipo;                                     // tipo do evento
    struct missao *mi // ponteiro para função
} ;


void chega(struct evento *ev, mundo *m );

void espera(struct evento *ev, mundo*m );

void desiste(struct evento *ev, mundo *m );

void avisa(struct evento *ev, mundo *m );

void entra (struct evento *ev, mundo *m);

void sai(struct evento *ev, mundo *m);

void viaja(struct evento *ev,mundo *m );

void morre (struct evento *ev,mundo *m);


void missao(struct evento *ev, mundo *m );
 

void fim (struct evento *ev, mundo *m);

#endif