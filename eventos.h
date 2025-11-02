#include "mundo.h"

#ifndef EVENTOS
#define EVENTOS


void chega(int t, struct heroi *h, struct base b);


void espera(int t, struct heroi *h, struct base b);

void desiste(int t, struct heroi *h , struct base b);

void avisa(int t, struct base b);

void entra (int t, struct heroi *h, struct base b);

void sai(int t, struct heroi *h, struct base b);

void viaja(int t, struct heroi *h, struct base d);     

void morre(int t, struct heroi *h, struct base b);


void missao(int t, struct heroi *h, struct missao m);
 

void fim (int t);

#endif