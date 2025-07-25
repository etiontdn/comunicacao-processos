cmake_minimum_required(VERSION 3.10)
project(ComunicacaoProcessos CXX)

add_library(models src/models/Produto.cpp)

add_executable(main src/main.cpp)
add_executable(processo_pai src/processo_pai.cpp)
add_executable(processo_filho src/processo_filho.cpp)

target_link_libraries(processo_pai models)
target_link_libraries(processo_filho models)
