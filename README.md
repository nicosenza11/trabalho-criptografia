# BR-BEPO

Implementação em C++ de uma cifra própria que combina substituição e transposição, com comparação de tempo de cifragem e decifragem contra AES e RSA da OpenSSL.

## Cifra BR-BEPO

A substituição é feita byte a byte. Letras ASCII são mapeadas pelo arranjo `BPOVDLJZWAUIECTSRNMYXKQGHF`, enquanto os demais bytes são representados em hexadecimal.

Após a substituição, os tokens são agrupados em blocos de até 26 elementos e transpostos por uma das 25 permutações armazenadas em `setup.h`. As permutações foram obtidas a partir dos resultados do Campeonato Brasileiro de 2002.

## Comparação

O experimento compara:

- BR-BEPO;
- AES-256-CBC;
- RSA-2048 com OAEP e SHA-256.

São utilizados quatro arquivos em diferentes faixas de tamanho:

- `<1K`;
- `1K–10K`;
- `10K–100K`;
- `>100K`.

Os arquivos utilizados estão em `dados_gutenberg/`.

## Dependências

- C++17;
- GNU Make;
- OpenSSL com headers de desenvolvimento;
- Python 3;
- Matplotlib.

## Compilação

```bash
make
```

Esse comando gera:

- `programa_BRBEPO`;
- `benchmark`;
- `gerador_tabela`.

## Execução da BR-BEPO

Coloque o texto de entrada em `texto.txt` e execute:

```bash
./programa_BRBEPO
```

O programa solicita uma chave entre 1 e 25 e gera:

- `arquivoencriptado.txt`;
- `arquivodescriptado.txt`.

## Testes

```bash
make test
```

Os testes verificam a cifragem e decifragem da BR-BEPO, AES e RSA, a geração das permutações e a preparação do corpus.

## Experimento

Para executar a comparação completa:

```bash
make experimento
```

O experimento usa a chave BR-BEPO 25, valida os quatro arquivos do corpus e executa uma cifragem e uma decifragem com cada algoritmo.

Os resultados são gravados em:

```text
resultados/
├── benchmark.csv
├── grafico_cifragem.png
└── grafico_decifragem.png
```

O arquivo `benchmark.csv` contém:

```text
arquivo,tamanho_bytes,algoritmo,operacao,tempo_ns
```

## Ordem recomendada

```bash
make
make test
make experimento
```

Para remover os binários e arquivos temporários:

```bash
make clean
```
