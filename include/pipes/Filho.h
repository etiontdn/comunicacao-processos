#ifndef FILHO_H
#define FILHO_H

#include <string>
#include <unistd.h>
#include "../models/Produto.h" 
using namespace std;

/**
 * A classe Filho representa o processo filho que se comunica com o processo pai via pipes.
 * Ela é responsável por receber dados do Produto do usuário e enviar respostas de volta ao pai.
**/
class Filho {
    public:
        /// @brief Construtor da classe Filho
        Filho(int pipe_filho_para_pai_escrita);
        ~Filho();

        /// @brief Inicia o terminal xterm com o processo filho
        void executa();

        /// @brief Recebe dados do Produto do usuário e o transforma em uma instância da classe Produto
        void recebeDados();

        /// @brief Envia respostas de volta ao pai
        void enviaRespostas();

        /// @brief Finaliza o processo filho, fechando pipes e limpando recursos
        void finaliza();
    private:
        Produto *produto; // Produto sendo processado pelo filho
        int fd_escrita_pai; // File descriptor para escrita no pipe do pai
        pid_t pid_filho; // PID do processo filho
        string mensagem_resposta; // Mensagem de resposta a ser enviada ao pai
        char buffer[256]; // Buffer para leitura/escrita de dados
};


#endif