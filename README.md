# Comunicação Entre Processos (IPC)

Este repositório contém exemplos práticos de **Comunicação Entre Processos (IPC)** utilizando **Pipes** e **Memória Compartilhada** em C++.


## Pré-requisitos

Para compilar e executar os exemplos, você precisará dos seguintes pacotes:

  * `cmake`
  * `build-essential` (para compilador C++ e ferramentas básicas de build)
  * `xterm` (terminal gráfico para o exemplo de Pipes)
  * `xfonts-scalable` (fontes para `xterm`)
  * `xfonts-base` (fontes para `xterm`)

**Observação:** A maioria das distribuições Linux já inclui esses pacotes por padrão.

### Compatibilidade

Este projeto foi desenvolvido e testado no **WSL2 (Windows Subsystem for Linux 2)**, mas deve funcionar sem problemas em qualquer sistema operacional baseado em Linux.

-----

## Como Compilar o Projeto

Siga os passos abaixo para compilar os exemplos:

1.  **Crie o diretório de build:**
    ```bash
    mkdir build
    ```
2.  **Acesse o diretório de build:**
    ```bash
    cd build
    ```
3.  **Configure o projeto com CMake:**
    ```bash
    cmake ..
    ```
4.  **Compile o código:**
    ```bash
    make
    ```

Após a compilação, os executáveis estarão disponíveis no diretório `build`.

-----

## Como Executar os Exemplos

### 1\. Exemplo de Pipes

Para iniciar a comunicação via **Pipes**, execute o processo pai:

```bash
./processo_pai
```

Este comando iniciará o processo principal, que, por sua vez, pode abrir terminais `xterm` adicionais para os processos filhos.

### 2\. Exemplo de Memória Compartilhada

O exemplo de memória compartilhada envolve um processo leitor e um ou mais processos escritores.

  * **Inicie o processo leitor (uma única instância):**

    ```bash
    ./quadro_leitor
    ```

  * **Inicie quantos processos escritores desejar (em terminais separados):**

    ```bash
    ./escritor
    ```

    Você pode abrir múltiplas instâncias de `./escritor` para ver a comunicação simultânea através da memória compartilhada.

-----