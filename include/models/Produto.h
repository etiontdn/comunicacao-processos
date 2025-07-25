#ifndef PRODUTO_H
#define PRODUTO_H

#include <string>
using namespace std;

/**
 * @brief Estrutura para armazenar dados de um produto no formato POD (Plain old data).
 * Esta estrutura é usada para facilitar a serialização e deserialização dos dados.
 */
struct podProduto {
    char nome[128];
    double preco;
};

class Produto {
public:
    /// @brief Construtor da classe Produto
    /// @param nome Nome do produto
    /// @param preco Preço do produto
    Produto(const char* nome, double preco);

    /// @brief Construtor que recebe uma estrutura podProduto
    /// @param pod Estrutura contendo os dados do produto
    /// @note Este construtor é útil para converter de uma estrutura POD para a classe Produto
    Produto(const podProduto& pod);

    /// @brief Obtém o nome do produto
    string getNome() const;

    /// @brief Obtém o preço do produto
    double getPreco() const;

    /// @brief Serializa os dados do produto para uma estrutura podProduto
    podProduto getPod() const;

private:
    char nome[128]; // Nome do produto, tamanho máximo de 128 caracteres
    double preco; // Preço do produto
};

#endif