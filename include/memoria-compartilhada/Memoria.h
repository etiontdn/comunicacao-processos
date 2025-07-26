#ifndef MEMORIA_H
#define MEMORIA_H

#include <string>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <semaphore.h>
using namespace std;

#define SHM_SIZE 256
#define SHM_NAME "/memoria_compartilhada"
#define SEM_NAME "/semaforo_memoria"
/**
 * @brief Classe para gerenciar a memória compartilhada.
 * Esta classe é responsável por criar, acessar e manipular a memória compartilhada
 * usada para comunicação entre processos.
 */
class Memoria
{
public:
    Memoria();
    ~Memoria();

    /// @brief Cria e abre a memória compartilhada para leitura e escrita.
    void criarEAbrirMemoria();

    /// @brief Cria um semáforo para sincronização de acesso à memória compartilhada.
    void criarEAbrirSemaforo();

    /// @brief Abre a memória compartilhada existente para leitura e escrita.
    void abrirMemoria();

    /// @brief Abre o semáforo existente para sincronização de acesso à memória compartilhada.
    void abrirSemaforo();

    /// @brief Obtém o ponteiro da memória compartilhada.
    /// @return Retorna um ponteiro para a memória compartilhada
    void *getMemoria();

    /// @brief Obtém o ponteiro do semáforo.
    /// @return Retorna um ponteiro para o semáforo
    sem_t *getSemaforo();

    /// @brief Fecha o semáforo
    void fecharSemaforo();

    /// @brief Fecha a memória compartilhada e libera os recursos alocados do sistema
    void liberarMemoria();

private:
    int shm_fd;      // Descritor de arquivo para a memória compartilhada
    void *memoria;   // Ponteiro para a memória compartilhada
    sem_t *semaforo; // Ponteiro para o semáforo de sincronização
};

#endif