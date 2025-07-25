#include <iostream>
#include <vector>
#include <string>
#include "../../include/pipes/Pai.h"

using namespace std;

void menuPrincipal()
{
    cout << "\n--- Menu Principal ---" << endl;
    cout << "1: Criar novo produto" << endl;
    cout << "2: Mostrar produtos" << endl;
    cout << "q: Finalizar" << endl;
    cout << "Escolha uma opção: ";
}

int main()
{
    string executavel_filho = "./processo_filho";
    Pai pai(executavel_filho);

    string input;
    menuPrincipal();
    while (true)
    {
        cout << "\nPai> ";
        cin >> input;

        pai.monitoraFilhos();

        if (input == "1")
        {
            int sucesso = pai.criaFilho();
            if (sucesso == -1)
            {
                cout << "Pai: Erro ao criar filho, finalizando." << endl;
                break;
            }
        }
        else if (input == "2")
        {
            pai.exibeProdutos();
        }
        else if (input == "q")
        {
            cout << "Pai: Finalizando." << endl;
            break;
        }
        else
        {
            cout << "Pai: Entrada inválida." << endl;
            menuPrincipal();
        }
    }

    return 0;
}