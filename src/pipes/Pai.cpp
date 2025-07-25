#include "../../include/pipes/Pai.h"
#include "../../include/pipes/Filho.h"
#include <unistd.h>

using namespace std;

Pai::Pai(const string &executavel_filho)
    : pid_pai(getpid()), executavel_filho(executavel_filho)
{
    cout << "Pai: Processo iniciado com PID " << pid_pai << endl;
}

Pai::~Pai()
{
    // Close all pipes and clean up resources
    for (int fd : fds_leitura)
    {
        close(fd);
    }
    fds_leitura.clear();
    cout << "Pai: Processo finalizado." << endl;
}

int Pai::criaFilho()
{
    cout << "Pai: Criando filho " << filhos.size() + 1 << endl;

    int pipe_filho_para_pai[2];
    if (pipe(pipe_filho_para_pai) == -1)
    {
        perror("Pai: Erro ao criar pipe");
        return;
    }

    pid_t pid = fork();
    // Erro ao criar o processo filho
    if (pid == -1)
    {
        perror("Pai: Erro ao criar processo filho");
        close(pipe_filho_para_pai[0]);
        close(pipe_filho_para_pai[1]);
        return -1;
    }
    // Processo filho
    else if (pid == 0)
    {
        Filho::executa(pipe_filho_para_pai[1], executavel_filho);
    }

    // Processo pai
    else
    {
        close(pipe_filho_para_pai[1]);                 // Fecha o lado de escrita do pipe no pai
        fds_leitura.push_back(pipe_filho_para_pai[0]); // Adiciona o FD de leitura do pipe à lista
        filhos.push_back(pid);                           // Adiciona o PID do filho à lista
        cout << "Pai: Filho criado com PID " << pid << endl;
        return 0;
    }
}
