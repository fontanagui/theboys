#include "mundo.h"

#ifndef EVENTOS
#define EVENTOS
extern struct fprio_t *LEF; // declara LEF para outros arquivos


typedef struct evento {
    int tempo;                                  // tempo do evento
    struct heroi *h;                            // herói envolvido
    struct base *b;                             // base envolvida
    int tipo;                                     // tipo do evento
    struct missao *mi // ponteiro para função
} evento;


void chega(struct evento *ev );

void espera(struct evento *ev );

void desiste(mundo *m, struct evento *ev );

void avisa(struct evento *ev );

void entra (struct evento *ev );

void sai(struct evento *ev, mundo *m);

void viaja(struct evento *ev,mundo *m );

void morre (struct evento *ev,mundo *m);


void missao(struct evento *ev, mundo *m );
 

void fim (struct evento *ev );

#endif