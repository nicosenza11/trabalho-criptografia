#include <iostream>
#include <fstream>
#include <chrono>
#include "encriptador.h"
#include "descriptador.h"

int main() 
{
    int rodada;

    // Seleção da rodada do brasileirão
    std::cout << "Digite a rodada do Brasileirao 2002 (1 a 25) para usar como chave: ";
    std::cin >> rodada;
    
    while (std::cin.fail() || rodada < 1 || rodada > 25) 
    {
        std::cin.clear(); 
        std::cin.ignore(10000, '\n'); 
        
        std::cout << "Valor invalido! Por favor, digite um numero entre 1 e 25: ";
        std::cin >> rodada;
    }
    
    std::ofstream relatorio("tempos.csv", std::ios::app);
    // Encriptagem
    auto inicio_encript = std::chrono::high_resolution_clock::now();        

    auto tokens = ler_arquivo("texto.txt");
    auto blocos = agrupar_blocos(tokens);
    auto texto_cifrado = transpor_brasileirao(blocos, rodada);
    arquivo_encriptado(texto_cifrado, "arquivoencriptado.txt");

    auto fim_encript = std::chrono::high_resolution_clock::now();
    auto tempo_encript = std::chrono::duration_cast<std::chrono::microseconds>(fim_encript - inicio_encript).count();

    relatorio << "Tempo Encriptacao," << tempo_encript << "\n";

    // Descriptagem
    auto inicio_descript = std::chrono::high_resolution_clock::now(); 

    auto tokens_lidos = ler_arquivo_cifrado("arquivoencriptado.txt");
    auto blocos_cifrados = agrupar_blocos(tokens_lidos);
    auto tokens_ordenados = destranspor_brasileirao(blocos_cifrados, rodada);

    std::vector<char> texto;
    for (size_t i = 0; i < tokens_ordenados.size(); i++)
    {
        char byte_restaurado = reverter_byte_encriptado(tokens_ordenados[i]);
        texto.push_back(byte_restaurado);
    }

    arquivo_descriptado(texto, "arquivodescriptado.txt");

    auto fim_descript = std::chrono::high_resolution_clock::now(); 
    auto tempo_descript = std::chrono::duration_cast<std::chrono::microseconds>(fim_descript - inicio_descript).count();
    
    relatorio << "Tempo Descriptacao," << tempo_descript << "\n";
    relatorio.close();

    return 0;
}