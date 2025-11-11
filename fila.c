#include <stdio.h>
#include <stdlib.h>
#include "fila.h" 

// Cria uma fila vazia
struct fila_t *fila_cria() {
    struct fila_t *f = malloc(sizeof(struct fila_t));
    if (f == NULL)
        return NULL;
    f->prim = NULL;
    f->ult = NULL;
    f->num = 0;
    return f;
}

// Libera todas as estruturas de dados da fila, inclusive os nodos
struct fila_t *fila_destroi(struct fila_t *f) {
    if (f == NULL)
        return NULL;

    struct fila_nodo_t *atual = f->prim;
    while (atual != NULL) {
        struct fila_nodo_t *prox = atual->prox;
        free(atual);
        atual = prox;
    }

    free(f);
    return NULL;
}

// Insere um item no final da fila (política FIFO)
int fila_insere(struct fila_t *f, int item) {
    if (f == NULL)
        return 0;

    struct fila_nodo_t *novo = malloc(sizeof(struct fila_nodo_t));
    if (novo == NULL)
        return 0;

    novo->item = item;
    novo->prox = NULL;

    if (f->ult == NULL) {
        // Fila vazia
        f->prim = novo;
        f->ult = novo;
    } else {
        f->ult->prox = novo;
        f->ult = novo;
    }

    f->num++;
    return 1;
}

// Retira o primeiro item da fila e o devolve
int fila_retira(struct fila_t *f, int *item) {
    if (f == NULL || f->prim == NULL)
        return 0;

    struct fila_nodo_t *removido = f->prim;
    if (item != NULL)
        *item = removido->item;

    f->prim = removido->prox;

    if (f->prim == NULL)
        f->ult = NULL; // fila ficou vazia

    free(removido);
    f->num--;

    return 1;
}

// Informa o número de itens na fila
int fila_tamanho(struct fila_t *f) {
    if (f == NULL)
        return -1;
    return f->num;
}

// Imprime o conteúdo da fila
void fila_imprime(struct fila_t *f) {
    if (f == NULL) {
        printf("(fila nula)\n");
        return;
    }

    struct fila_nodo_t *atual = f->prim;
    printf("Fila: ");
    while (atual != NULL) {
        printf("%d ", atual->item);
        atual = atual->prox;
    }
    printf("\n");
}
