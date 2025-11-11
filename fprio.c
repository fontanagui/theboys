// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Definição da interface
 
// NÃO ALTERAR
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria ()
{
    struct fprio_t *f = (struct fprio_t *)malloc(sizeof(struct fprio_t));
    if (!f) return NULL;
    f->prim = NULL;
    f->fim = NULL;
    f->num = 0;
    return f;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f)
{
    if (!f) return NULL;
    struct fpnodo_t *atual = f->prim;
    while (atual) {
        struct fpnodo_t *prox = atual->prox;
        free(atual->item);
        free(atual);
        atual = prox;
    }
    free(f);
    return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {
    if (!f || !item) return -1;

    struct fpnodo_t *novo_nodo = (struct fpnodo_t *)malloc(sizeof(struct fpnodo_t));
    if (!novo_nodo) return -1;
    novo_nodo->item = item;
    novo_nodo->tipo = tipo;
    novo_nodo->prio = prio;
    novo_nodo->prox = NULL;

    if (f->prim == NULL) {
        f->prim = novo_nodo;
        f->fim = novo_nodo;
    } else if (prio < f->prim->prio) {
        novo_nodo->prox = f->prim;
        f->prim = novo_nodo;
    } else {
        struct fpnodo_t *atual = f->prim;
        struct fpnodo_t *anterior = NULL;

        while (atual != NULL && atual->prio <= prio) {
            anterior = atual;
            atual = atual->prox;
        }

        novo_nodo->prox = atual;
        anterior->prox = novo_nodo;

        if (atual == NULL) {
            f->fim = novo_nodo;
        }
    }

    f->num++;
    return f->num;
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
    if (!f || f->prim == NULL) return NULL;

    struct fpnodo_t *remover = f->prim;
    void *item = remover->item;
    if (tipo) *tipo = remover->tipo;
    if (prio) *prio = remover->prio;

    f->prim = remover->prox;
    if (f->prim == NULL) {
        f->fim = NULL;
    }

    free(remover);
    f->num--;
    return item;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f)
{
    if (!f) return -1;
    return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f)
{
    if (!f) return;
    struct fpnodo_t *atual = f->prim;
    while (atual) {
        printf("(%d %d) ", atual->tipo, atual->prio);
        atual = atual->prox;
    }
}
