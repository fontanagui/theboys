// eventos.c - versão corrigida
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "mundo.h"
#include "conjunto.h"
#include "fprio.h"
#include "fila.h"
#include "eventos.h"

#define N_HABILIDADES 10
#define INT_MAX_LOCAL 100000

/* --- utilitárias simples --- */

int min_missao(mundo *m){
    int min = INT_MAX_LOCAL;
    for (int i = 0; i < m->Nmissoes; i++){
        if (m->missao[i] && m->missao[i]->tentativas < min)
            min = m->missao[i]->tentativas;
    }
    if (min == INT_MAX_LOCAL)
        return -1;
    return min;
}

int max_missao(mundo *m){
    int max = -1;
    for (int i = 0; i < m->Nmissoes; i++){
        if (m->missao[i] && m->missao[i]->tentativas > max)
            max = m->missao[i]->tentativas;
    }
    return max;
}

float media_missao(mundo *m) {
    if (m->Nmissoes == 0) return 0.0f;
    float soma = 0.0f;
    for (int i = 0; i < m->Nmissoes; i++) {
        if (m->missao[i])
            soma += (float)m->missao[i]->tentativas;
    }
    soma = soma / (float)m->Nmissoes;
    return soma;
}

/* distância euclidiana (retorna inteiro truncado) */
int calcula_distancia(struct coordenada p1, struct coordenada p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    double dist = sqrt((double)dx * (double)dx + (double)dy * (double)dy);
    return (int) dist;
}

/* --- Funções do domínio --- */

/* Verifica se a equipe da base tem todas as habilidades da missão.
   Nota: o parâmetro hab_base é ignorado aqui (não conseguimos "retornar"
   via parâmetro sem alterar a assinatura). */
int equipe_apta(struct base *base, struct missao *missao, mundo *m, struct cjto_t *hab_base) {
    (void)hab_base; // evita warning: parâmetro intencionalmente não usado

    if (!base || !missao || !m) return 0;
    if (cjto_card(base->presentes) == 0) return 0;

    struct cjto_t *habilidades_equipe = cjto_cria(N_HABILIDADES);
    if (!habilidades_equipe) return 0;

    for (int id = 0; id < base->presentes->cap; id++) {
        if (base->presentes->flag[id]) {
            struct heroi *h = m->herois[id];
            if (h && h->vivo) {
                struct cjto_t *temp = cjto_uniao(habilidades_equipe, h->habilidades);
                cjto_destroi(habilidades_equipe);
                habilidades_equipe = temp;
            }
        }
    }

    int apta = cjto_contem(habilidades_equipe, missao->requisitos);

    cjto_destroi(habilidades_equipe);
    return apta;
}

/* Retorna o herói mais experiente presente na base */
struct heroi *heroi_mais_experiente(struct base *base, mundo *m) {
    if (!base || !base->presentes || !m) return NULL;

    struct heroi *mais_exp = NULL;
    int max_exp = -1;

    for (int i = 0; i < base->presentes->cap; i++) {
        if (base->presentes->flag[i]) {
            struct heroi *h = m->herois[i];
            if (h && h->vivo && h->experiencia > max_exp) {
                max_exp = h->experiencia;
                mais_exp = h;
            }
        }
    }

    return mais_exp;
}

/* Incrementa experiência dos heróis presentes, exceto 'exceto' (se não NULL) */
void incrementa_experiencia_equipe(struct base *base, struct heroi *exceto, mundo *m) {
    if (!base || !base->presentes || !m) return;

    for (int i = 0; i < base->presentes->cap; i++) {
        if (base->presentes->flag[i]) {
            struct heroi *h = m->herois[i];
            if (h && h->vivo && (exceto == NULL || h->id != exceto->id)) {
                h->experiencia++;
            }
        }
    }
}

struct evento *cria_evento(int t, struct heroi *h, struct base *b, int type, struct missao *mi) {
    struct evento *ev = malloc(sizeof(struct evento));
    if (!ev) return NULL;

    ev->tempo = t;
    ev->h = h;
    ev->b = b;
    ev->tipo = type;
    ev->mi = mi;

    return ev;
}


/* --- Handlers de eventos --- */

void chega(struct evento *chega, mundo *m) {
    if (!chega || !chega->h || !chega->h->vivo) return;
    m->eventos++;
    chega->h->base = chega->b ? chega->b->id : -1;

    /* condição espera_evento (corrigida): usar cjto_card e fila_tamanho */
    int espera_evento = 0;
    if (chega->b) {
        espera_evento = ((cjto_card(chega->b->presentes) < chega->b->lotacao) &&
                         (fila_tamanho(chega->b->espera) == 0))
                        || (chega->h->paciencia > 10 * fila_tamanho(chega->b->espera));
    }

    struct evento *ev = NULL;
    if (espera_evento) {
        printf("%d: CHEGA HEROI %d BASE %d (%d/%d) ESPERA\n",
               chega->tempo, chega->h->id, chega->b->id,
               cjto_card(chega->b->presentes), chega->b->lotacao);
        ev = cria_evento(chega->tempo, chega->h, chega->b, 2, NULL);
    } else {
        printf("%d: CHEGA HEROI %d BASE %d (%d/%d) DESISTE\n",
               chega->tempo, chega->h->id, chega->b->id,
               cjto_card(chega->b->presentes), chega->b->lotacao);
        ev = cria_evento(chega->tempo, chega->h, chega->b, 3, NULL);
    }

    if (ev) fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}

void espera(struct evento *espera, mundo *m) {
    if (!espera || !espera->h || espera->h->vivo == 0 || !espera->b) return;
    m->eventos++;
    printf("%d: HEROI %d ESPERA BASE %d (%2d)\n",
           espera->tempo, espera->h->id, espera->b->id, fila_tamanho(espera->b->espera));

    /* INSERE NA FILA O ID DO HEROI (int) - não o ponteiro */
    fila_insere(espera->b->espera, espera->h->id);
    if (fila_tamanho(espera->b->espera) > espera->b->filamax)
        espera->b->filamax = fila_tamanho(espera->b->espera);

    struct evento *ev = cria_evento(espera->tempo, NULL, espera->b, 4, NULL);
    if (ev) fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}

void desiste(struct evento *desiste, mundo *m) {
    if (!desiste || !desiste->h || desiste->h->vivo == 0) return;
    m->eventos++;
    int indice_b = desiste->b ? desiste->b->id : -1;

    if (m->Nbases <= 1) return;

    int indice_destino = rand() % (m->Nbases - 1);
    if (indice_destino >= indice_b) indice_destino += 1;

    struct base *D = m->bases[indice_destino];

    printf("%d: HEROI %d DESISTE BASE %d\n", desiste->tempo, desiste->h->id, desiste->b->id);

    struct evento *ev = cria_evento(desiste->tempo, desiste->h, D, 7, NULL);
    if (ev) fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}

void avisa(struct evento *avisa, mundo *m) {
    if (!avisa || !avisa->b) return;
    m->eventos++;
    printf("%d: AVISA PORTEIRO BASE %d (%d/%d) FILA [ ",
           avisa->tempo, avisa->b->id, cjto_card(avisa->b->presentes), avisa->b->lotacao);
    fila_imprime(avisa->b->espera);
    printf(" ]\n");

    /* Enquanto houver vagas e itens na fila (fila guarda IDs) */
    while (cjto_card(avisa->b->presentes) < avisa->b->lotacao && fila_tamanho(avisa->b->espera) > 0) {
        int id_heroi = -1;
        if (!fila_retira(avisa->b->espera, &id_heroi)) break;
        if (id_heroi < 0 || id_heroi >= m->Nherois) continue;
        struct heroi *h = m->herois[id_heroi];
        if (!h || h->vivo == 0) continue;

        cjto_insere(avisa->b->presentes, h->id);
        printf("%d: AVISA PORTEIRO BASE %d ADMITE %d.\n", avisa->tempo, avisa->b->id, h->id);

        struct evento *ev = cria_evento(avisa->tempo, h, avisa->b, 5, NULL);
        if (ev) fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
    }
}

void entra(struct evento *entra, mundo *m) {
    if (!entra || !entra->h || entra->h->vivo == 0) return;
    m->eventos++;

    int aleatorio = rand() % 20 + 1;
    int TPB = 15 + entra->h->paciencia * aleatorio;

    printf("%d: ENTRA HEROI %d BASE %d (%d/%d) SAI %d\n",
           entra->tempo, entra->h->id, entra->b->id,
           cjto_card(entra->b->presentes), entra->b->lotacao, entra->tempo + TPB);

    struct evento *ev = cria_evento(entra->tempo + TPB, entra->h, entra->b, 6, NULL);
    if (ev) fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}

void sai(struct evento *sai, mundo *m) {
    if (!sai || !sai->h || sai->h->vivo == 0) return;
    m->eventos++;

    /* retira do conjunto de presentes */
    cjto_retira(sai->b->presentes, sai->h->id);

    /* escolhe base destino aleatória (pode ser mesma) */
    int indice_destino = rand() % m->Nbases;
    struct base *D = m->bases[indice_destino];

    printf("%d: HEROI %d SAI DA BASE %d (%d/%d)\n",
           sai->tempo, sai->h->id, sai->b->id, cjto_card(sai->b->presentes), sai->b->lotacao);

    /* evento VIAJA */
    struct evento *ev_viaja = cria_evento(sai->tempo, sai->h, D, 7, NULL);
    if (ev_viaja) fprio_insere(m->LEF, ev_viaja, ev_viaja->tipo, ev_viaja->tempo);

    /* evento AVISA para a base original */
    struct evento *ev_avisa = cria_evento(sai->tempo, NULL, sai->b, 4, NULL);
    if (ev_avisa) fprio_insere(m->LEF, ev_avisa, ev_avisa->tipo, ev_avisa->tempo);
}

void viaja(struct evento *viaja, mundo *m) {
    if (!viaja || !viaja->h || viaja->h->vivo == 0) return;
    m->eventos++;

    struct base *B_atual = NULL;

    if (viaja->h->base >= 0 && viaja->h->base < m->Nbases) {
        B_atual = m->bases[viaja->h->base];
    } else {
        static struct base origem = { .local = {0,0}, .presentes = NULL, .espera = NULL, .lotacao = 0, .id = -1, .missoes = 0, .filamax = 0 };
        B_atual = &origem;
    }

    int dx = viaja->b->local.x - B_atual->local.x;
    int dy = viaja->b->local.y - B_atual->local.y;
    double distancia = sqrt((double)dx*dx + (double)dy*dy);

    int duracao = (int) ceil(distancia / (double) viaja->h->velocidade);
    if (duracao < 0) duracao = 0;

    printf("%d: HEROI %d VIAJA DE BASE %d PARA BASE %d, DIST %d VEL %d CHEGA %d.\n",
           viaja->tempo, viaja->h->id,
           B_atual->id, viaja->b->id,
           (int)distancia, viaja->h->velocidade, viaja->tempo + duracao);

    struct evento *ev_chega = cria_evento(viaja->tempo + duracao, viaja->h, viaja->b, 1, NULL);
    if (ev_chega) fprio_insere(m->LEF, ev_chega, ev_chega->tipo, ev_chega->tempo);
}

void morre(struct evento *morre, mundo *m) {
    if (!morre || !morre->h) return;
    m->eventos++;
    cjto_retira(morre->b->presentes, morre->h->id);
    m->mortos++;
    morre->h->vivo = 0;

    printf("%d: HEROI %d MORRE MISSAO %d.\n", morre->tempo, morre->h->id, morre->mi ? morre->mi->id : -1);

    struct evento *ev = cria_evento(morre->tempo, NULL, morre->b, 4, NULL);
    if (ev) fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}

void missao(struct evento *mev, mundo *m) {
    if (!mev || !mev->mi) return;
    mev->mi->tentativas++;
    m->eventos++;
    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", mev->tempo, mev->mi->id, mev->mi->tentativas);
    cjto_imprime(mev->mi->requisitos);
    printf(" ]\n");

    /* encontra base mais próxima apta */
    struct base *base_mais_proxima_apta = NULL;
    int menor_distancia_apta = INT_MAX_LOCAL;

    for (int i = 0; i < m->Nbases; i++) {
        struct base *b = m->bases[i];
        if (!b) continue;
        int dist = calcula_distancia(b->local, mev->mi->local);
        if (dist < menor_distancia_apta && equipe_apta(b, mev->mi, m, NULL)) {
            menor_distancia_apta = dist;
            base_mais_proxima_apta = b;
        }
    }

    if (base_mais_proxima_apta != NULL) {
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS [ ",
               mev->tempo, mev->mi->id, base_mais_proxima_apta->id);
        /* recalcula habilidades da equipe apenas para imprimir */
        struct cjto_t *hab_equ = cjto_cria(N_HABILIDADES);
        for (int id = 0; id < base_mais_proxima_apta->presentes->cap; id++) {
            if (base_mais_proxima_apta->presentes->flag[id]) {
                struct heroi *h = m->herois[id];
                if (h && h->vivo) {
                    struct cjto_t *tmp = cjto_uniao(hab_equ, h->habilidades);
                    cjto_destroi(hab_equ);
                    hab_equ = tmp;
                }
            }
        }
        cjto_imprime(hab_equ);
        printf(" ]\n");
        cjto_destroi(hab_equ);

        base_mais_proxima_apta->missoes++;
        mev->mi->status = 1;
        m->missoes_completas++;
        incrementa_experiencia_equipe(base_mais_proxima_apta, NULL, m);
        return;
    }

    /* nenhuma equipe apta - tentar Composto V */
    struct base *base_mais_proxima = NULL;
    int menor_distancia = INT_MAX_LOCAL;
    for (int i = 0; i < m->Nbases; i++) {
        struct base *b = m->bases[i];
        if (!b) continue;
        int dist = calcula_distancia(b->local, mev->mi->local);
        if (dist < menor_distancia && cjto_card(b->presentes) > 0) {
            menor_distancia = dist;
            base_mais_proxima = b;
        }
    }

    if (m->NCompostosV > 0 && (mev->tempo % 2500 == 0) && base_mais_proxima != NULL) {
        struct heroi *heroi_exp = heroi_mais_experiente(base_mais_proxima, m);
        if (heroi_exp != NULL) {
            printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROI %d COMPOSTO V\n",
                   mev->tempo, mev->mi->id, base_mais_proxima->id, heroi_exp->id);
            m->NCompostosV--;
            mev->mi->status = 1;
            m->missoes_completas++;
            base_mais_proxima->missoes++;
            incrementa_experiencia_equipe(base_mais_proxima, heroi_exp, m);

            struct evento *ev = cria_evento(mev->tempo, heroi_exp, base_mais_proxima, 5, mev->mi);
            if (ev) fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
            return;
        }
    }

    /* missão impossível por agora - reagenda para +1440 */
    printf("%6d: MISSAO %d IMPOSSIVEL\n", mev->tempo, mev->mi->id);
    mev->mi->status = 0;
    mev->mi->tentativas++;
    struct evento *ev = cria_evento(mev->tempo + 1440, NULL, NULL, 9, mev->mi);
    if (ev) fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}

void fim(struct evento *fim, mundo *m) {
    if (!fim || !m) return;
    printf("%6d: FIM\n", fim->tempo);

    for (int i = 0; i < m->Nherois; i++) {
        struct heroi *h = m->herois[i];
        if (!h) continue;
        if (h->vivo) {
            printf("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS [ ", h->id, h->paciencia, h->velocidade, h->experiencia);
        } else {
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [ ", h->id, h->paciencia, h->velocidade, h->experiencia);
        }
        cjto_imprime(h->habilidades);
        printf("]\n");
    }

    for (int i = 0; i < m->Nbases; i++) {
        struct base *b = m->bases[i];
        if (!b) continue;
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", b->id, b->lotacao, b->filamax, b->missoes);
    }

    printf("EVENTOS TRATADOS: %d\n", m->eventos);
    float porcentagem = 0.0f;
    if (m->Nmissoes > 0) porcentagem = ((float)m->missoes_completas / (float)m->Nmissoes) * 100.0f;
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", m->missoes_completas, m->Nmissoes, porcentagem);

    int minimo_missao = min_missao(m);
    int maximo_missao = max_missao(m);
    float media = media_missao(m);
    printf("TENTATIVA / MISSAO: MIN %d MAX %d MEDIA %.1f\n", minimo_missao, maximo_missao, media);

    float mortalidade = 0.0f;
    if (m->Nherois > 0) mortalidade = ((float)m->mortos / (float)m->Nherois) * 100.0f;
    printf("TAXA DE MORTALIDADE: %.1f%%\n", mortalidade);
}
