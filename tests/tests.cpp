#include "descriptador.h"
#include "encriptador.h"
#include "openssl_crypto.h"
#include "setup.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void exigir(bool condicao, const std::string& mensagem) {
    if (!condicao) {
        throw std::runtime_error(mensagem);
    }
}

Bytes sequencia(size_t tamanho) {
    Bytes dados(tamanho);
    for (size_t i = 0; i < tamanho; ++i) {
        dados[i] = static_cast<unsigned char>((i * 73 + 19) & 0xFF);
    }
    return dados;
}

void testar_chaves_brasileirao() {
    for (int r = 0; r < 25; ++r) {
        std::vector<int> chave(CHAVES_RODADAS[r], CHAVES_RODADAS[r] + 26);
        std::sort(chave.begin(), chave.end());
        for (int i = 0; i < 26; ++i) {
            exigir(chave[static_cast<size_t>(i)] == i,
                   "CHAVES_RODADAS contem uma linha que nao e permutacao 0..25.");
        }
    }
}

void testar_brbepo_exemplo() {
    const Bytes entrada = {'A', ' ', 'b', 0xC3, 0xA1, '!'};
    const std::string esperado = "LP|UB|XA1|X21|XC3|X20|";
    const std::string cifrado = cifrar_brbepo(entrada, 1);
    exigir(cifrado == esperado, "O exemplo BR-BEPO conhecido mudou.");
    exigir(decifrar_brbepo(cifrado, 1) == entrada, "Falha no exemplo BR-BEPO conhecido.");
}

void testar_brbepo_todas_chaves() {
    Bytes todos_bytes(256);
    std::iota(todos_bytes.begin(), todos_bytes.end(), 0);

    const std::vector<size_t> tamanhos = {0, 1, 2, 25, 26, 27, 51, 52, 53, 255, 256, 1024};
    for (int rodada = 1; rodada <= 25; ++rodada) {
        exigir(decifrar_brbepo(cifrar_brbepo(todos_bytes, rodada), rodada) == todos_bytes,
               "Falha BR-BEPO com todos os 256 bytes na chave " + std::to_string(rodada));

        for (size_t tamanho : tamanhos) {
            const Bytes entrada = sequencia(tamanho);
            exigir(decifrar_brbepo(cifrar_brbepo(entrada, rodada), rodada) == entrada,
                   "Falha BR-BEPO no tamanho " + std::to_string(tamanho) +
                   " e chave " + std::to_string(rodada));
        }
    }
}

void testar_aes() {
    AES256CBC aes;
    for (size_t tamanho : {size_t(0), size_t(1), size_t(15), size_t(16), size_t(17),
                           size_t(255), size_t(256), size_t(1024), size_t(65537)}) {
        const Bytes entrada = sequencia(tamanho);
        const Bytes cifrado = aes.cifrar(entrada);
        exigir(!cifrado.empty() && cifrado.size() % 16 == 0,
               "Saida AES invalida no tamanho " + std::to_string(tamanho));
        exigir(aes.decifrar(cifrado) == entrada,
               "Falha AES no tamanho " + std::to_string(tamanho));
    }
}

void testar_rsa() {
    RSA2048OAEP rsa;
    for (size_t tamanho : {size_t(0), size_t(1), size_t(189), size_t(190), size_t(191),
                           size_t(379), size_t(380), size_t(381), size_t(1000), size_t(4096)}) {
        const Bytes entrada = sequencia(tamanho);
        const Bytes cifrado = rsa.cifrar(entrada);
        const size_t blocos = tamanho == 0 ? 0 : (tamanho + 189) / 190;
        exigir(cifrado.size() == blocos * 256,
               "Tamanho de saida RSA inesperado para entrada " + std::to_string(tamanho));
        exigir(rsa.decifrar(cifrado) == entrada,
               "Falha RSA no tamanho " + std::to_string(tamanho));
    }
}
}

int main() {
    try {
        testar_chaves_brasileirao();
        testar_brbepo_exemplo();
        testar_brbepo_todas_chaves();
        testar_aes();
        testar_rsa();
        std::cout << "Todos os testes passaram.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "TESTE FALHOU: " << e.what() << '\n';
        return 1;
    }
}
