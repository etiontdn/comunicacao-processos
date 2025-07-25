#include "../../include/pipes/Pai.h"
#include "../../include/pipes/Filho.h"
#include <unistd.h>
#include <string>
#include <cstring>

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
        return -1;
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
        filhos.push_back(pid);                         // Adiciona o PID do filho à lista
        cout << "Pai: Filho criado com PID " << pid << endl;
        return 0;
    }

    return 0;
}

void Pai::monitoraFilhos()
{
    // --- Parent's continuous monitoring for child messages ---
    // This part runs in the main loop of the parent,
    // so it can receive messages from children even while waiting for new input.

    // Use a temporary vector to store FDs that are still open
    vector<int> fds_leitura_ativos;
    vector<pid_t> filhos_ativos;

    for (size_t i = 0; i < fds_leitura.size(); ++i)
    {
        int read_fd = fds_leitura[i];
        pid_t child_pid = filhos[i];

        // Check if there's data in the pipe without blocking
        fd_set fds;
        struct timeval timeout;

        FD_ZERO(&fds);
        FD_SET(read_fd, &fds);

        timeout.tv_sec = 0;      // Don't wait
        timeout.tv_usec = 10000; // 10ms timeout

        int ready = select(read_fd + 1, &fds, NULL, NULL, &timeout);

        if (ready > 0)
        {
            ssize_t bytes_read;

            // Lendo exatamente o tamanho da struct podProduto
            std::vector<char> raw_data_buffer(sizeof(podProduto));
            bytes_read = read(read_fd, raw_data_buffer.data(), sizeof(podProduto));

            if (bytes_read == -1)
            {
                perror("Pai: leitura do filho falhou durante a monitoração");
                close(read_fd);
            }
            else if (bytes_read == 0) // Finalizado
            {
                cout << "Pai: filho " << child_pid << " pipe fechado (EOF)." << endl;
                close(read_fd);
            }
            else if (bytes_read < (ssize_t)sizeof(podProduto))
            {
                // Mensagem incompleta (menos bytes do que o esperado para podProduto)
                std::cerr << "Pai: recebeu mensagem incompleta do filho " << child_pid
                          << " (" << bytes_read << " bytes esperados " << sizeof(podProduto) << ")." << std::endl;
                close(read_fd);
            }
            else // bytes_read == sizeof(podProduto)
            {
                cout << "Pai recebeu " << bytes_read << " bytes do filho " << child_pid << "." << endl;
                podProduto pod;

                // Copia os bytes lidos diretamente para a estrutura pod
                std::memcpy(&pod, raw_data_buffer.data(), sizeof(podProduto));

                Produto produto(pod);
                produtos.push_back(produto);

                // Após processar a mensagem, continue monitorando se este pipe ainda está ativo.
                // fds_leitura_ativos.push_back(read_fd);
                // filhos_ativos.push_back(child_pid);
            }
        }
        else if (ready == -1)
        {
            perror("select failed");
            close(read_fd); // Close bad FD
        }
        else
        { // ready == 0, no data, pipe still open
            fds_leitura_ativos.push_back(read_fd);
            filhos_ativos.push_back(child_pid);
        }
    }
    // Update the lists of active FDs and PIDs for the next iteration
    fds_leitura = fds_leitura_ativos;
    filhos = filhos_ativos;

    // Also reap any zombie children periodically (non-blocking waitpid)
    int status;
    pid_t terminated_pid;
    while ((terminated_pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        cout << "Parent: Reaped terminated child (terminal) " << terminated_pid << " with status " << status << endl;
    }
}

void Pai::exibeProdutos()
{
    cout << "\nLista de Produtos:" << endl;
    for (const Produto &produto : produtos)
    {
        cout << "Nome: " << produto.getNome() << ", Preço: " << produto.getPreco() << endl;
    }
}