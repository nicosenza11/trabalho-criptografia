#include <vector>
#include <string>

std::string substituir_byte(char c);
std::vector<std::string> ler_arquivo(std::string nome_arquivo);
std::vector<std::vector<std::string>> agrupar_blocos(std::vector<std::string> texto_bytes);
std::vector<std::string> transpor_brasileirao(std::vector<std::vector<std::string>> blocos_texto, int rodada_chave);
void arquivo_encriptado (std::vector<std::string> texto_final, std::string nome_saida);