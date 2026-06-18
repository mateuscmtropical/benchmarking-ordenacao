# Benchmarking e Análise Empírica de Algoritmos de Ordenação

Trabalho prático de Estrutura de Dados — implementação, instrumentação e
comparação empírica de seis algoritmos clássicos de ordenação:
**Bubblesort, Insertionsort, Selectionsort, Shellsort, Quicksort e Heapsort**.

Cada algoritmo é instrumentado para contar **comparações de chaves** e
**trocas (movimentações no vetor)**, e cada execução é cronometrada com
`clock_gettime(CLOCK_MONOTONIC)`.

## Estrutura do repositório

```
.
├── src/
│   ├── ordenacao.h     # struct Metricas + protótipos dos 6 algoritmos
│   ├── ordenacao.c     # implementação instrumentada dos 6 algoritmos
│   └── main.c          # geradores de dados + harness dos Problemas 1, 2 e 3
├── data/               # saídas geradas pelo programa (CSV) + gráfico (PNG)
│   ├── problema1.csv
│   ├── problema2_execucoes.csv
│   ├── problema2_resumo.csv
│   ├── problema3.csv
│   └── grafico_problema2.png
├── grafico.py          # gera o gráfico de barras do Problema 2 (matplotlib)
├── RELATORIO.md        # relatório com tabelas, gráfico e análise das questões
└── README.md
```

## Como compilar

O Quicksort clássico é recursivo e, sobre um vetor já ordenado (Problema 2),
atinge profundidade de recursão O(n). Por isso a pilha é ampliada na
compilação/execução.

### Linux / macOS

```bash
# aumenta a pilha do shell para evitar estouro no Quicksort (caso degenerado)
ulimit -s unlimited

gcc src/ordenacao.c src/main.c -o programa -O2 -lm
mkdir -p data
./programa
```

### Windows (MinGW-w64 / MSYS2)

No Windows não existe `ulimit`; a pilha é ampliada por flag de linker
(`--stack`, aqui 64 MB):

```powershell
gcc src/ordenacao.c src/main.c -o programa.exe -O2 "-Wl,--stack,67108864" -lm
New-Item -ItemType Directory -Force -Path data
.\programa.exe
```

> O programa grava os arquivos em `data/`; crie essa pasta **antes** de executar.

## Como gerar o gráfico do Problema 2

```bash
pip install matplotlib
python grafico.py        # lê data/problema2_resumo.csv -> data/grafico_problema2.png
```

## Parâmetros dos experimentos

| Problema | Algoritmos | N | Cenário |
|---|---|---|---|
| 1 | Bubble (otimizado), Insertion, Selection | 30.000 | aleatório, ordenado, inverso |
| 2 | Heapsort, Quicksort (pivô = último) | 5.000 | já ordenado, 10 execuções (média + desvio) |
| 3 | Insertion, Shellsort (gaps N/2,N/4,…,1) | 50.000 | quase ordenado (0,5% de vizinhos trocados) |

Semente fixa `srand(42)` garante reprodutibilidade dos dados aleatórios e
quase-ordenados.

## Convenção de contagem

- **Comparações:** incrementadas a cada comparação de chaves do vetor.
- **Trocas:** incrementadas a cada movimentação de elemento:
  - Bubble / Selection / Quick / Heap → cada *swap* conta como 1;
  - Insertion / Shell → cada deslocamento (`vetor[j] = vetor[j-gap]`) conta como 1,
    seguindo exatamente o exemplo de instrumentação do enunciado.
