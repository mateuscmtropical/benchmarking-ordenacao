# Relatório de Execução — Benchmarking e Análise Empírica de Algoritmos de Ordenação

## 1. Identificação

- **Integrante 1:** Mateus da Silva Rego (RA: 2864584)
- **Integrante 2:** Dieimes Nunes de Souza (RA: 2848244)
- **Integrante 3:** Pedro Manoel Rosa Domingues Santos (RA: 2627167)
- **Disciplina:** Estrutura de Dados — 1º Semestre / UTFPR
- **Linguagem:** C (gcc 15.2.0, MSYS2/MinGW-w64)

## 2. Ambiente de Teste

| Item | Especificação |
|---|---|
| Processador | Intel(R) Core(TM) i5-7200U CPU @ 2.50GHz (2 núcleos / 4 threads) |
| Memória RAM | 18 GiB |
| Sistema Operacional | Ubuntu (Linux 7.0.0-22-generic x86_64) |
| Compilador | gcc 15.2.0 (Ubuntu 15.2.0-16ubuntu1) |
| Flags de compilação | `-O2 -lm` |

### Metodologia de medição

- O tempo é medido com `clock_gettime(CLOCK_MONOTONIC)` (relógio monotônico de
  alta resolução), englobando apenas a chamada da função de ordenação.
- Cada execução opera sobre uma **cópia nova** do vetor de entrada, de modo que o
  vetor-base original seja preservado e todos os algoritmos recebam exatamente os
  mesmos dados.
- A semente aleatória é fixa (`srand(42)`), garantindo reprodutibilidade dos
  cenários aleatório e quase-ordenado.
- O Quicksort clássico é recursivo e, sobre vetor já ordenado, atinge
  profundidade O(n). Como a execução ocorreu em Linux, a pilha foi ampliada
  para tamanho ilimitado via comando `ulimit -s unlimited` antes da execução.
- **Contagem de operações** (convenção idêntica ao esqueleto do enunciado):
  *comparações* = toda comparação de chaves; *trocas* = toda movimentação de
  elemento (cada *swap* conta 1; em Insertion/Shell cada deslocamento
  `vetor[j] = vetor[j-gap]` conta 1).

> Observação: os tempos absolutos variam entre execuções (carga do sistema), mas
> as contagens de comparações e trocas são **determinísticas** e reproduzem-se
> exatamente. Por isso a análise se apoia principalmente nessas contagens, que
> são a evidência mais robusta do comportamento de cada algoritmo.

---

## 3. Apresentação de Dados

### 3.1 Problema 1 — N = 30.000 (Bubble otimizado, Insertion, Selection)

> Referência teórica: `n(n-1)/2 = 449.985.000` comparações para os algoritmos
> quadráticos no pior caso.

| Algoritmo | Cenário | Tempo (s) | Comparações | Trocas |
|---|---|---:|---:|---:|
| Bubblesort | Aleatório | 3,042576 | 449.897.368 | 225.667.291 |
| Bubblesort | Ordenado (melhor) | 0,000050 | 29.999 | 0 |
| Bubblesort | Inverso (pior) | 3,345392 | 449.985.000 | 449.985.000 |
| Insertionsort | Aleatório | 0,498843 | 225.697.282 | 225.667.291 |
| Insertionsort | Ordenado (melhor) | 0,000081 | 29.999 | 0 |
| Insertionsort | Inverso (pior) | 1,002064 | 449.985.000 | 449.985.000 |
| Selectionsort | Aleatório | 1,474775 | 449.985.000 | 29.992 |
| Selectionsort | Ordenado (melhor) | 1,470404 | 449.985.000 | 0 |
| Selectionsort | Inverso (pior) | 1,525016 | 449.985.000 | 15.000 |

_(dados brutos em `data/problema1.csv`)_

**Leitura rápida da tabela:**

- O **Selectionsort** mantém **449.985.000 comparações** nos três cenários —
  exatamente `n(n-1)/2`, confirmando que é insensível à ordem da entrada.
- O **Insertionsort** varia de **29.999** comparações (ordenado) a
  **449.985.000** (inverso) — variação drástica conforme a entrada.
- A otimização do **Bubblesort** aparece no cenário ordenado: uma única passagem
  (29.999 comparações, 0 trocas) e parada imediata.

### 3.2 Problema 2 — N = 5.000, vetor já ordenado (Heapsort × Quicksort)

Cada algoritmo foi executado **10 vezes** independentes. As comparações e trocas
são idênticas em toda execução (determinísticas).

| Algoritmo | Tempo médio (s) | Desvio padrão (s) | Comparações | Trocas |
|---|---:|---:|---:|---:|
| Heapsort | 0,000378 | 0,000155 | 112.126 | 60.932 |
| Quicksort | 0,023203 | 0,002982 | 12.497.500 | 12.502.499 |

_(dados brutos em `data/problema2_execucoes.csv` e `data/problema2_resumo.csv`)_

**Gráfico de barras (tempo médio ± desvio padrão):**

![Heapsort vs Quicksort — Problema 2](data/grafico_problema2.png)

O Quicksort foi, em média, **~46× mais lento** que o Heapsort neste cenário, e
suas **12.497.500 comparações** equivalem exatamente a `n(n-1)/2` para n = 5.000
— a assinatura inconfundível do **pior caso O(n²)**.

### 3.3 Problema 3 — N = 50.000, vetor quase ordenado (Insertion × Shell)

Cenário: vetor ordenado com **0,5% dos elementos (250)** trocados de lugar com o
vizinho imediato.

| Algoritmo | Tempo (s) | Comparações | Trocas |
|---|---|---:|---:|
| Insertionsort | 0,000189 | 50.249 | 250 |
| Shellsort | 0,001539 | 700.256 | 250 |

_(dados brutos em `data/problema3.csv`)_

O Insertionsort foi **~8× mais rápido** e fez **~14× menos comparações** que o
Shellsort, embora ambos tenham realizado exatamente o mesmo número de trocas
(250).

---

## 4. Análise das Questões

### Problema 1 — Qual foi mais rápido no vetor aleatório: Selection ou Insertion?

No vetor aleatório, o **Insertionsort** foi claramente mais rápido:
**0,498843 s contra 1,474775 s** do Selectionsort — cerca de **3,0× mais veloz**. A
explicação está na lógica interna de cada um:

- **Selectionsort é não-adaptativo.** Para cada posição `i`, ele percorre *todo*
  o subvetor restante à procura do menor elemento, sem qualquer atalho. Isso
  resulta sempre em `n(n-1)/2 = 449.985.000` comparações, **independentemente**
  da ordem dos dados — por isso o número é idêntico nos cenários ordenado e
  inverso, como aponta o enunciado. O laço interno executa ~450 milhões de
  iterações em qualquer entrada.

- **Insertionsort é adaptativo.** O laço interno (`while`) tem um `break`: assim
  que encontra a posição correta da chave, ele para. Em dados aleatórios, cada
  elemento percorre, em média, apenas **metade** do prefixo já ordenado antes de
  encontrar seu lugar. O resultado empírico confirma isso: **225.697.282**
  comparações — praticamente metade (`0,502×`) das 449.985.000 do Selectionsort.
  Como o laço interno do Insertion executa cerca de **metade** das iterações do
  Selection, ele termina em uma fração do tempo.

Vale notar um detalhe contra-intuitivo: o Insertionsort realizou **muito mais
trocas** (225.667.291) que o Selectionsort (29.992). Ainda assim foi mais
rápido. Isso mostra que, nestes algoritmos quadráticos, o gargalo é o **número de
comparações / iterações do laço de varredura**, não as escritas — que são
sequenciais, baratas e bem servidas pela cache. O Selectionsort economiza
escritas, mas paga caro por nunca poder interromper sua varredura.

### Problema 2 — Por que o Quicksort sofre com um vetor já ordenado e o Heapsort não?

O **Quicksort clássico** deste trabalho usa o **último elemento como pivô**. Em
um vetor já ordenado em ordem crescente, o último elemento de qualquer subvetor é
sempre o seu **máximo**. Na partição de Lomuto, todos os demais elementos são
menores ou iguais ao pivô e, portanto, vão todos para o mesmo lado; o pivô acaba
na última posição. Cada partição produz então:

- um subvetor de tamanho **n−1** (todos os elementos, exceto o pivô), e
- um subvetor de tamanho **0** (vazio).

Ou seja, a partição **degenera**: em vez de dividir o problema pela metade
(ideal O(n log n)), ela apenas remove **um único elemento** por chamada. A
recorrência vira `T(n) = T(n−1) + O(n)`, que resolve para **O(n²)**. A prova
empírica é direta: **12.497.500 comparações**, valor exatamente igual a
`n(n-1)/2` para n = 5.000 — o pior caso teórico se materializa nos dados. Além
disso, a recursão atinge profundidade ~n (5.000 chamadas aninhadas), o que exige
ampliar a pilha para não estourar (daí o `ulimit`/flag de pilha).

O **Heapsort**, ao contrário, é **insensível à ordem inicial**. Ele organiza os
dados em um *heap* binário e extrai o máximo repetidamente; cada operação de
`heapify` custa O(log n) e há O(n) delas, totalizando **O(n log n)** garantido em
qualquer entrada — melhor, médio ou pior caso. Por isso registrou apenas
**112.126 comparações** (contra 12,5 milhões do Quicksort) e tempo médio
**~46× menor**. O *heap* não tem um "caso degenerado" análogo ao do pivô fixo do
Quicksort: a estrutura de árvore mantém a altura logarítmica sempre.

> **Resumindo:** o vilão é a escolha do pivô. Pivô fixo (último elemento) +
> entrada ordenada = partições desbalanceadas (n−1 e 0) = O(n²). Estratégias como
> pivô aleatório ou mediana-de-três evitariam essa degeneração, mas não foram
> usadas aqui por exigência do enunciado.

### Problema 3 — Em dados quase-ordenados, os "saltos longos" do Shell ajudam?

Neste cenário, **não**: o **Insertionsort sozinho foi mais eficiente**
(0,189 ms contra 1,539 ms — ~8× mais rápido), e fez **50.249 comparações**
contra **700.256** do Shellsort.

A razão está em como cada um lida com um vetor que já está quase ordenado:

- O **Insertionsort é adaptativo**: seu custo é O(n + d), onde `d` é o número de
  inversões. Com apenas 250 perturbações locais, `d` é minúsculo. Cada elemento é
  comparado essencialmente **uma vez** com seu antecessor, percebe que já está em
  ordem e para (`break`). Por isso as comparações (50.249) ficam logo acima de
  `n = 50.000`: comportamento praticamente **linear**. As 250 trocas
  correspondem exatamente às pouquíssimas correções realmente necessárias.

- O **Shellsort** foi concebido para **dados muito desordenados**, onde mover
  elementos a grandes distâncias (saltos de tamanho `gap`) corrige muitas
  inversões de uma vez. Mas ele paga um preço fixo: executa **uma passagem
  completa para cada gap** da sequência `N/2, N/4, …, 1` — cerca de
  `log₂(50.000) ≈ 16` passagens, cada uma varrendo ~n elementos. Em um vetor já
  quase ordenado, **cada passagem encontra quase nada para mover**, mas mesmo
  assim precisa comparar todos os elementos para descobrir isso. Daí as ~700 mil
  comparações: são, em essência, **trabalho de varredura desperdiçado**.

O detalhe revelador é que **ambos fizeram 250 trocas**: o vetor exigia pouquíssima
movimentação real. A diferença é que o Insertionsort descobriu isso com ~50 mil
comparações (uma varredura quase-linear), enquanto o Shellsort gastou ~700 mil
comparações (dezesseis varreturas) para chegar ao mesmo resultado.

> **Conclusão:** os saltos longos do Shellsort só compensam quando há desordem de
> longo alcance a corrigir. Sobre dados quase ordenados, a estrutura multi-passe
> do Shell vira sobrecarga pura, e o atalho `break` do Insertionsort tradicional
> é imbatível. A "evolução" só é vantajosa no problema para o qual foi projetada.
