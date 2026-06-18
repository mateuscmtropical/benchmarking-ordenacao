#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "ordenacao.h"

/* ================================================================== */
/*  FUNCOES AUXILIARES DE GERACAO DE DADOS                            */
/* ================================================================== */
void gerarVetorAleatorio(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = rand() % 100000;
}

void gerarVetorOrdenado(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = i;
}

void gerarVetorInverso(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = n - i;
}

/* Vetor quase ordenado: ordenado e depois 'desordens' trocas de vizinhos. */
void gerarVetorQuaseOrdenado(int *v, int n, int desordens) {
    gerarVetorOrdenado(v, n);
    for (int k = 0; k < desordens; k++) {
        int i = rand() % (n - 1);   /* troca v[i] com o vizinho imediato v[i+1] */
        int tmp = v[i];
        v[i] = v[i + 1];
        v[i + 1] = tmp;
    }
}

/* ================================================================== */
/*  INFRAESTRUTURA DE MEDICAO                                         */
/* ================================================================== */
typedef void (*FuncOrd)(int *, int, Metricas *);

static int *copiarVetor(const int *base, int n) {
    int *v = (int *)malloc(n * sizeof(int));
    if (v == NULL) { fprintf(stderr, "Falha de malloc\n"); exit(1); }
    memcpy(v, base, n * sizeof(int));
    return v;
}

static int estaOrdenado(const int *v, int n) {
    for (int i = 1; i < n; i++) if (v[i - 1] > v[i]) return 0;
    return 1;
}

/*
 * Executa 'f' sobre uma COPIA de 'base' (o vetor original e preservado),
 * cronometra com clock_gettime(CLOCK_MONOTONIC) e devolve o tempo em segundos.
 * As metricas (comparacoes/trocas) ficam em *m.
 */
static double executar(FuncOrd f, const int *base, int n, Metricas *m) {
    int *v = copiarVetor(base, n);
    struct timespec inicio, fim;

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    f(v, n, m);
    clock_gettime(CLOCK_MONOTONIC, &fim);

    double t = (fim.tv_sec - inicio.tv_sec)
             + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    if (!estaOrdenado(v, n)) {
        fprintf(stderr, "[ERRO] vetor NAO ficou ordenado (n=%d)!\n", n);
    }
    free(v);
    m->tempo_segundos = t;
    return t;
}

/* ================================================================== */
/*  PROBLEMA 1: custo teorico vs tempo real                          */
/*  Bubble (otim.), Insertion, Selection | N=30000 | 3 cenarios      */
/* ================================================================== */
void problema1(void) {
    const int N = 30000;
    printf("\n================ PROBLEMA 1 (N = %d) ================\n", N);

    int *aleatorio = (int *)malloc(N * sizeof(int));
    int *ordenado  = (int *)malloc(N * sizeof(int));
    int *inverso   = (int *)malloc(N * sizeof(int));

    srand(42);                       /* semente fixa -> mesmos dados para todos */
    gerarVetorAleatorio(aleatorio, N);
    gerarVetorOrdenado(ordenado, N);
    gerarVetorInverso(inverso, N);

    struct { const char *nome; FuncOrd f; } algos[] = {
        { "Bubblesort",    bubbleSort    },
        { "Insertionsort", insertionSort },
        { "Selectionsort", selectionSort },
    };
    struct { const char *nome; int *v; } cenarios[] = {
        { "Aleatorio",        aleatorio },
        { "Ordenado(melhor)", ordenado  },
        { "Inverso(pior)",    inverso   },
    };

    FILE *csv = fopen("data/problema1.csv", "w");
    if (csv) fprintf(csv, "Algoritmo,Cenario,Tempo_s,Comparacoes,Trocas\n");

    printf("%-15s %-18s %12s %16s %16s\n",
           "Algoritmo", "Cenario", "Tempo(s)", "Comparacoes", "Trocas");
    printf("--------------------------------------------------------------------------------\n");

    for (int a = 0; a < 3; a++) {
        for (int c = 0; c < 3; c++) {
            Metricas m;
            double t = executar(algos[a].f, cenarios[c].v, N, &m);
            printf("%-15s %-18s %12.6f %16llu %16llu\n",
                   algos[a].nome, cenarios[c].nome, t, m.comparacoes, m.trocas);
            if (csv) fprintf(csv, "%s,%s,%.6f,%llu,%llu\n",
                             algos[a].nome, cenarios[c].nome, t, m.comparacoes, m.trocas);
        }
        printf("--------------------------------------------------------------------------------\n");
    }

    if (csv) fclose(csv);
    free(aleatorio); free(ordenado); free(inverso);
}

/* ================================================================== */
/*  PROBLEMA 2: Quicksort classico vs Heapsort                       */
/*  N=5000 | vetor ja ordenado | 10 execucoes -> media e desvio      */
/* ================================================================== */
void problema2(void) {
    const int N = 5000;
    const int REP = 10;
    printf("\n================ PROBLEMA 2 (N = %d, %d execucoes) ================\n", N, REP);

    int *ordenado = (int *)malloc(N * sizeof(int));
    gerarVetorOrdenado(ordenado, N);   /* caso degenerado para o Quicksort */

    struct { const char *nome; FuncOrd f; } algos[] = {
        { "Heapsort",  heapSort  },
        { "Quicksort", quickSort },
    };

    FILE *runs = fopen("data/problema2_execucoes.csv", "w");
    if (runs) fprintf(runs, "Algoritmo,Execucao,Tempo_s\n");
    FILE *resumo = fopen("data/problema2_resumo.csv", "w");
    if (resumo) fprintf(resumo, "Algoritmo,Media_s,DesvioPadrao_s,Comparacoes,Trocas\n");

    for (int a = 0; a < 2; a++) {
        double tempos[REP];
        Metricas m;
        for (int r = 0; r < REP; r++) {
            tempos[r] = executar(algos[a].f, ordenado, N, &m);
            if (runs) fprintf(runs, "%s,%d,%.6f\n", algos[a].nome, r + 1, tempos[r]);
        }
        /* media */
        double soma = 0.0;
        for (int r = 0; r < REP; r++) soma += tempos[r];
        double media = soma / REP;
        /* desvio padrao amostral (denominador REP-1) */
        double var = 0.0;
        for (int r = 0; r < REP; r++) var += (tempos[r] - media) * (tempos[r] - media);
        double desvio = sqrt(var / (REP - 1));

        printf("%-12s media = %.6f s | desvio = %.6f s | comparacoes = %llu | trocas = %llu\n",
               algos[a].nome, media, desvio, m.comparacoes, m.trocas);
        if (resumo) fprintf(resumo, "%s,%.6f,%.6f,%llu,%llu\n",
                            algos[a].nome, media, desvio, m.comparacoes, m.trocas);
    }

    if (runs) fclose(runs);
    if (resumo) fclose(resumo);
    free(ordenado);
}

/* ================================================================== */
/*  PROBLEMA 3: Insertionsort tradicional vs Shellsort               */
/*  N=50000 | vetor quase ordenado (0,5% de vizinhos trocados)       */
/* ================================================================== */
void problema3(void) {
    const int N = 50000;
    const int DESORDENS = (int)(N * 0.005);   /* 0,5% = 250 trocas de vizinhos */
    printf("\n================ PROBLEMA 3 (N = %d, %d desordens) ================\n", N, DESORDENS);

    int *quase = (int *)malloc(N * sizeof(int));
    srand(42);                                 /* reproducibilidade */
    gerarVetorQuaseOrdenado(quase, N, DESORDENS);

    struct { const char *nome; FuncOrd f; } algos[] = {
        { "Insertionsort", insertionSort },
        { "Shellsort",     shellSort     },
    };

    FILE *csv = fopen("data/problema3.csv", "w");
    if (csv) fprintf(csv, "Algoritmo,Tempo_s,Comparacoes,Trocas\n");

    printf("%-15s %12s %16s %16s\n", "Algoritmo", "Tempo(s)", "Comparacoes", "Trocas");
    printf("------------------------------------------------------------\n");
    for (int a = 0; a < 2; a++) {
        Metricas m;
        double t = executar(algos[a].f, quase, N, &m);
        printf("%-15s %12.6f %16llu %16llu\n", algos[a].nome, t, m.comparacoes, m.trocas);
        if (csv) fprintf(csv, "%s,%.6f,%llu,%llu\n", algos[a].nome, t, m.comparacoes, m.trocas);
    }

    if (csv) fclose(csv);
    free(quase);
}

int main(void) {
    printf("Benchmarking de Algoritmos de Ordenacao\n");
    problema1();
    problema2();
    problema3();
    printf("\nConcluido. CSVs gravados em data/.\n");
    return 0;
}
