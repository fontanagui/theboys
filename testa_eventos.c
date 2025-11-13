#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mundo.h"
#include "conjunto.h"
#include "fprio.h"
#include "fila.h"
#include "eventos.h"

int main(void) {
    srand((unsigned)time(NULL));

    printf("=== TESTE DO SISTEMA DE EVENTOS ===\n");

    // cria mundo (assume que cria_mundo faz alocações corretas)
    mundo *m = cria_mundo();
    if (!m) {
        fprintf(stderr, "Erro ao criar mundo\n");
        return 1;
    }

    // Ajustes mínimos para que eventos façam sentido:
    // - garante que o heroi 0 exista e esteja vivo
    // - coloca o herói 0 como presente na base 0 (se houver)
    if (m->Nherois > 0 && m->Nbases > 0) {
        struct heroi *h0 = m->herois[0];
        struct base *b0 = m->bases[0];
        if (h0 && b0) {
            h0->vivo = 1;
            h0->base = b0->id;
            cjto_insere(b0->presentes, h0->id);    // marca presente
        }
    }

    // Agenda alguns eventos iniciais:
    // 1) Evento CHEGA (tipo 1) do herói 0 na base 0 no tempo 0
    if (m->Nherois > 0 && m->Nbases > 0) {
        struct evento *e_chega = cria_evento(0, m->herois[0], m->bases[0], 1, NULL);
        fprio_insere(m->LEF, e_chega, e_chega->tipo, e_chega->tempo);
    }

    // 2) Agenda uma missão (tipo 9) usando a missão 0, tempo 10
    if (m->Nmissoes > 0) {
        struct evento *e_mis = cria_evento(10, NULL, NULL, 9, m->missao[0]);
        fprio_insere(m->LEF, e_mis, e_mis->tipo, e_mis->tempo);
    }

    // 3) Agenda um AVISA (tipo 4) para a base 0 no tempo 5
    if (m->Nbases > 0) {
        struct evento *e_avisa = cria_evento(5, NULL, m->bases[0], 4, NULL);
        fprio_insere(m->LEF, e_avisa, e_avisa->tipo, e_avisa->tempo);
    }

    // 4) Agenda FIM (tipo 10) no tempo 1000
    struct evento *e_fim = cria_evento(1000, NULL, NULL, 10, NULL);
    fprio_insere(m->LEF, e_fim, e_fim->tipo, e_fim->tempo);

    // Loop principal: retira evento mais próximo e executa handler
    while (1) {
        int tipo, prio;
        struct evento *ev = (struct evento *) fprio_retira(m->LEF, &tipo, &prio);
        if (!ev) {
            // fila vazia -> fim do teste
            printf("LEF vazia — fim do processamento de eventos.\n");
            break;
        }

        // atualiza relógio do mundo
        m->relogio = ev->tempo;

        // chama handler conforme tipo (use ev->tipo por segurança)
        switch (ev->tipo) {
            case 1: // CHEGA
                chega(ev, m);
                break;
            case 2: // ESPERA
                espera(ev, m);
                break;
            case 3: // DESISTE
                desiste(ev, m);
                break;
            case 4: // AVISA
                avisa(ev, m);
                break;
            case 5: // ENTRA
                entra(ev, m);
                break;
            case 6: // SAI
                sai(ev, m);
                break;
            case 7: // VIAJA
                viaja(ev, m);
                break;
            case 8: // MORRE
                morre(ev, m);
                break;
            case 9: // MISSAO
                missao(ev, m);
                break;
            case 10: // FIM
                fim(ev, m);
                // libera o evento atual e sai do loop
                free(ev);
                goto finish;
            default:
                printf("Evento desconhecido tipo=%d tempo=%d\n", ev->tipo, ev->tempo);
                break;
        }

        // liberamos o evento processado (handlers criam novos eventos quando necessário)
        free(ev);
    }

finish:
    // destrói mundo e termina teste
    destroi_mundo(m);
    printf("Teste finalizado — mundo destruído.\n");
    return 0;
}
