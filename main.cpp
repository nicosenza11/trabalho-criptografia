#include <iostream>
#include <fstream>
#include "encriptador.h"
// #include "descriptador.h"

int main() 
{
    int rodada = 5;
    
    auto tokens = ler_arquivo("texto.txt");
    auto blocos = agrupar_blocos(tokens);
    auto texto_cifrado = transpor_brasileirao(blocos, rodada);
    arquivo_encriptado(texto_cifrado, "arquivoencriptado.txt");

    return 0;
}