cmake_minimum_required(VERSION 3.10)
project(ComunicacaoProcessos CXX)

add_library(models src/models/Produto.cpp)
add_library(pipes src/pipes/Filho.cpp src/pipes/Pai.cpp)
add_library(memoria-compartilhada src/memoria-compartilhada/Memoria.cpp)

target_link_libraries(pipes models)
target_link_libraries(memoria-compartilhada rt)

add_executable(main src/main.cpp)
add_executable(processo_pai src/pipes/processo_pai.cpp)
add_executable(processo_filho src/pipes/processo_filho.cpp)
add_executable(quadro_leitor src/memoria-compartilhada/quadro_leitor.cpp)
add_executable(escritor src/memoria-compartilhada/escritor.cpp)

target_link_libraries(processo_pai models pipes)
target_link_libraries(processo_filho models pipes)

target_link_libraries(quadro_leitor memoria-compartilhada)
target_link_libraries(escritor memoria-compartilhada)

