#include <iostream>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include "setup.h"


std::vector<std::string> ler_arquivo_cifrado(std::string nome_arquivo)
{
    std::ifstream arquivo(nome_arquivo);
    std::vector<std::string> tokens;

    std::string token_atual;

    while (std::getline(arquivo, token_atual, '|'))
    {
        if (!token_atual.empty())
            tokens.push_back(token_atual);
    }
}

std::vector<std::string> destranspor_brasileirao(std::vector<std::vector<std::string>> blocos_cifrados, int rodada)
{
    std::vector<std::string> texto_separado;

    for (int i = 0; i < blocos_cifrados.size(); i++)
    {
        std::vector<std::string> bloco_atual = blocos_cifrados[i];

        int tamanho = bloco_atual.size();
        std::vector<std::string> bloco_original(tamanho);

        int indice_cifrado = 0;

        for (int j = 0; j < 26; j++)
        {
            int posicao_alvo = CHAVES_RODADAS[rodada - 1][j];

            if (posicao_alvo < tamanho)
            {
                bloco_original[posicao_alvo] = bloco_atual[indice_cifrado];
                indice_cifrado++; 
            }

        }

        for (int k = 0; k < tamanho; k++)
        {
            texto_separado.push_back(bloco_original[k]);
        }
    }

    return texto_separado;
}