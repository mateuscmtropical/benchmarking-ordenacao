# Relatório de Execução — Ambiente Dieimes

## 1. Identificação

- **Integrante 1:** Mateus da Silva Rego (RA: 2864584)
- **Integrante 2:** Dieimes Nunes de Souza (RA: 2848244)
- **Integrante 3:** Pedro Manoel Rosa Domingues Santos (RA: 2627167)
- **Disciplina:** Estrutura de Dados — UTFPR
- **Linguagem:** C
- **Ambiente:** Linux Mint

---

## 2. Ambiente de Teste — Dieimes

| Item | Especificação |
|---|---|
| Processador | AMD Ryzen 5 5600G with Radeon Graphics |
| Memória RAM | 31 GiB — equivalente a 32 GB instalados |
| Sistema Operacional | Linux Mint 22.3 |
| Compilador | gcc 13.3.0 |
| Flags de compilação | `-O2 -lm` |
| Observação | Antes da execução foi utilizado `ulimit -s unlimited` para evitar estouro de pilha no Quicksort clássico. |

---

## 3. Apresentação dos Dados

### 3.1 Problema 1 — N = 30.000

| Algoritmo | Cenário | Tempo (s) | Comparações | Trocas |
|---|---|---:|---:|---:|
| Bubblesort | Aleatório | 1,613143 | 449.897.368 | 225.667.291 |
| Bubblesort | Ordenado (melhor) | 0,000030 | 29.999 | 0 |
| Bubblesort | Inverso (pior) | 2,564479 | 449.985.000 | 449.985.000 |
| Insertionsort | Aleatório | 0,103178 | 225.697.282 | 225.667.291 |
| Insertionsort | Ordenado (melhor) | 0,000027 | 29.999 | 0 |
| Insertionsort | Inverso (pior) | 0,205280 | 449.985.000 | 449.985.000 |
| Selectionsort | Aleatório | 0,813140 | 449.985.000 | 29.992 |
| Selectionsort | Ordenado (melhor) | 0,813375 | 449.985.000 | 0 |
| Selectionsort | Inverso (pior) | 0,812564 | 449.985.000 | 15.000 |

### 3.2 Problema 2 — N = 5.000, vetor ordenado

| Algoritmo | Tempo médio (s) | Desvio padrão (s) | Comparações | Trocas |
|---|---:|---:|---:|---:|
| Heapsort | 0,000229 | 0,000042 | 112.126 | 60.932 |
| Quicksort | 0,007127 | 0,000100 | 12.497.500 | 12.502.499 |

![Gráfico do Problema 2](data/grafico_problema2.png)

### 3.3 Problema 3 — N = 50.000, vetor quase ordenado

| Algoritmo | Tempo (s) | Comparações | Trocas |
|---|---:|---:|---:|
| Insertionsort | 0,000044 | 50.249 | 250 |
| Shellsort | 0,000650 | 700.256 | 250 |

---

## 4. Análise das Questões

### Problema 1 — Selectionsort ou Insertionsort no vetor aleatório?

No vetor aleatório, o **Insertionsort** foi mais rápido que o **Selectionsort**.

O Insertionsort executou em **0,103178 s**, enquanto o Selectionsort executou em **0,813140 s**. Ou seja, no ambiente de teste utilizado, o Insertionsort foi aproximadamente **8 vezes mais rápido**.

A principal explicação está na lógica dos algoritmos. O Selectionsort sempre procura o menor elemento no restante do vetor, independentemente de o vetor estar ordenado, invertido ou aleatório. Por isso ele realizou **449.985.000 comparações** em todos os cenários.

Já o Insertionsort é mais adaptativo. Em um vetor aleatório, ele não precisa percorrer sempre todo o trecho anterior do vetor. Quando encontra a posição correta do elemento, o laço interno para. Por isso, no cenário aleatório, ele realizou **225.697.282 comparações**, aproximadamente metade das comparações do Selectionsort.

Mesmo realizando mais movimentações no vetor, o Insertionsort foi mais rápido porque reduziu bastante o número de comparações e iterações internas.

---

### Problema 2 — Por que o Quicksort clássico demora no vetor ordenado?

O Quicksort clássico utilizado neste trabalho escolhe sempre o **último elemento como pivô**. Quando o vetor já está ordenado em ordem crescente, esse último elemento é sempre o maior valor do subvetor.

Com isso, a partição fica totalmente desbalanceada: de um lado ficam praticamente todos os elementos e do outro lado não fica nenhum elemento. Em vez de dividir o problema em duas partes equilibradas, o Quicksort reduz o problema apenas de `n` para `n-1`.

Esse comportamento leva o algoritmo ao pior caso, com complexidade **O(n²)**. Os dados confirmam isso, pois o Quicksort realizou **12.497.500 comparações**, valor equivalente a `n(n-1)/2` para `n = 5.000`.

Já o Heapsort não depende da escolha de pivô. Ele organiza os dados em uma estrutura de heap e realiza operações de custo logarítmico. Por isso, mesmo com o vetor ordenado, ele manteve um desempenho muito melhor, realizando apenas **112.126 comparações**.

No ambiente testado, o Quicksort teve tempo médio de **0,007127 s**, enquanto o Heapsort teve tempo médio de **0,000229 s**. Assim, o Quicksort foi aproximadamente **31 vezes mais lento** nesse cenário.

---

### Problema 3 — Em dados quase ordenados, Shellsort ou Insertionsort?

No cenário quase ordenado, o **Insertionsort** foi mais eficiente que o **Shellsort**.

O Insertionsort executou em **0,000044 s**, enquanto o Shellsort executou em **0,000650 s**. Além disso, o Insertionsort realizou **50.249 comparações**, enquanto o Shellsort realizou **700.256 comparações**.

Isso acontece porque o vetor já estava praticamente ordenado. Nesse tipo de situação, o Insertionsort tem ótimo comportamento, pois cada elemento precisa de poucas comparações para confirmar que já está próximo da posição correta.

O Shellsort, por outro lado, faz várias passagens usando diferentes valores de gap. Esses saltos longos são úteis quando o vetor está muito desordenado, mas em um vetor quase ordenado eles acabam gerando trabalho extra. Mesmo havendo poucas movimentações reais, o Shellsort ainda precisa percorrer o vetor várias vezes.

Portanto, para este cenário específico, os saltos do Shellsort não trouxeram vantagem. O Insertionsort foi mais eficiente por aproveitar melhor o fato de o vetor já estar quase ordenado.

---

## 5. Observação sobre os resultados

Os tempos de execução podem variar conforme o computador, o sistema operacional, o compilador e a carga do sistema no momento dos testes. Por isso, os tempos obtidos neste ambiente devem ser comparados principalmente dentro do próprio ambiente Dieimes.

As contagens de comparações e trocas são mais estáveis, pois dependem diretamente da implementação dos algoritmos e dos dados de entrada utilizados.