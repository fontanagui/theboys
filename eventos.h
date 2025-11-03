#include "mundo.h"

#ifndef EVENTOS
#define EVENTOS
extern struct fprio_t *LEF; // declara LEF para outros arquivos


typedef struct evento {
    int tempo;                                  // tempo do evento
    struct heroi *h;                            // herói envolvido
    struct base *b;                             // base envolvida
    void (*funcao)(int, struct heroi*, struct base*);
    struct missao *mi // ponteiro para função
} evento;


void chega(int t, struct heroi *h, struct base *b);


void espera(int t, struct heroi *h, struct base *b);

void desiste(int t, struct heroi *h , struct base *b);

void avisa(int t, struct base *b);

void entra (int t, struct heroi *h, struct base *b);

void sai(int t, struct heroi *h, struct base *b);

void viaja(int t, struct heroi *h, struct base *d);     

void morre(int t, struct heroi *h, struct base *b);


void missao(int t, struct heroi *h, struct missao *mi);
 

void fim (int t);

#endif