// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
#include "mundo.h"
#include "eventos.h"

#define T_FINAL 525600  // 525600 minutos = 1 ano (365 dias)

int main() {
    // Inicializa gerador de números aleatórios
    srand(time(NULL));
    
    // Cria o mundo
    mundo *m = cria_mundo();
    if (!m) {
        fprintf(stderr, "Erro ao criar mundo!\n");
        return 1;
    }
    
    // Cria eventos CHEGA para todos os heróis
    for (int i = 0; i < m->Nherois; i++) {
        // Escolhe base aleatória
        struct base *b = m->bases[rand() % m->Nbases];
        
        // Tempo aleatório de chegada
        int t = rand() % 4321;
        
        // Cria evento CHEGA (tipo 1)
        struct evento *ev = cria_evento(t, m->herois[i], b, 1, NULL);
        if (ev) {
            fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
        }
    }
    
    // Cria eventos MISSAO para todas as missões
    for (int i = 0; i < m->Nmissoes; i++) {
        struct evento *ev = cria_evento(m->missao[i]->tempo, NULL, NULL, 9, m->missao[i]);
        if (ev) {
            fprio_insere(m->LEF, ev, 9, ev->tempo);
        }
    }
    
    // Cria evento FIM (tipo 10) - ESSENCIAL para parar o programa
    struct evento *ev_fim = cria_evento(T_FINAL, NULL, NULL, 10, NULL);
    if (ev_fim) {
        fprio_insere(m->LEF, ev_fim, 10, T_FINAL);
    }
    
    // Loop principal de simulação
    int type;
    while (m->relogio <= T_FINAL) {
        // Retira próximo evento da fila de prioridade
        struct evento *ev = fprio_retira(m->LEF, &type, &m->relogio);
        // Se não há mais eventos, encerra
        if (!ev) break;
        // Processa evento de acordo com seu tipo
        switch (type) {
            case 1:  // CHEGA
                chega(ev, m);
                break;
            
            case 2:  // ESPERA
                espera(ev, m);
                break;
            
            case 3:  // DESISTE
                desiste(ev, m);
                break;
            
            case 4:  // AVISA
                avisa(ev, m);
                break;
            
            case 5:  // ENTRA
                entra(ev, m);
                break;
            
            case 6:  // SAI
                sai(ev, m);
                break;
            
            case 7:  // VIAJA
                viaja(ev, m);
                break;
            
            case 8:  // MORRE
                morre(ev, m);
                break;
            
            case 9:  // MISSAO
                missao(ev, m);
                break;
            
            case 10:  // FIM
                fim(ev, m);
                free(ev);
                destroi_mundo(m);
                return 0;  // Encerra o programa
        }
        
        // Libera memória do evento processado
        free(ev);
    }
    
    // Se chegou aqui, acabaram os eventos antes do FIM
    destroi_mundo(m);
    return 0;
}