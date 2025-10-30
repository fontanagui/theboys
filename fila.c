#include <stdio.h>
#include <stdlib.h>
#include "fila.h"



// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria ()
{
    struct fila_t *f = (struct fila_t *)malloc(sizeof(struct fila_t));
    if (!f) return NULL;
    f->prim = NULL;
    f->fim = NULL;
    f->num = 0;
    return f;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *f)
{
    if (!f) return NULL;
    struct fila_nodo_t *atual = f->prim;
    while (atual) {
        struct fila_nodo_t *prox = atual->prox;
        free(atual->item);
        free(atual);
        atual = prox;
    }
    free(f);
    return NULL;
}

// Insere o item na fila
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fila_insere (struct fila_t *f, void *item)
{
    if (!f || !item) return -1;

    struct fila_nodo_t *novo_nodo = (struct fila_nodo_t *)malloc(sizeof(struct fila_nodo_t));
    if (!novo_nodo) return -1;
    novo_nodo->item = item;
    novo_nodo->prox = NULL;

    if (f->prim == NULL) {
        f->prim = novo_nodo;
        f->fim = novo_nodo;
    } else {
        f->fim->prox = novo_nodo;
        f->fim = novo_nodo;
    }
    f->num++;
    return f->num;
}

// Retira o primeiro item da fila e o devolve
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fila_retira (struct fila_t *f)
{
    if (!f || f->prim == NULL) return NULL;

    struct fila_nodo_t *nodo_retirado = f->prim;
    void *item = nodo_retirado->item;
    f->prim = nodo_retirado->prox;
    if (f->prim == NULL) {
        f->fim = NULL;
    }
    free(nodo_retirado);
    f->num--;
    return item;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f)
{
    if (!f) return -1;
    return f->num;
}

// Imprime o conteúdo da fila 
void fila_imprime (struct fila_t *f)
{
    if (!f) return;
    struct fila_nodo_t *atual = f->prim;
    while (atual) {
        printf("%p ", atual->item);
        atual = atual->prox;
    }
}



