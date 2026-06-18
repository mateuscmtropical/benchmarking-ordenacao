#ifndef ORDENACAO_H
#define ORDENACAO_H

/*
 * Estrutura para contabilizar as metricas do algoritmo.
 * (Identica ao esqueleto obrigatorio fornecido no enunciado.)
 */
typedef struct {
    unsigned long long comparacoes;
    unsigned long long trocas;
    double tempo_segundos;
} Metricas;

/*
 * Convencao de instrumentacao (segue o exemplo de insertionSort do enunciado):
 *
 *  comparacoes -> incrementada a CADA comparacao de chaves do vetor.
 *
 *  trocas      -> incrementada a CADA movimentacao de elemento no vetor:
 *                   - Bubble / Selection / Quick / Heap: cada troca (swap) = 1;
 *                   - Insertion / Shell: cada deslocamento (vetor[j] = vetor[j-gap])
 *                     = 1, exatamente como no esqueleto dado.
 *
 * Toda funcao zera m->comparacoes e m->trocas no inicio.
 */

void bubbleSort(int *vetor, int n, Metricas *m);     /* otimizado: para sem trocas + ultima troca */
void insertionSort(int *vetor, int n, Metricas *m);
void selectionSort(int *vetor, int n, Metricas *m);
void shellSort(int *vetor, int n, Metricas *m);      /* gaps de Shell: N/2, N/4, ..., 1 */
void quickSort(int *vetor, int n, Metricas *m);      /* pivo = ultimo elemento (Lomuto) */
void heapSort(int *vetor, int n, Metricas *m);

#endif /* ORDENACAO_H */
