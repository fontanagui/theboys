# The Boys — Simulação por Lista de Eventos Futuros

Trabalho acadêmico da disciplina de **Programação 1** (DINF/UFPR), implementado em **C**. O projeto simula, por eventos discretos, um mundo com **heróis**, **bases** e **missões**, no qual heróis chegam a bases, esperam em filas, ganham experiência, viajam entre bases, cumprem missões e podem morrer, tudo controlado por um relógio de simulação e uma **Lista de Eventos Futuros (LEF)** implementada como fila de prioridade.

## 🧠 Sobre a simulação

O programa modela um cenário inspirado na série *The Boys*: heróis chegam a bases com determinada paciência, aguardam vaga (ou desistem), executam missões que exigem certas habilidades e podem viajar entre bases ou morrer ao longo do tempo.

A simulação é do tipo **orientada a eventos discretos**: cada acontecimento (chegada, espera, viagem, morte etc.) é representado por um evento com um horário associado, inserido na **Lista de Eventos Futuros (LEF)** — uma fila de prioridade ordenada pelo tempo. O programa retira repetidamente o evento mais próximo, avança o relógio da simulação para aquele instante e o processa, até atingir o tempo final (`T_FINAL = 525600` minutos, equivalente a 1 ano) ou a LEF ficar vazia.

### Tipos de evento processados

| # | Evento | Função |
|---|--------|--------|
| 1 | CHEGA | `chega()` — herói chega a uma base |
| 2 | ESPERA | `espera()` — herói entra na fila de espera |
| 3 | DESISTE | `desiste()` — herói desiste da espera |
| 4 | AVISA | `avisa()` — aviso de disponibilidade de vaga |
| 5 | ENTRA | `entra()` — herói entra na base |
| 6 | SAI | `sai()` — herói sai da base |
| 7 | VIAJA | `viaja()` — herói viaja entre bases |
| 8 | MORRE | `morre()` — herói morre |
| 9 | MISSAO | `missao()` — execução de uma missão |
| 10 | FIM | `fim()` — encerra a simulação |

### Principais estruturas de dados (`mundo.h`)

- **`struct heroi`**: id, paciência, velocidade, experiência, base atual, conjunto de habilidades e status de vida.
- **`struct base`**: id, lotação, conjunto de heróis presentes, fila de espera, coordenadas, missões completadas e tamanho máximo da fila.
- **`struct missao`**: id, conjunto de habilidades requeridas, coordenadas, status, tentativas e tempo.
- **`mundo`**: struct principal que agrega vetores de heróis, bases e missões, o relógio da simulação e a Lista de Eventos Futuros (`LEF`, campo `struct fprio_t *LEF`).

## 🔧 Requisitos

- GCC (ou outro compilador C compatível com C99)
- `make`
- (opcional) [Valgrind](https://valgrind.org/) para checagem de vazamentos de memória

## 🛠️ Compilando e executando

```bash
# compilar
make

# compilar e rodar
make run

# checar vazamentos de memória
make valgrind

# limpar arquivos gerados
make clean

# gerar pacote .tgz para entrega
make tgz
```

O `makefile` usa as flags `-Wall -Wextra -Werror -g -std=c99` e a lib `-lm`.

## 🧪 Testes

O repositório inclui testes unitários específicos para os módulos principais:

- `testa_mundo.c` — testa a criação/destruição do mundo, heróis, bases e missões.
- `testa_eventos.c` — testa a criação e o processamento dos eventos.

Compile-os individualmente (ex: `gcc testa_mundo.c mundo.c fila.c fprio.c conjunto.o -o testa_mundo -lm`) ou ajuste o `makefile` para incluí-los como alvos.

## 👤 Autoria

**Guilherme Fontana**

Trabalho desenvolvido para a disciplina de Programação 1 do curso de Ciência da Computação (DINF/UFPR), com o tema "The Boys" e implementação de uma simulação por Lista de Eventos Futuros.
