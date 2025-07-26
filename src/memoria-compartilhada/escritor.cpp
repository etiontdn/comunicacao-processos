#include "../../include/memoria-compartilhada/Memoria.h"
#include <iostream>
#include <cstring>
#include <string>
using namespace std;

int main() {
    Memoria memoria;
    memoria.abrirMemoria();
    memoria.abrirSemaforo();

    // Obtém o ponteiro da memória compartilhada
    void *ptr = memoria.getMemoria();
    if (ptr != nullptr) {
        cout << "Memória compartilhada acessada com sucesso." << endl;
    } else {
        cerr << "Falha ao acessar a memória compartilhada." << endl;
    }

    // Obtém o ponteiro do semáforo
    sem_t *semaforo = memoria.getSemaforo();
    if (semaforo != nullptr) {
        cout << "Semáforo acessado com sucesso." << endl;
    } else {
        cerr << "Falha ao acessar o semáforo." << endl;
    }

    string mensagem;
    cout << "Digite uma mensagem para escrever na memória compartilhada: ";
    getline(cin, mensagem);

    sem_wait(semaforo); // Aguarda o semáforo para garantir acesso exclusivo à memória compartilhada

    if (ptr != nullptr) {
        strncpy(static_cast<char *>(ptr), mensagem.c_str(), SHM_SIZE);
        cout << "Mensagem escrita na memória compartilhada: " << mensagem << endl;
    } else {
        cerr << "Falha ao escrever na memória compartilhada." << endl;
    }

    sem_post(semaforo); // Libera o semáforo após a escrita

    return 0;
}