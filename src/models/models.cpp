#include "../../include/models/Produto.h"
#include <cstring>

Produto::Produto(const char* nome, double preco) {
    strncpy(this->nome, nome, sizeof(this->nome) - 1);
    this->nome[sizeof(this->nome) - 1] = '\0';
    this->preco = preco;
}

Produto::Produto(const podProduto& pod) {
    strncpy(this->nome, pod.nome, sizeof(this->nome) - 1);
    this->nome[sizeof(this->nome) - 1] = '\0';
    this->preco = pod.preco;
}

string Produto::getNome() const {
    return string(nome);
}

double Produto::getPreco() const {
    return preco;
}