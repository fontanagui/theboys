#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mundo.h"
#include "conjunto.h"
#include "fprio.h"
#include "fila.h"
#include "eventos.h"

#define N_HABILIDADES 10
#define INT_MAX 100000

 struct evento *cria_evento (int t , struct heroi *h, struct base *b, int type, struct missao *mi) {
    struct evento *ev = malloc(sizeof(struct evento));
    if (!ev) return;

    ev->tempo = t;
    ev->h = h;
    ev->b = b;           
    ev->tipo= type; 
    ev->mi = mi;

    return ev;
 }

// Função auxiliar para calcular distância entre dois pontos
int calcula_distancia(struct coordenada p1, struct coordenada p2) {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return (int)sqrt(dx * dx + dy * dy);
}

// Retorna 1 se a equipe da base tem todas as habilidades requeridas para a missão
int equipe_apta(struct base *base, struct missao *missao,  mundo *m) {
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

// Retorna o herói mais experiente presente na base
struct heroi *heroi_mais_experiente( struct base *base,  mundo *m) {
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

// Incrementa a experiência de todos os heróis presentes na base, exceto 'exceto'
void incrementa_experiencia_equipe(struct base *base, struct heroi *exceto,  mundo *m) {
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


void chega( struct evento *chega, mundo *m) {
    if (!chega  || !chega->h->vivo) return;
    m->eventos++;
    chega->h->base = chega->b->id;
    printf("%d: CHEGA HEROI %d BASE %d.\n", chega->tempo, chega->h->id, chega->b->id);

    int espera_evento = ((cjt_card(chega->b->presentes) < chega-> b->lotacao) && fila_vazia(chega->b->espera)) ||
                        (chega->h->paciencia > 10 * chega->b->espera->num);

    struct evento *ev = malloc(sizeof(struct evento *) );
    if (espera_evento){
       ev =  cria_evento (chega->tempo,chega->h,chega->b,2, NULL);}
    else {
       ev = cria_evento (chega->tempo,chega->h,chega->b,3, NULL);
    }
    
    fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}

void espera(struct evento *espera, mundo*m) {
    if (!espera || (!espera->h->vivo)) return;
    m->eventos++;
    fila_insere(espera->b->espera, espera->h);
    if (fila_tamanho(espera->b->espera)>espera->b->filamax) 
        espera->b->filamax= fila_tamanho(espera->b->espera);
    printf("%d: HEROI %d ENTRA NA FILA DE ESPERA DA BASE %d.\n", espera->tempo, espera->h->id, espera->b->id);

    struct evento *ev = malloc(sizeof(struct evento *));
    if (!ev) return;
    ev = cria_evento (espera->tempo,NULL,espera->b,4, NULL);

    fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}



void desiste(struct evento *desiste, mundo *m) {
    if (!desiste ||(desiste->h->vivo==0)) return;
    m->eventos++;
    int indice_b = desiste-> b->id;
    int indice_destino;

    // gera índice aleatório diferente do índice atual
    if (m->Nbases <= 1) return;  // não há outra base

    indice_destino = rand() % (m->Nbases - 1);
    if (indice_destino >= indice_b) {
        indice_destino += 1;
    }

    struct base *D = &m->bases[indice_destino];

    printf("%d: HEROI %d DESISTE DA BASE %d E VAI PARA BASE %d.\n", desiste->tempo,desiste-> h->id, desiste-> b->id, D->id);

    // Cria evento VIAJA
    struct evento *ev = malloc(sizeof(struct evento*));
    if (!ev) return;
    ev =cria_evento(desiste->tempo,desiste->h,D,7,NULL);
    fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}   

void avisa(struct evento *avisa,mundo *m) {
    m->eventos++;
    struct heroi *h;

    // Enquanto houver vagas e heróis na fila
    while (cjto_card(avisa->b->presentes) < avisa->b->lotacao && !fila_vazia(avisa->b->espera) ) {
        h = fila_retira(avisa->b->espera);       // retira primeiro herói da fila
        if (!h || (h->vivo==0)) continue;
        cjto_insere(avisa->b->presentes, h->id); // adiciona ao conjunto de presentes

        printf("%d: HEROI %d ENTRA NA BASE %d.\n", avisa->tempo, h->id, avisa->b->id);

        // Cria evento ENTRA
        struct evento *ev = malloc(sizeof(struct evento *));
        if (!ev) return ;
        ev=cria_evento(avisa->tempo,h,avisa->b,5,NULL);
        fprio_insere(LEF, ev, ev->tipo, ev->tempo);
    }
}


void entra(struct evento *entra, mundo *m) {
    m->eventos++;
    if (!entra ||(!entra->h->vivo)) return;
    // Calcula tempo de permanência na base
    int aleatorio = rand() % 20 + 1;  // número aleatório entre 1 e 20
    int TPB = 15 + entra->h->paciencia * aleatorio;

    printf("%d: HEROI %d ENTRA NA BASE %d POR %d UNIDADES DE TEMPO.\n", entra->tempo, entra->h->id, entra->b->id, TPB);

    // Cria evento SAI
    struct evento *ev = malloc(sizeof(struct evento *));
    if (!ev) return;

    ev->tempo = entra->tempo + TPB; // momento em que o herói sai
    ev->h =entra->h;
    ev->b = entra->b;
    ev->tipo = 6;  // SAI
    ev->mi = NULL;
    ev=cria_evento((entra->tempo+TPB),entra->h, entra->b, 6, NULL);
    fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}

void sai(struct evento *sai, mundo *m) {
    m->eventos++;
    if (!sai || sai->h->vivo==0) return;
    // Remove o herói do conjunto de presentes da base
    cjto_retira(sai->b->presentes, sai-> h->id);

    // Escolhe uma base destino aleatória (pode ser a mesma)
    int indice_destino = rand() % m->Nbases;
    struct base *D = &m->bases[indice_destino];

    printf("%d: HEROI %d SAI DA BASE %d E VIAJA PARA BASE %d.\n", sai->tempo, sai->h->id, sai->b->id, D);

    // Cria evento VIAJA
    struct evento *ev_viaja = malloc(sizeof(struct evento* ));
    if (ev_viaja) {
        ev_viaja=cria_evento(sai->tempo,sai->h,D,7,NULL);
        fprio_insere(m->LEF, ev_viaja, ev_viaja->tipo, ev_viaja->tempo);

    }

    // Cria evento AVISA para a base original
     struct evento *ev_avisa = malloc(sizeof(struct evento*));
    if (ev_avisa) {
        ev_avisa=cria_evento(sai->tempo,NULL,sai->b,4,NULL);
        fprio_insere(m->LEF, ev_avisa, ev_avisa->tipo, ev_avisa->tempo);
    }
}



void viaja(struct evento *viaja,mundo*m) {
    if (!viaja|| (viaja->h->vivo==0)) return;
    m->eventos++;
    struct base *B_atual = NULL;

    // Se o herói tem uma base atual válida, pega a base
    if (viaja->h->base >= 0 && viaja->h->base < m->Nbases) {
        B_atual = &m->bases[viaja->h->base];
    } else {
        // Caso não tenha base, assume origem (0,0)
        static struct base origem = { .local = {0,0} };
        B_atual = &origem;
    }

    // Calcula distância cartesiana
    int dx = viaja->b->local.x - B_atual->local.x;
    int dy =viaja->b->local.y - B_atual->local.y;
    double distancia = sqrt(dx*dx + dy*dy);

    // Calcula duração da viagem (tempo = distância / velocidade)
    int duracao = (int) ceil(distancia / viaja-> h->velocidade);

    printf("%d: HEROI %d VIAJA DE BASE %d PARA BASE %d, DURACAO %d.\n",
           viaja->tempo, viaja->h->id, B_atual->id, viaja->b->id, duracao);

    // Cria evento CHEGA

    struct evento *ev_chega = malloc(sizeof(struct evento*));
    if (ev_chega) {
        ev_chega=cria_evento(viaja->tempo+duracao,viaja->h,viaja->b,1,NULL);
        fprio_insere(m->LEF, ev_chega, ev_chega->tipo, ev_chega->tempo);
    }
}


  


void morre(struct evento *morre, mundo *m) {
    if (!morre) return;
    m->eventos++;
    // Retira herói do conjunto de presentes da base
    cjto_retira(morre->b->presentes, morre->h->id);
    m->mortos++;
    // Marca o herói como morto
    morre->h->vivo = 0;

    printf("%d: HEROI %d MORRE NA BASE %d.\n", morre->tempo, morre->h->id, morre->b->id);

    // Cria evento AVISA para a base
    struct evento *ev = malloc(sizeof(struct evento*));
    if (!ev) return;
    ev=cria_evento(morre->tempo,NULL,morre->b,4,NULL);
    fprio_insere(LEF, ev, ev->tipo, ev->tempo);
}

// Função principal do evento MISSAO
void missao( struct evento *missao,  mundo *m ) {
    missao->mi->tentativas++;
    m->eventos++;
    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", 
           missao->tempo, missao->mi->id, missao->mi->tentativas);
    cjto_imprime(missao->mi->requisitos);
    printf(" ]\n");
    
    // Calcula distância de cada base ao local da missão
    struct base *base_mais_proxima_apta = NULL;
    int menor_distancia_apta = INT_MAX;
    
    for (int i = 0; i < m->Nbases; i++) {
        int dist = calcula_distancia(m->bases[i]->local, missao->mi->local);
        
        // Verifica se é a base mais próxima E se está apta
        if (dist < menor_distancia_apta && equipe_apta(m->bases[i], missao->mi,m)) {
            menor_distancia_apta = dist;
            base_mais_proxima_apta = m->bases[i];
        }
    }
    
    // Se encontrou uma base apta
    if (base_mais_proxima_apta != NULL) {
        printf("%6d: MISSAO %d CUMPRIDA BASE %d DIST %d\n", 
               missao->tempo, missao->mi->id, base_mais_proxima_apta->id, menor_distancia_apta);
        base_mais_proxima_apta->missoes++;
        missao->mi->status = 1; // Marca como cumprida
        m->missoes_completas++;
        incrementa_experiencia_equipe(base_mais_proxima_apta,NULL, m);
        m->missoes_completas++;
        return;
    }
    
    // Nenhuma equipe está apta - tenta usar Composto V
    
    // Encontra a base mais próxima (independente de estar apta)
    struct base *base_mais_proxima = NULL;
    int menor_distancia = INT_MAX;
    
    for (int i = 0; i < m->Nbases; i++) {
        int dist = calcula_distancia(m->bases[i]->local, missao->mi->local);
        if (dist < menor_distancia && fila_tamanho(m->bases[i]->presentes) > 0) {
            menor_distancia = dist;
            base_mais_proxima = m->bases[i];
        }
    }
    
    // Verifica se pode usar Composto V
    if (m->NCompostosV > 0 && (missao->tempo % 2500 == 0) && base_mais_proxima != NULL) {
        struct heroi *heroi_exp = heroi_mais_experiente(base_mais_proxima, m);
        
        if (heroi_exp != NULL) {
            printf("%6d: MISSAO %d CUMPRIDA BASE %d HEROI %d COMPOSTO V\n",
                   missao->tempo, missao->mi->id, base_mais_proxima->id, heroi_exp->id);
            
            m->NCompostosV--;
            missao->mi->status = 1; // Marca como cumprida
            m->missoes_completas++;
            base_mais_proxima->missoes++;
            // Incrementa experiência dos demais heróis (exceto o que morrerá)
            incrementa_experiencia_equipe(base_mais_proxima, heroi_exp, m);
            struct evento *ev = malloc(sizeof(struct evento*));
            if (!ev) return; 
            ev=cria_evento (missao->tempo,heroi_exp,base_mais_proxima,5,NULL);
            fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
            return;
}
        }
    
    
    // Missão impossível - adia para o dia seguinte
    printf("%6d: MISSAO %d IMPOSSIVEL\n", missao->tempo, missao->mi->id);
    
    missao->mi->status = 0; // Marca como impossível
    missao->mi->tentativas++;
   struct evento *ev = malloc(sizeof(struct evento *));
    if (!ev) return;
    ev=cria_evento(missao->tempo+1440,NULL,NULL,9,missao->mi);
    fprio_insere(m->LEF, ev, ev->tipo, ev->tempo);
}
    


void fim (struct evento *fim, mundo *m) {
     printf("%6d: FIM \n" , fim->tempo);

     for (int i=0;i<m->Nherois; i++) {
        struct heroi *h=m->herois[i];
        if (h->vivo){
            printf("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d  HABS [ " , h->id , h->paciencia, h->experiencia );
            cjto_imprime(h->habilidades);
            printf("]\n");}
        else
        {
            printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d  HABS [ " , h->id , h->paciencia, h->experiencia );
            cjto_imprime(h->habilidades);
            printf("]\n");} 
        } 
     for (int i=0;i<m->Nbases; i++) {
        struct base *b=m->bases[i];
        printf("BASE %2d LOT %2d FILA MAX  %2d MISSOES %d \n" , b->id , b->lotacao, b->filamax, b->missoes);
}
    printf("EVENTOS TRATADOS: %d \n" , m->eventos);
    printf("MISSOES CUMPRIDAS: %d/%d \n", m->missoes_completas, m->Nmissoes);
}
