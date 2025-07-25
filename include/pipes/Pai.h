#ifndef PAI_H
#define PAI_H

#include <string>
#include <vector>
#include <unistd.h>  
#include <sys/wait.h>
#include <iostream>  
#include <cstdio>
#include "../models/Produto.h" 
using namespace std;

#define TERMINAL_EMULATOR "xterm"
#define TERMINAL_ARG "-e"

/**
 * A classe Pai gerencia as funções do processo pai, incluindo: criação de processos filhos, comunicação via pipes, e monitoramento dos filhos.
**/
class Pai {
    public:
        /// @brief Construtor da classe Pai
        /// @param child_executable_path Caminho para o executável do processo filho
        Pai(const string& child_executable_path);
        ~Pai();

        /// @brief Checa a situação dos filhos, removendo aqueles que terminaram e fechando pipes.
        void monitoraFilhos();

        /// @brief Cria um novo processo filho, configurando pipes para comunicação.
        void criaFilho();
    private:
        pid_t pid_pai; // PID do processo pai
        vector<pid_t> pids_filhos; // Lista de PIDs dos processos filhos
        vector<int> fds_leitura; // Lista de FDs de leitura dos pipes para comunicação com os filhos
        string diretorio_executavel_filho; // Diretório do executável do processo filho
        string executavel_filho; // Nome do executável do processo filho
        vector<Produto> produtos; // Lista de produtos gerenciados pelo pai
};


#endif