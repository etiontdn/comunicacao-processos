#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>
using namespace std;

struct podProduto {
    char nome[128];
    double preco;
};

class Produto {
public:
    Produto(const char* nome, double preco);
    Produto(const podProduto& pod);
    string getNome() const;
    double getPreco() const;

private:
    char nome[128];
    double preco;
};

#endif