# BR-BEPO — Trabalho 1 de Criptografia

Implementacao em C++ de uma cifra propria que combina substituicao e transposicao, com comparacao de tempos de cifragem e decifragem contra AES e RSA da OpenSSL.

## Cifra BR-BEPO

A substituicao trabalha byte a byte:

- letras ASCII maiusculas: `U` + letra correspondente no layout `BPOVDLJZWAUIECTSRNMYXKQGHF`;
- letras ASCII minusculas: `L` + letra correspondente no mesmo layout;
- demais bytes: `X` + dois digitos hexadecimais.

Os tokens sao agrupados em blocos de ate 26 elementos. A transposicao usa uma das 25 permutacoes de `CHAVES_RODADAS`, derivadas pelo `gerador_tabela.cpp` a partir de `resultados_brasileirao_2002.txt`. O gerador fecha uma nova permutacao a cada 13 partidas processadas; portanto, essas 25 chaves nao devem ser descritas automaticamente como 25 rodadas oficiais do campeonato.

A cifra preserva os bytes do arquivo. Caracteres UTF-8 fora de ASCII sao representados pelos bytes hexadecimais correspondentes e recuperados byte a byte.

## AES e RSA usados na comparacao

Como o enunciado pede AES e RSA da OpenSSL sem fixar parametros, a implementacao usa:

- AES-256-CBC pela API EVP da OpenSSL, com padding PKCS#7;
- RSA-2048 com OAEP, SHA-256 e MGF1-SHA-256 pela API EVP;
- para permitir a cifragem dos arquivos com RSA-2048/OAEP-SHA256, a entrada e processada em blocos de no maximo 190 bytes.

A leitura dos arquivos e a preparacao das chaves/IV ficam fora do intervalo cronometrado. Cada resultado de decifragem e conferido byte a byte contra a entrada original.

## Arquivos do Project Gutenberg

O diretorio `dados_gutenberg/` contem os quatro arquivos usados no experimento:

- `<1K`: `lt1k_73576_excerpt.txt` — trecho textual contiguo e verbatim de *A Kiss for the Conqueror*;
- `1K–10K`: `73576-0.txt` — *A Kiss for the Conqueror*, arquivo completo;
- `10K–100K`: `1065.txt` — *The Raven*, arquivo completo;
- `>100K`: `41102-0.txt` — *The King's Threshold; and On Baile's Strand*, copia local do texto do Project Gutenberg usada no experimento.

A faixa `<1K` usa um trecho verbatim porque um arquivo `.txt` completo do Project Gutenberg nao atendia naturalmente a essa faixa. Para reprodutibilidade, `dados_gutenberg/manifesto.csv` registra o tamanho, o SHA-256 efetivamente usado e a URL de origem de cada entrada. Os arquivos `73576-0.txt` e `1065.txt` tambem sao conferidos contra snapshots conhecidos. A copia local `41102-0.txt` e validada pelo manifesto e pela faixa `>100K`, sem afirmar correspondencia byte a byte com um snapshot remoto.

## Dependencias

- compilador C++17;
- GNU Make;
- OpenSSL com headers de desenvolvimento;
- Python 3 e Matplotlib para gerar os graficos.

## Compilar

    make

Sao gerados:

- `programa_BRBEPO`: demonstracao interativa da cifra propria usando `texto.txt`;
- `benchmark`: comparacao BR-BEPO, AES e RSA;
- `gerador_tabela`: reproduz as chaves a partir dos resultados do campeonato.

## Testes

    make test

Os testes verificam:

- as 25 linhas de `CHAVES_RODADAS` como permutacoes de `0..25`;
- o exemplo conhecido `A bá!` com a chave 1;
- round-trip BR-BEPO para todos os 256 valores de byte em todas as 25 chaves e varios tamanhos de entrada;
- round-trip AES em limites de bloco;
- round-trip RSA antes, no limite e depois do bloco de 190 bytes;
- reproducao exata de `CHAVES_RODADAS` pelo `gerador_tabela`;
- execucao integrada do benchmark e criacao dos dois graficos;
- regras de preparacao e validacao dos arquivos do Project Gutenberg.

## Demonstracao da BR-BEPO

Coloque a entrada em `texto.txt` e execute:

    ./programa_BRBEPO

O programa cria `arquivoencriptado.txt` e `arquivodescriptado.txt`.

## Experimento pedido no enunciado

O experimento final usa a chave BR-BEPO 25 e executa uma medicao de cifragem e uma de decifragem com BR-BEPO, AES e RSA em cada um dos quatro arquivos:

    make experimento

Esse comando valida os quatro arquivos, executa o benchmark e gera:

- `resultados/benchmark.csv`;
- `resultados/grafico_cifragem.png`;
- `resultados/grafico_decifragem.png`.

O CSV possui as colunas:

    arquivo,tamanho_bytes,algoritmo,operacao,tempo_ns

## Arquivo de resultados do Brasileirao

`gerador_tabela.cpp` le `resultados_brasileirao_2002.txt` e gera uma matriz `25 x 26`. O comando `make test` verifica que a matriz gerada e exatamente a mesma armazenada em `setup.h`.
