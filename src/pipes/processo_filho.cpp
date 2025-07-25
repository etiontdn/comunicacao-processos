#include <iostream>
#include <string>
#include <unistd.h> // For getpid(), read(), write(), close()
#include "../../include/pipes/Filho.h"

int main(int argc, char *argv[])
{
    // Espera que receba o descritor de arquivo de escrita do pipe como argumento
    if (argc < 2)
    {
        std::cerr << "Filho: Erro: Argumento de descritor de arquivo de escrita do pipe ausente." << std::endl;
        return 1;
    }
    int pipe_filho_para_pai_escrita = std::stoi(argv[1]); // Convert string argument to integer FD

    Filho filho(pipe_filho_para_pai_escrita);
    filho.recebeDados();    // Recebe dados do usuário e cria uma instância de Produto
    filho.enviaRespostas(); // Envia a resposta de volta ao pai
    return 0;
}