#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fila.h"
#include "fprio.h"
#include "mundo.h"

void chega(int t, struct heroi *h, struct base *b) {
    h->base = b->id;
    if ( ((cjt_card(b->presentes) < b->lotacao) && (fila_vazia(b->espera)) ) || (h->paciencia > (10 * b->espera->num))){
        printf("%d: CHEGA HEROI %d BASE %d.\n", t, h->id, b->id);
        fprio_insere ;
   
    }
}


void espera(int t, struct heroi *h, struct base b);
void desiste(int t, struct heroi *h , struct base b);

void avisa(int t, struct base b);

void entra (int t, struct heroi *h, struct base b);

void sai(int t, struct heroi *h, struct base b);

void viaja(int t, struct heroi *h, struct base d);     

void morre(int t, struct heroi *h, struct base b);


void missao(int t, struct heroi *h, struct missao m);
 

void fim (int t);
