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

/**
 * A classe Pai gerencia as funções do processo pai, incluindo: criação de processos filhos, comunicação via pipes, e monitoramento dos filhos.
 **/
class Pai
{
public:
    /// @brief Construtor da classe Pai
    /// @param executavel_filho Caminho para o executável do processo filho
    Pai(const string &executavel_filho);
    ~Pai();

    /// @brief Checa a situação dos filhos, removendo aqueles que terminaram e fechando pipes.
    void monitoraFilhos();

    /// @brief Cria um novo processo filho, configurando pipes para comunicação.
    /// @return 0 em caso de sucesso, -1 em caso de erro.
    int criaFilho();

    /// @brief Exibe a lista de produtos gerenciados pelo pai.
    void exibeProdutos();

private:
    pid_t pid_pai;            // PID do processo pai
    vector<pid_t> filhos;     // Lista de PIDs dos filhos
    vector<int> fds_leitura;  // Lista de FDs de leitura dos pipes para comunicação com os filhos
    string executavel_filho;  // Nome do executável do processo filho
    vector<Produto> produtos; // Lista de produtos gerenciados pelo pai
};

#endif