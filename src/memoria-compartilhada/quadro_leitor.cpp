#include "../../include/memoria-compartilhada/Memoria.h"
#include <iostream>
#include <cstring>
#include <string>
#include <atomic>
#include <csignal>
#include <cstdlib>
using namespace std;

atomic<bool> interrupcao(false);

/// @brief Manipulador de sinal para interrupções, no caso SIGINT e SIGTERM,
///        para garantir que o espaço de memória compartilhada seja liberado corretamente.
/// @param signal O sinal recebido
void sinalInterrupcao(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        cout << "Interrupção recebida. Encerrando..." << endl;
        interrupcao.store(true);
    }
}

/// @brief Leitor de quadro de memória compartilhada, inicia a memória compartilhada e o semáforo,
///        tenta ler a mensagem da memória compartilhada para atualizar o quadro de mensagem a cada 5 segundos.
/// @return Código de saída
int main()
{
    signal(SIGINT, sinalInterrupcao);
    signal(SIGTERM, sinalInterrupcao);

    Memoria memoria;
    memoria.criarEAbrirMemoria();
    memoria.criarEAbrirSemaforo();

    string mensagem = "Mensagem inicial";
    // Copia a mensagem inicial para a memória compartilhada
    void *ptr = memoria.getMemoria();
    if (ptr != nullptr)
    {
        strncpy(static_cast<char *>(ptr), mensagem.c_str(), SHM_SIZE);
        cout << "Memória compartilhada criada e acessada com sucesso." << endl;
    }

    sem_t *semaforo = memoria.getSemaforo();


    while (true)
    {
        if (interrupcao.load())
        {
            break;
        }
        // Aguarda 5 segundos antes de ler novamente
        sleep(5);

        sem_wait(semaforo); // Aguarda o semáforo para garantir acesso exclusivo à memória compartilhada

        // Lê a mensagem da memória compartilhada
        char *mensagem = static_cast<char *>(memoria.getMemoria());
        if (mensagem != nullptr)
        {
            system("clear");
            cout << "Mensagem lida: " << mensagem << endl;
        }

        sem_post(semaforo); // Libera o semáforo após a leitura
    }

    cout << "Liberando memória e fechando semáforo..." << endl;
    memoria.liberarMemoria();
    memoria.fecharSemaforo();
    return 0;
}