#include "conjunto.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include  "fila.h"
#include "fprio.h"



#ifndef ENTIDADES
#define ENTIDADES


struct cordenada
{
    int x;
    int y;
};

struct heroi
{
    int id;                    // identificador do herói
    int paciencia;             // paciência do herói
    int velocidade;            // velocidade do herói
    int experiencia;           // experiência do herói
    int base;                  // base do herói
    struct conjto_t *habilidades; // conjunto de habilidades do heroi
    int vivo;                  // status de vida do herói (1 = vivo, 0 = morto)
};

struct base
{
    int id;                    // identificador da base
    int lotacao;               // capacidade da base
    struct conjto_t *presentes;  // conjunto de ids dos herois na base
    struct fila_t *espera ;     // fila de espera na base
    struct cordenada local;    // coordenadas da base
    int missoes ;              // número de missões completadas pela base


};

struct missao
{
    int id;                    // identificador da missão
    struct conjto_t *requisitos; // conjunto de habilidades necessárias
    struct cordenada local; // coordenadas da missão
    int status;
    int tempo;
};

typedef struct
{
    struct cordenada dimensao; // dimensões do mundo
    int relogio;               // relógio do mundo
    int Nherois;               // número de heróis no mundo
    int Nbases;                // número de bases no mundo
    int Nmissoes;              // número de missões no mundo
    int Nhabilidades;          // número de habilidades no mundo
    int NCompostosV;           // número de compostos v no mundo
    struct heroi **herois; // vetor de heróis no mundo
    struct base **bases;   // vetor de bases no mundo
    struct missao **missao; // vetor de missões no mundo
    int vivos;                   // número de heróis vivos no mundo
    int eventos ;                 // número de eventos processados no mundo 
    int missoes_completas;        // número de missões completadas no mundo
} mundo;



mundo* cria_mundo();
#endif