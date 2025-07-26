#include "../../include/memoria-compartilhada/Memoria.h"
#include <iostream>
using namespace std;

void Memoria::criarEAbrirMemoria()
{
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        cerr << "Erro ao criar ou abrir memória compartilhada." << endl;
        return;
    }
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        cerr << "Erro ao definir o tamanho da memória compartilhada." << endl;
        return;
    }
    memoria = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (memoria == MAP_FAILED)
    {
        cerr << "Erro ao mapear a memória compartilhada." << endl;
        return;
    }
}

void Memoria::criarEAbrirSemaforo()
{
    semaforo = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (semaforo == SEM_FAILED)
    {
        cerr << "Erro ao criar ou abrir semáforo." << endl;
        return;
    }
}

void Memoria::abrirMemoria()
{
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1)
    {
        cerr << "Erro ao abrir memória compartilhada." << endl;
        return;
    }
    memoria = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (memoria == MAP_FAILED)
    {
        cerr << "Erro ao mapear a memória compartilhada." << endl;
        return;
    }
}

void Memoria::abrirSemaforo()
{
    semaforo = sem_open(SEM_NAME, 0);
    if (semaforo == SEM_FAILED)
    {
        cerr << "Erro ao abrir semáforo." << endl;
        return;
    }
}

void *Memoria::getMemoria()
{
    return memoria;
}

void Memoria::fecharSemaforo()
{
    if (sem_close(semaforo) == -1)
    {
        cerr << "Erro ao fechar semáforo." << endl;
    }
    sem_unlink(SEM_NAME); // Desvincula o semáforo
}

void Memoria::liberarMemoria()
{
    if (munmap(memoria, SHM_SIZE) == -1)
    {
        cerr << "Erro ao liberar memória compartilhada." << endl;
    }
    if (shm_unlink(SHM_NAME) == -1)
    {
        cerr << "Erro ao desvincular memória compartilhada." << endl;
    }
}

Memoria::Memoria() : shm_fd(-1), memoria(nullptr), semaforo(nullptr)
{
}

Memoria::~Memoria()
{
    if (memoria != nullptr)
    {
        liberarMemoria();
    }
    if (semaforo != nullptr)
    {
        fecharSemaforo();
    }
    if (shm_fd != -1)
    {
        close(shm_fd);
    }
}