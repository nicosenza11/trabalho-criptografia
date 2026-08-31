#!/usr/bin/env python3
from pathlib import Path
import sys
import tempfile

sys.path.insert(0, str(Path(__file__).resolve().parent))
from preparar_gutenberg import MARCADOR_TRECHO, criar_trecho_lt1k, validar_faixas


def main():
    fonte = b"cabecalho\r\n" + MARCADOR_TRECHO + b"\r\n" + (b"linha de teste 1234567890\r\n" * 100)
    trecho = criar_trecho_lt1k(fonte)
    assert 0 < len(trecho) < 1000
    assert trecho in fonte
    assert trecho.startswith(MARCADOR_TRECHO)

    validar_faixas({"<1K": 900, "1K-10K": 9970, "10K-100K": 27070, ">100K": 107810})

    casos_invalidos = [
        {"<1K": 1000, "1K-10K": 9970, "10K-100K": 27070, ">100K": 107810},
        {"<1K": 900, "1K-10K": 1024, "10K-100K": 27070, ">100K": 107810},
        {"<1K": 900, "1K-10K": 9970, "10K-100K": 10000, ">100K": 107810},
        {"<1K": 900, "1K-10K": 9970, "10K-100K": 27070, ">100K": 100001},
    ]
    for caso in casos_invalidos:
        try:
            validar_faixas(caso)
            raise AssertionError(f"Caso invalido aceito: {caso}")
        except ValueError:
            pass

    print("Testes da preparacao Gutenberg: OK")


if __name__ == "__main__":
    main()
