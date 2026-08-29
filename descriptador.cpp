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

    return tokens;
}

std::vector<std::string> destranspor_brasileirao(std::vector<std::vector<std::string>> blocos_cifrados, int rodada)
{
    std::vector<std::string> texto_separado;

    for (size_t i = 0; i < blocos_cifrados.size(); i++)
    {
        std::vector<std::string> bloco_atual = blocos_cifrados[i];

        int tamanho = bloco_atual.size();
        std::vector<std::string> bloco_original(tamanho);

        int indice_cifrado = 0;

        for (size_t j = 0; j < 26; j++)
        {
            int posicao_alvo = CHAVES_RODADAS[rodada - 1][j];

            if (posicao_alvo < tamanho)
            {
                bloco_original[posicao_alvo] = bloco_atual[indice_cifrado];
                indice_cifrado++; 
            }

        }

        for (size_t k = 0; k < (size_t)tamanho; k++)
        {
            texto_separado.push_back(bloco_original[k]);
        }
    }

    return texto_separado;
}

char reverter_byte_encriptado(std::string token)
{
    if (token[0] != 'X')
    {
        char letra_cifrada = token[1];
        
        for (size_t i = 0; i < 26; i++)
        {
            if (BEPO_LAYOUT[i] == letra_cifrada)
            {
                if (token[0] == 'U')
                        return static_cast<char>('A' + i);
                else if (token[0] == 'L') 
                        return static_cast<char>('a' + i);
            }
        }   
    }
    
    int valor = std::stoi(token.substr(1), nullptr, 16);
    return static_cast<char>(valor);
}

void arquivo_descriptado (std::vector<char> texto_final, std::string nome_saida)
{
    std::ofstream arquivo_saida(nome_saida, std::ios::binary);

    if (arquivo_saida.is_open())
    {
        for(size_t i = 0; i < texto_final.size(); i++)
        {
            arquivo_saida << texto_final[i];
        }
    }

    arquivo_saida.close();
}