#include "conjunto.h"
#ifndef ENTIDADES
#define MAXH 100
#define MAXB 50
#define ENTIDADES

struct cordenada 
{
    int x ;
    int y ;
} ;

struct mundo {
    struct cordenada  dimensao ;       // dimensões do mundo
    int relogio ;          // relógio do mundo
    int Nherois ;         // número de heróis no mundo
    int Nbases ;         // número de bases no mundo
    int Nmissoes ;        // número de missões no mundo
    int Nhabilidades ;    // número de habilidades no mundo
    int NCompostosV  ;  // número de compostos v no mundo
struct heroi
{
    int id ;               // identificador do herói
    int paciencia ;        // paciência do herói
    int velocidade ;       // velocidade do herói
    int experiencia ;      // experiência do herói
    int base ;              // base do herói
    struct conjto_t *habilidades ; // conjunto de habilidades do herói
    
};
struct heroi herois[MAXH] ; // vetor de heróis no mundo


struct base
{
    int id ;               // identificador da base
    int lotacao ;       // capacidade da base
    struct conjto_t *presentes ; // conjunto de ids dos herois  na base
    struct cordenada  local ;           // coordenadas da base
} ;
struct base bases [MAXB] ; // vetor de bases no mundo

};

#endif