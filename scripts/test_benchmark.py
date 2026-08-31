#!/usr/bin/env python3
import csv
import subprocess
import tempfile
from pathlib import Path

raiz = Path(__file__).resolve().parents[1]

with tempfile.TemporaryDirectory() as tmp:
    tmp = Path(tmp)
    arquivos = []
    for indice, tamanho in enumerate((31, 257, 1025, 4097), start=1):
        caminho = tmp / f"teste_{indice}.txt"
        caminho.write_bytes(bytes(((i * 37 + indice) & 0xFF) for i in range(tamanho)))
        arquivos.append(caminho)

    csv_saida = tmp / "benchmark.csv"
    subprocess.run(
        [str(raiz / "benchmark"), "--rodada", "25",
         "--saida", str(csv_saida), *map(str, arquivos)],
        cwd=raiz,
        check=True,
        stdout=subprocess.DEVNULL,
    )

    with csv_saida.open(newline="", encoding="utf-8") as f:
        linhas = list(csv.DictReader(f))

    esperado = 4 * 3 * 2
    if len(linhas) != esperado:
        raise SystemExit(f"CSV do benchmark tem {len(linhas)} linhas; esperado: {esperado}")

    if set(linhas[0]) != {"arquivo", "tamanho_bytes", "algoritmo", "operacao", "tempo_ns"}:
        raise SystemExit(f"Colunas inesperadas no CSV: {list(linhas[0])}")

    algoritmos = {linha["algoritmo"] for linha in linhas}
    operacoes = {linha["operacao"] for linha in linhas}
    if algoritmos != {"BR-BEPO", "AES-256-CBC", "RSA-2048-OAEP-SHA256"}:
        raise SystemExit(f"Algoritmos inesperados no CSV: {sorted(algoritmos)}")
    if operacoes != {"cifragem", "decifragem"}:
        raise SystemExit(f"Operacoes inesperadas no CSV: {sorted(operacoes)}")
    if any(int(linha["tempo_ns"]) < 0 for linha in linhas):
        raise SystemExit("CSV contem tempo negativo.")

    chaves = {(x["arquivo"], x["algoritmo"], x["operacao"]) for x in linhas}
    if len(chaves) != len(linhas):
        raise SystemExit("CSV contem medicao duplicada.")

    graficos = tmp / "graficos"
    subprocess.run(
        ["python3", str(raiz / "scripts" / "plot_benchmark.py"),
         str(csv_saida), "--diretorio", str(graficos)],
        cwd=raiz,
        check=True,
        stdout=subprocess.DEVNULL,
    )

    for nome in ("grafico_cifragem.png", "grafico_decifragem.png"):
        caminho = graficos / nome
        if not caminho.is_file() or caminho.stat().st_size == 0:
            raise SystemExit(f"Grafico nao foi criado corretamente: {nome}")

print("Benchmark e geracao dos dois graficos passaram no teste de integracao.")
