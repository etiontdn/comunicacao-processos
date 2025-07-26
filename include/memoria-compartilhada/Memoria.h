#ifndef MEMORIA_H
#define MEMORIA_H

#include <string>
#include <sys/mman.h>  // Para shm_open, ftruncate, mmap, munmap, shm_unlink
#include <fcntl.h>     // Para O_CREAT, O_RDWR
#include <unistd.h>    // Para ftruncate
#include <cstring>     // Para strlen, strcpy
#include <semaphore.h> // Para sem_open, sem_wait, sem_post, sem_unlink
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
    Memoria() {};
    ~Memoria() {};

    /// @brief Cria e abre a memória compartilhada.
    /// @return Retorna um ponteiro para a memória compartilhada ou nullptr em caso de erro
    void criarEAbrirMemoria()
    {
        // Implementação para criar e abrir a memória compartilhada
        // Retorna um ponteiro para a memória compartilhada
        return;
    }

    /// @brief Abre a memória compartilhada existente.
    /// @return Retorna um ponteiro para a memória compartilhada ou nullptr em caso de erro
    void abrirMemoria()
    {
        // Implementação para abrir a memória compartilhada
        // Retorna um ponteiro para acesso à memória compartilhada
        return;
    }

    void *getMemoria()
    {
        // Implementação para obter o ponteiro da memória compartilhada
        // Retorna um ponteiro para a memória compartilhada
        return nullptr;
    }

private:
    void *memoria;
    sem_t *semaforo;
};

#endif