#include "ordenacao.h"

/* Troca dois inteiros de posicao e contabiliza UMA troca. */
static void trocar(int *vetor, int a, int b, Metricas *m) {
    int tmp = vetor[a];
    vetor[a] = vetor[b];
    vetor[b] = tmp;
    m->trocas++;
}

/* ------------------------------------------------------------------ */
/* 1) BUBBLESORT (otimizado)                                          */
/*    - Para quando uma passagem nao realiza nenhuma troca.           */
/*    - Encurta o limite ate a posicao da ultima troca da passagem.   */
/* ------------------------------------------------------------------ */
void bubbleSort(int *vetor, int n, Metricas *m) {
    m->comparacoes = 0;
    m->trocas = 0;

    int limite = n - 1;          /* indice ate onde ainda pode haver desordem */
    while (limite > 0) {
        int ultimaTroca = 0;     /* posicao da ultima troca desta passagem */
        for (int j = 0; j < limite; j++) {
            m->comparacoes++;
            if (vetor[j] > vetor[j + 1]) {
                trocar(vetor, j, j + 1, m);
                ultimaTroca = j;
            }
        }
        /* Tudo acima de ultimaTroca ja esta ordenado -> reduz o limite.
           Se nenhuma troca ocorreu, ultimaTroca = 0 e o laco termina. */
        limite = ultimaTroca;
    }
}

/* ------------------------------------------------------------------ */
/* 2) INSERTIONSORT (instrumentacao identica ao esqueleto do enunciado)*/
/* ------------------------------------------------------------------ */
void insertionSort(int *vetor, int n, Metricas *m) {
    m->comparacoes = 0;
    m->trocas = 0;

    for (int i = 1; i < n; i++) {
        int chave = vetor[i];
        int j = i - 1;

        while (j >= 0) {
            m->comparacoes++;             /* comparacao do 'if' conceitual do while */
            if (vetor[j] > chave) {
                vetor[j + 1] = vetor[j];
                m->trocas++;              /* conta o deslocamento/escrita */
                j--;
            } else {
                break;
            }
        }
        vetor[j + 1] = chave;             /* escrita de posicionamento: nao contabilizada */
    }
}

/* ------------------------------------------------------------------ */
/* 3) SELECTIONSORT                                                   */
/*    - Sempre n(n-1)/2 comparacoes (independe da ordem da entrada).  */
/*    - No maximo n-1 trocas (so troca quando o minimo nao e o i).    */
/* ------------------------------------------------------------------ */
void selectionSort(int *vetor, int n, Metricas *m) {
    m->comparacoes = 0;
    m->trocas = 0;

    for (int i = 0; i < n - 1; i++) {
        int indiceMin = i;
        for (int j = i + 1; j < n; j++) {
            m->comparacoes++;
            if (vetor[j] < vetor[indiceMin]) {
                indiceMin = j;
            }
        }
        if (indiceMin != i) {
            trocar(vetor, i, indiceMin, m);
        }
    }
}

/* ------------------------------------------------------------------ */
/* 4) SHELLSORT (sequencia de gaps de Shell: N/2, N/4, ..., 1)        */
/*    - Insertion com saltos de tamanho 'gap'.                        */
/*    - Mesma convencao de troca do insertion (cada deslocamento = 1).*/
/* ------------------------------------------------------------------ */
void shellSort(int *vetor, int n, Metricas *m) {
    m->comparacoes = 0;
    m->trocas = 0;

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int chave = vetor[i];
            int j = i;
            while (j >= gap) {
                m->comparacoes++;
                if (vetor[j - gap] > chave) {
                    vetor[j] = vetor[j - gap];
                    m->trocas++;
                    j -= gap;
                } else {
                    break;
                }
            }
            vetor[j] = chave;            /* posicionamento: nao contabilizado */
        }
    }
}

/* ------------------------------------------------------------------ */
/* 5) QUICKSORT classico (pivo = ultimo elemento, particao de Lomuto) */
/* ------------------------------------------------------------------ */
static int particionar(int *vetor, int baixo, int alto, Metricas *m) {
    int pivo = vetor[alto];              /* pivo = ultimo elemento do subvetor */
    int i = baixo - 1;
    for (int j = baixo; j < alto; j++) {
        m->comparacoes++;
        if (vetor[j] <= pivo) {
            i++;
            trocar(vetor, i, j, m);
        }
    }
    trocar(vetor, i + 1, alto, m);       /* coloca o pivo na posicao final */
    return i + 1;
}

static void quickSortRec(int *vetor, int baixo, int alto, Metricas *m) {
    if (baixo < alto) {
        int p = particionar(vetor, baixo, alto, m);
        quickSortRec(vetor, baixo, p - 1, m);
        quickSortRec(vetor, p + 1, alto, m);
    }
}

void quickSort(int *vetor, int n, Metricas *m) {
    m->comparacoes = 0;
    m->trocas = 0;
    quickSortRec(vetor, 0, n - 1, m);
}

/* ------------------------------------------------------------------ */
/* 6) HEAPSORT (max-heap baseado em vetor)                            */
/* ------------------------------------------------------------------ */
static void heapificar(int *vetor, int n, int i, Metricas *m) {
    int maior = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n) {
        m->comparacoes++;
        if (vetor[esq] > vetor[maior]) maior = esq;
    }
    if (dir < n) {
        m->comparacoes++;
        if (vetor[dir] > vetor[maior]) maior = dir;
    }
    if (maior != i) {
        trocar(vetor, i, maior, m);
        heapificar(vetor, n, maior, m);  /* desce o elemento (altura O(log n)) */
    }
}

void heapSort(int *vetor, int n, Metricas *m) {
    m->comparacoes = 0;
    m->trocas = 0;

    /* Constroi o max-heap (de baixo para cima). */
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapificar(vetor, n, i, m);
    }
    /* Extrai o maior repetidamente para o fim do vetor. */
    for (int i = n - 1; i > 0; i--) {
        trocar(vetor, 0, i, m);
        heapificar(vetor, i, 0, m);
    }
}
