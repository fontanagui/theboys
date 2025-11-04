#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fila.h"
#include "fprio.h"
#include "mundo.h"
#include "eventos.h"


extern struct fprio_t *LEF;  // LEF global
extern mundo *m;        // mundo global



// Verifica se a base tem pelo menos um herói vivo
int base_tem_heroi_vivo(struct base *b, mundo *mundo_global) {
    for (int hid = 0; hid < MAXH; hid++) {
        if (cjto_pertence(b->presentes, hid)) {
            if (mundo_global->herois[hid].vivo) return 1;
        }
    }
    return 0;
}
// Cria a união das habilidades de todos os heróis vivos da base
struct cjto_t *habilidades_base(struct base *b, mundo *mundo_global) {
    struct cjto_t *habilidades = cjto_cria(mundo_global->Nhabilidades);
    for (int hid = 0; hid < MAXH; hid++) {
        if (cjto_pertence(b->presentes, hid)) {
            struct heroi *h = &mundo_global->herois[hid];
            if (h->vivo) {
                struct cjto_t *temp = cjto_uniao(habilidades, h->habilidades);
                cjto_destroi(habilidades);
                habilidades = temp;
            }
        }
    }
    return habilidades;
}


// Agenda a missão para 24h depois
void adia_missao(int t, struct missao *mi) {
    evento *ev = malloc(sizeof(evento));
    ev->tempo = t + 24*60;
    ev->h = NULL;
    ev->b = NULL;
    ev->mi = mi;
    ev->tipo= 9; // MISSÃO
    fprio_insere(LEF, ev, ev->tipo, ev->tempo);
    printf("%d: MISSÃO %d ADIADA\n", t, mi->id);
}

void chega(int t, struct heroi *h, struct base *b) {
    if (!h || !b || (h->vivo==0)) return;
    h->base = b->id;
    printf("%d: CHEGA HEROI %d BASE %d.\n", t, h->id, b->id);

    int espera_evento = ((cjt_card(b->presentes) < b->lotacao) && fila_vazia(b->espera)) ||
                        (h->paciencia > 10 * b->espera->num);

    evento *ev = malloc(sizeof(evento));
    if (!ev) return;
    ev->tempo = t;
    ev->h = h;
    ev->b = b;
    ev->tipo = espera_evento ? 2 : 3;
    ev->mi = NULL;

    fprio_insere(LEF, ev, ev->tipo, ev->tempo);
}

void espera(int t, struct heroi *h, struct base *b) {
    if (!h || !b || (h->vivo==0)) return;
    fila_insere(b->espera, h);
    printf("%d: HEROI %d ENTRA NA FILA DE ESPERA DA BASE %d.\n", t, h->id, b->id);

    evento *ev = malloc(sizeof(evento));
    if (!ev) return;
    ev->tempo = t;
    ev->h = NULL;
    ev->b = b;
    ev->tipo = 4;  // AVISA
    ev->mi = NULL;

    fprio_insere(LEF, ev, ev->tipo, ev->tempo);
}



void desiste(int t, struct heroi *h, struct base *b) {
    if (!h || !b || (h->vivo==0)) return;
    int indice_b = b->id;
    int indice_destino;

    // gera índice aleatório diferente do índice atual
    if (m->Nbases <= 1) return;  // não há outra base

    indice_destino = rand() % (m->Nbases - 1);
    if (indice_destino >= indice_b) {
        indice_destino += 1;
    }

    struct base *D = &m->bases[indice_destino];

    printf("%d: HEROI %d DESISTE DA BASE %d E VAI PARA BASE %d.\n", t, h->id, b->id, D->id);

    // Cria evento VIAJA
    evento *ev = malloc(sizeof(evento));
    if (!ev) return;

    ev->tempo = t;
    ev->h = h;
    ev->b = D;           // destino da viagem
    ev->tipo= 7;  // VIAJA
    ev->mi = NULL;

    fprio_insere(LEF, ev, ev->tipo, ev->tempo);
}

void avisa(int t, struct base *b) {
    struct heroi *h;

    // Enquanto houver vagas e heróis na fila
    while (cjto_card(b->presentes) < b->lotacao && !fila_vazia(b->espera) ) {
        h = fila_retira(b->espera);       // retira primeiro herói da fila
        if (!h || (h->vivo==0)) continue;
        cjto_insere(b->presentes, h->id); // adiciona ao conjunto de presentes

        printf("%d: HEROI %d ENTRA NA BASE %d.\n", t, h->id, b->id);

        // Cria evento ENTRA
        evento *ev = malloc(sizeof(evento));
        if (!ev) continue;

        ev->tempo = t;
        ev->h = h;
        ev->b = b;
        ev->tipo = 5;  // ENTRA
        ev->mi = NULL;

        fprio_insere(LEF, ev, ev->tipo, ev->tempo);
    }
}


void entra(int t, struct heroi *h, struct base *b) {
    if (!h || !b || (h->vivo==0)) return;
    // Calcula tempo de permanência na base
    int aleatorio = rand() % 20 + 1;  // número aleatório entre 1 e 20
    int TPB = 15 + h->paciencia * aleatorio;

    printf("%d: HEROI %d ENTRA NA BASE %d POR %d UNIDADES DE TEMPO.\n", t, h->id, b->id, TPB);

    // Cria evento SAI
    evento *ev = malloc(sizeof(evento));
    if (!ev) return;

    ev->tempo = t + TPB; // momento em que o herói sai
    ev->h = h;
    ev->b = b;
    ev->tipo = 6;  // SAI
    ev->mi = NULL;

    fprio_insere(LEF, ev, ev->tipo, ev->tempo);
}

void sai(int t, struct heroi *h, struct base *b) {
    if (!h || !b || (h->vivo==0)) return;
    // Remove o herói do conjunto de presentes da base
    cjto_remove(b->presentes, h->id);

    // Escolhe uma base destino aleatória (pode ser a mesma)
    int indice_destino = rand() % m->Nbases;
    struct base *D = &m->bases[indice_destino];

    printf("%d: HEROI %d SAI DA BASE %d E VIAJA PARA BASE %d.\n", t, h->id, b->id, D->id);

    // Cria evento VIAJA
    evento *ev_viaja = malloc(sizeof(evento));
    if (ev_viaja) {
        ev_viaja->tempo = t;
        ev_viaja->h = h;
        ev_viaja->b = D;
        ev_viaja->tipo = 7;  // VIAJA
        ev_viaja->mi = NULL;
        fprio_insere(LEF, ev_viaja, ev_viaja->tipo, ev_viaja->tempo);

    }

    // Cria evento AVISA para a base original
    evento *ev_avisa = malloc(sizeof(evento));
    if (ev_avisa) {
        ev_avisa->tempo = t;
        ev_avisa->h = NULL;
        ev_avisa->b = b;
        ev_avisa->tipo= 4;  // AVISA    
        ev_avisa->mi = NULL;
        fprio_insere(LEF, ev_avisa, ev_avisa->tipo, ev_avisa->tempo);
    }
}



void viaja(int t, struct heroi *h, struct base *d) {
    if (!h || !d || (h->vivo==0)) return;
    struct base *B_atual = NULL;

    // Se o herói tem uma base atual válida, pega a base
    if (h->base >= 0 && h->base < m->Nbases) {
        B_atual = &m->bases[h->base];
    } else {
        // Caso não tenha base, assume origem (0,0)
        static struct base origem = { .local = {0,0} };
        B_atual = &origem;
    }

    // Calcula distância cartesiana
    int dx = d->local.x - B_atual->local.x;
    int dy =d->local.y - B_atual->local.y;
    double distancia = sqrt(dx*dx + dy*dy);

    // Calcula duração da viagem (tempo = distância / velocidade)
    int duracao = (int) ceil(distancia / h->velocidade);

    printf("%d: HEROI %d VIAJA DE BASE %d PARA BASE %d, DURACAO %d.\n",
           t, h->id, B_atual->id, d->id, duracao);

    // Cria evento CHEGA

    evento *ev_chega = malloc(sizeof(evento));
    if (ev_chega) {
        ev_chega->tempo = t + duracao;
        ev_chega->h = h;
        ev_chega->b = d;
        ev_chega->tipo= 1;  // AVISA    
        ev_chega->mi = NULL;
        fprio_insere(LEF, ev_chega, ev_chega->tipo, ev_chega->tempo);
    }
}


  


void morre(int t, struct heroi *h, struct base *b) {
    if (!h || !b) return;

    // Retira herói do conjunto de presentes da base
    cjto_retira(b->presentes, h->id);

    // Marca o herói como morto
    h->vivo = 0;

    printf("%d: HEROI %d MORRE NA BASE %d.\n", t, h->id, b->id);

    // Cria evento AVISA para a base
    evento *ev = malloc(sizeof(evento));
    if (!ev) return;

    ev->tempo = t;
    ev->h = NULL;  // não há herói associado
    ev->b = b;
    ev->tipo = 4;  // AVISA
    ev->mi = NULL;

    fprio_insere(LEF, ev, ev->tipo, ev->tempo);
}


void missao(int t,  struct missao *mi) {
    if (!mi) return;

    // 1. Encontrar a base mais próxima com pelo menos 1 herói vivo
    int BMP_index = -1;
    double menor_dist = 1e9;
    for (int i = 0; i < m->Nbases; i++) {
        struct base *b = &m->bases[i];
        if (!base_tem_heroi_vivo(b, m)) continue;

        double dx = b->local.x - mi->local.x;
        double dy = b->local.y - mi->local.y;
        double dist = sqrt(dx*dx + dy*dy);
        if (dist < menor_dist) {
            menor_dist = dist;
            BMP_index = i;
        }
    }

    if (BMP_index == -1) {
        printf("%d: MISSÃO %d ADIADA (nenhuma base com heróis vivos)\n", t, mi->id);
        adia_missao(t, mi);
        return;
    }

    struct base *b = &m->bases[BMP_index];

    // 2. Verificar se os heróis da base têm todas as habilidades necessárias
    struct cjto_t *habs = habilidades_base(b, m);
    if (cjto_contem(habs, mi->requisitos)) {
        // Missão cumprida normalmente
        mi->status = 1;
        for (int hid = 0; hid < MAXH; hid++) {
            if (cjto_pertence(b->presentes, hid)) {
                struct heroi *h = &m->herois[hid];
                if (h->vivo) h->experiencia += 10;
            }
        }
        printf("%d: MISSÃO %d CUMPRIDA PELA BASE %d\n", t, mi->id, b->id);
        cjto_destroi(habs);
        return;
    }
    cjto_destroi(habs);

    // 3. Verificar se é possível usar Composto V
    if (m->NCompostosV > 0 && t % 2500 == 0) {
        int h_max = -1, exp_max = -1;
        for (int hid = 0; hid < MAXH; hid++) {
            if (cjto_pertence(b->presentes, hid)) {
                struct heroi *h = &m->herois[hid];
                if (h->vivo && h->experiencia > exp_max) {
                    h_max = hid;
                    exp_max = h->experiencia;
                }
            }
        }

        if (h_max != -1) {
            struct heroi *h = &m->herois[h_max];
            m->NCompostosV--;
            mi->status = 1;

            evento *ev = malloc(sizeof(evento));
            ev->tempo = t;
            ev->h = h;
            ev->b = b;
            ev->mi = NULL;
            ev->tipo = 8; //morre
            fprio_insere(LEF, ev, ev->tipo, ev->tempo);

            for (int hid = 0; hid < MAXH; hid++) {
                if (cjto_pertence(b->presentes, hid) && hid != h_max) {
                    struct heroi *h2 = &m->herois[hid];
                    if (h2->vivo) h2->experiencia += 10;
                }
            }

            printf("%d: MISSÃO %d CUMPRIDA COM COMPOSTO V PELO HERÓI %d\n", t, mi->id, h_max);
            return;
        }
    }

    // 4. Nenhuma opção → adiar missão
    adia_missao(t, mi);
}
void fim (int t);
