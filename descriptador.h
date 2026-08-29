#include <vector>
#include <string>

std::vector<std::string> ler_arquivo_cifrado(std::string nome_arquivo);
std::vector<std::string> destranspor_brasileirao(std::vector<std::vector<std::string>> blocos_cifrados, int rodada);
char reverter_byte_encriptado(std::string token);
void arquivo_descriptado (std::vector<char> texto_final, std::string nome_saida);