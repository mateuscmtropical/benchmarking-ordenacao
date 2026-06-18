# Trabalho Prático: Benchmarking e Análise Empírica de Algoritmos de Ordenação

Pacote de entrega consolidado. Reúne, em uma única pasta, a implementação em C, os
dados medidos nos três computadores do grupo e o relatório que compara os
resultados entre as máquinas.

## Integrantes

| Integrante | RA | Máquina | Ambiente |
|---|---|---|---|
| Dieimes Nunes de Souza | 2848244 | Ryzen 5 5600G | Linux Mint 22.3 / gcc 13.3.0 |
| Mateus da Silva Rego | 2864584 | i7-11800H | Windows 11 / gcc 15.2.0 (MinGW) |
| Pedro Manoel Rosa Domingues Santos | 2627167 | i5-7200U | Ubuntu (Linux) / gcc 15.2.0 |

## Como ler esta entrega

1. **`RELATORIO_CONSOLIDADO.pdf`** (ou `.md`) é o documento principal. Contém a
   análise completa, as respostas às perguntas do enunciado e a comparação entre os
   três ambientes. Comece por ele.
2. Os dados brutos de cada máquina estão em `data_consolidado/`.
3. O código-fonte único, compilado e executado nas três máquinas, está em `src/`.

## Estrutura da pasta

```
Entrega-Ordenacao/
├── README.md                        (este arquivo)
├── RELATORIO_CONSOLIDADO.md         (relatório principal, fonte)
├── RELATORIO_CONSOLIDADO.pdf        (relatório principal, pronto p/ enviar)
├── grafico_consolidado.py           (gera os 2 gráficos a partir dos CSVs)
├── src/
│   ├── ordenacao.h                  (struct Metricas + protótipos)
│   ├── ordenacao.c                  (os 6 algoritmos instrumentados)
│   ├── main.c                       (geradores, harness, escrita dos CSVs)
│   └── README_build.md              (detalhes de compilação do código)
├── data_consolidado/
│   ├── mateus/                      (4 CSVs medidos no PC do Mateus)
│   ├── dieimes/                     (4 CSVs medidos no PC do Dieimes)
│   ├── pedro/                       (4 CSVs medidos no PC do Pedro)
│   ├── grafico_p2_consolidado.png
│   └── grafico_throughput_consolidado.png
└── relatorios_individuais/
    ├── RELATORIO_Mateus.md
    ├── relatorio_dieimes.md
    └── RELATORIO_Pedro.md
```

## Como compilar e executar o código

O mesmo código roda nos três sistemas; só muda a forma de ampliar a pilha (exigida
pelo Quicksort recursivo no pior caso).

**Linux (Dieimes, Pedro):**

```bash
cd src
ulimit -s unlimited
gcc -O2 -o ordenacao main.c ordenacao.c -lm
mkdir -p data
./ordenacao
```

**Windows (Mateus, MSYS2/MinGW):**

```bash
cd src
gcc -O2 -Wl,--stack,67108864 -o ordenacao.exe main.c ordenacao.c -lm
mkdir data
./ordenacao.exe
```

O programa gera os arquivos `data/problema1.csv`, `data/problema2_execucoes.csv`,
`data/problema2_resumo.csv` e `data/problema3.csv`. Esses foram os arquivos
coletados de cada integrante e reunidos em `data_consolidado/`.

## Como regenerar os gráficos

Na raiz da entrega (precisa de Python com matplotlib):

```bash
python grafico_consolidado.py
```

Lê os CSVs de `data_consolidado/<maquina>/` e regrava os 2 PNGs. Nada é fixado no
código; os gráficos refletem sempre os dados reais.

## Notas

- As contagens de comparações e trocas são determinísticas e serviram para validar
  que o mesmo código rodou nas três máquinas (ver seção 3 do relatório principal).
- O relatório individual do Pedro (`relatorios_individuais/RELATORIO_Pedro.md`)
  tem partes de texto desatualizadas herdadas de um template; os valores corretos
  da máquina dele estão nos CSVs de `data_consolidado/pedro/`, que foram a fonte de
  verdade usada no relatório consolidado (ver Apêndice B do relatório principal).
