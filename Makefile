CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic -I.
OPENSSL_LIBS := -lssl -lcrypto

BRBEPO_SRCS := encriptador.cpp descriptador.cpp

.PHONY: all clean test graphs dados validar-dados experimento

all: programa_BRBEPO benchmark gerador_tabela

programa_BRBEPO: main.cpp $(BRBEPO_SRCS) encriptador.h descriptador.h setup.h
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(BRBEPO_SRCS)

benchmark: benchmark.cpp $(BRBEPO_SRCS) openssl_crypto.cpp openssl_crypto.h setup.h
	$(CXX) $(CXXFLAGS) -o $@ benchmark.cpp $(BRBEPO_SRCS) openssl_crypto.cpp $(OPENSSL_LIBS)

gerador_tabela: gerador_tabela.cpp resultados_brasileirao_2002.txt
	$(CXX) $(CXXFLAGS) -o $@ gerador_tabela.cpp

tests/testes: tests/tests.cpp $(BRBEPO_SRCS) openssl_crypto.cpp openssl_crypto.h setup.h
	$(CXX) $(CXXFLAGS) -o $@ tests/tests.cpp $(BRBEPO_SRCS) openssl_crypto.cpp $(OPENSSL_LIBS)

test: tests/testes gerador_tabela benchmark
	./tests/testes
	python3 scripts/verificar_gerador.py
	python3 scripts/test_benchmark.py
	python3 scripts/test_gutenberg.py

dados:
	python3 scripts/preparar_gutenberg.py

validar-dados:
	python3 scripts/validar_gutenberg.py

experimento: benchmark
	python3 scripts/executar_experimento.py

graphs:
	python3 scripts/plot_benchmark.py resultados/benchmark.csv --diretorio resultados

clean:
	rm -f programa_BRBEPO benchmark gerador_tabela tests/testes
	rm -f arquivoencriptado.txt arquivodescriptado.txt
