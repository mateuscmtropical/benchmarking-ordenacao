"""
Graficos comparativos dos 3 PCs (Mateus / Dieimes / Pedro).

Le os CSVs de data_consolidado/<maquina>/ -- nada e fixado no codigo, os
graficos refletem sempre os dados reais de cada maquina.

Saidas (em data_consolidado/):
  1. grafico_p2_consolidado.png        -> Heapsort vs Quicksort (P2) nos 3 PCs
  2. grafico_throughput_consolidado.png -> vazao do Selectionsort (450M comparacoes)

Uso:  python grafico_consolidado.py
"""
import csv
import os
import matplotlib
matplotlib.use("Agg")  # backend sem janela (salva direto em arquivo)
import matplotlib.pyplot as plt

BASE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "data_consolidado")

# (rotulo no grafico, subpasta, cor)
MAQUINAS = [
    ("Mateus\ni7-11800H | Windows", "mateus", "#2E86C1"),
    ("Dieimes\nRyzen 5 5600G | Linux", "dieimes", "#27AE60"),
    ("Pedro\ni5-7200U | Linux", "pedro", "#C0392B"),
]


def ler_resumo_p2(sub):
    """Retorna {Algoritmo: (media_s, desvio_s)} do problema2_resumo.csv."""
    out = {}
    caminho = os.path.join(BASE, sub, "problema2_resumo.csv")
    with open(caminho, newline="", encoding="utf-8") as f:
        for row in csv.DictReader(f):
            out[row["Algoritmo"]] = (float(row["Media_s"]), float(row["DesvioPadrao_s"]))
    return out


def ler_selection_p1(sub):
    """Tempo medio (s) e nº de comparacoes do Selectionsort nos 3 cenarios do P1."""
    tempos, comps = [], []
    caminho = os.path.join(BASE, sub, "problema1.csv")
    with open(caminho, newline="", encoding="utf-8") as f:
        for row in csv.DictReader(f):
            if row["Algoritmo"] == "Selectionsort":
                tempos.append(float(row["Tempo_s"]))
                comps.append(int(row["Comparacoes"]))
    media = sum(tempos) / len(tempos)
    return media, comps[0]  # comparacoes identicas nos 3 cenarios (input-independente)


# ---------------------------------------------------------------------------
# GRAFICO 1 -- Problema 2: Heapsort vs Quicksort (2 paineis, ms, escala log)
# ---------------------------------------------------------------------------
dados = {sub: ler_resumo_p2(sub) for _, sub, _ in MAQUINAS}
rotulos = [r for r, _, _ in MAQUINAS]
cores = [c for _, _, c in MAQUINAS]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

for ax, algo in ((ax1, "Heapsort"), (ax2, "Quicksort")):
    medias = [dados[sub][algo][0] * 1000 for _, sub, _ in MAQUINAS]   # s -> ms
    desvios = [dados[sub][algo][1] * 1000 for _, sub, _ in MAQUINAS]
    barras = ax.bar(rotulos, medias, yerr=desvios, capsize=8,
                    color=cores, edgecolor="black", zorder=3)
    ax.set_title(f"{algo}  (N=5000, vetor ordenado)", fontsize=12, fontweight="bold")
    ax.set_ylabel("Tempo medio (ms)  [media de 10 execucoes]")
    ax.grid(axis="y", linestyle="--", alpha=0.5, zorder=0)
    for b, mu, sd in zip(barras, medias, desvios):
        ax.text(b.get_x() + b.get_width() / 2, b.get_height() + sd,
                f"{mu:.3f} ms\n(+/- {sd:.3f})", ha="center", va="bottom", fontsize=9)
    ax.set_ylim(0, max(medias) * 1.30)

fig.suptitle("Problema 2 -- Heapsort vs Quicksort (pivo no ultimo elemento) nos 3 PCs",
             fontsize=14, fontweight="bold")
plt.tight_layout(rect=[0, 0, 1, 0.96])
saida1 = os.path.join(BASE, "grafico_p2_consolidado.png")
plt.savefig(saida1, dpi=150)
plt.close(fig)
print("salvo:", saida1)

# ---------------------------------------------------------------------------
# GRAFICO 2 -- Vazao do Selectionsort: 450M comparacoes IDENTICAS em todo PC
#              (carga de trabalho exatamente igual -> comparacao de hardware pura)
# ---------------------------------------------------------------------------
medias_sel, throughputs = [], []
for _, sub, _ in MAQUINAS:
    media, comp = ler_selection_p1(sub)
    medias_sel.append(media)
    throughputs.append(comp / media / 1e6)  # Mcomparacoes/segundo

fig, ax = plt.subplots(figsize=(8.5, 5.5))
barras = ax.bar(rotulos, throughputs, color=cores, edgecolor="black", zorder=3)
ax.set_title("Vazao de varredura -- Selectionsort\n"
             "449.985.000 comparacoes IDENTICAS em cada PC (carga input-independente)",
             fontsize=12, fontweight="bold")
ax.set_ylabel("Milhoes de comparacoes por segundo (maior = melhor)")
ax.grid(axis="y", linestyle="--", alpha=0.5, zorder=0)
ax.set_ylim(0, max(throughputs) * 1.18)
for b, tp, mt in zip(barras, throughputs, medias_sel):
    ax.text(b.get_x() + b.get_width() / 2, b.get_height(),
            f"{tp:.0f} Mcomp/s\n({mt:.3f} s)", ha="center", va="bottom", fontsize=10)

plt.tight_layout()
saida2 = os.path.join(BASE, "grafico_throughput_consolidado.png")
plt.savefig(saida2, dpi=150)
plt.close(fig)
print("salvo:", saida2)
print("OK -- 2 graficos gerados.")
