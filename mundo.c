#include "conjunto.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include  "fila.h"
#include "fprio.h"
#include "mundo.h"



mundo* cria_mundo() {
    mundo *m=( mundo *) malloc (sizeof ( mundo));
   if (!m) return NULL;
   return m;
}