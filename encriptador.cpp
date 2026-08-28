#include <iostream>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include "setup.h"


std::string substituir_byte(char c) 
{
    int indice;

    if (c >= 'A' && c <= 'Z')
    {
        indice = c - 'A';
        return std::string("U") + BEPO_LAYOUT[indice];
    }
    else if (c >= 'a' && c <= 'z')
    {
        indice = c - 'a';
        return std::string ("L") + BEPO_LAYOUT[indice];
    }
    else
    {
        char hex_str[4];
        snprintf(hex_str, sizeof(hex_str), "X%02X", (unsigned char)c);
        return std::string(hex_str);
    }
}

std::vector<std::string> ler_arquivo(std::string nome_arquivo)
{
    std::ifstream arquivo(nome_arquivo, std::ios::binary);

    if (arquivo.is_open())
    {
        char byte_lido;
        std::vector<std::string> texto_bytes;

        while (arquivo.get(byte_lido))
        {
            texto_bytes.push_back(substituir_byte(byte_lido) + "|");
        }

        return texto_bytes;
    }
    else
    {
        printf("Arquivo não aberto corretamente");
        return {};
    }
}

std::vector<std::vector<std::string>> agrupar_blocos(std::vector<std::string> texto_bytes)
{
    std::vector<std::vector<std::string>> blocos;

    for (int i = 0; i < texto_bytes.size(); i += 26)
    {
        std::vector<std::string> bloco_atual;

        for (int j = 0; j < 26; j++)
        {
            if (i + j < texto_bytes.size())
                bloco_atual.push_back(texto_bytes[i + j]);
        }

        blocos.push_back(bloco_atual);
    }

    return blocos;
}

std::vector<std::string> transpor_brasileirao(std::vector<std::vector<std::string>> blocos_texto, int rodada_chave)
{
    std::vector<std::string> texto_final;

    for (int i = 0; i < blocos_texto.size(); i++)
    {
        std::vector<std::string> bloco_atual = blocos_texto[i];
        int tamanho_bloco = bloco_atual.size();

        for (int j = 0; j < 26; j++)
        {
            int posicao_alvo = CHAVES_RODADAS[rodada_chave - 1][j];

            if (posicao_alvo < tamanho_bloco)
                texto_final.push_back(bloco_atual[posicao_alvo]);
        }
        
    }

    return texto_final;
}

void arquivo_encriptado (std::vector<std::string> texto_final, std::string nome_saida)
{
    std::ofstream arquivo_saida(nome_saida);

    if (arquivo_saida.is_open())
    {
        for(int i = 0; i < texto_final.size(); i++)
        {
            arquivo_saida << texto_final[i];
        }
    }

    arquivo_saida.close();
}
