cmake_minimum_required(VERSION 3.10)
project(ComunicacaoProcessos CXX)

add_library(models src/models/Produto.cpp)
add_library(pipes src/pipes/Filho.cpp src/pipes/Pai.cpp)

target_link_libraries(pipes models)

add_executable(main src/main.cpp)
add_executable(processo_pai src/pipes/processo_pai.cpp)
add_executable(processo_filho src/pipes/processo_filho.cpp)

target_link_libraries(processo_pai models pipes)
target_link_libraries(processo_filho models pipes)
