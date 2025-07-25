#include "../../include/pipes/Filho.h"
#include "../../include/models/Produto.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>
using namespace std;

Filho::Filho(int pipe_filho_para_pai_escrita)
    : fd_escrita_pai(pipe_filho_para_pai_escrita), produto(nullptr), pid_filho(getpid())
{
    cout << "Filho: Processo iniciado com PID " << pid_filho << endl;
}

Filho::~Filho()
{
    if (produto)
    {
        delete produto; // Clean up the Produto instance
    }
    close(fd_escrita_pai); // Close the pipe to the parent
    cout << "Filho: Processo finalizado." << endl;
}

void Filho::recebeDados()
{
    cout << "Filho: Aguardando dados do Produto..." << endl;
    string nome;
    double preco;

    cout << "Digite o nome do produto: ";
    cin.ignore(); // Clear the newline character from the input buffer
    getline(cin, nome);

    cout << "Digite o preço do produto: ";
    cin >> preco;

    // Create a new Produto instance
    produto = new Produto(nome.c_str(), preco);
    cout << "Filho: Produto recebido - Nome: " << produto->getNome() << ", Preço: " << produto->getPreco() << endl;
}

void Filho::enviaRespostas()
{
    if (produto)
    {
        podProduto pod = produto->getPod();

        // Transforma POD em binário para enviar via pipe

        std::vector<char> byte_array(sizeof(podProduto));
        std::memcpy(byte_array.data(), &pod, sizeof(podProduto));

        if (write(fd_escrita_pai, byte_array.data(), byte_array.size()) == -1)
        {
            perror("Filho: Erro ao enviar dados para o pai");
        }
        else
        {
            cout << "Filho: Resposta enviada ao pai." << endl;
        }
    }
}

void Filho::executa(int pipe_filho_para_pai_escrita, string executavel_filho)
{
    cout << "Filho: Iniciando terminal " << TERMINAL_EMULATOR << " com o executável " << executavel_filho << endl;
    execlp(TERMINAL_EMULATOR, TERMINAL_EMULATOR, TERMINAL_ARG,
           executavel_filho.c_str(),                       // Path to your actual child program
           to_string(pipe_filho_para_pai_escrita).c_str(), // Pass the write FD as a string argument
           NULL);                                          // MUST be NULL-terminated
    perror("execlp failed");
    exit(1);
}
