# Introduction

Este projeto consiste em uma versão CLI do classico Snake Game. Entretanto, a cobra, personagem do jogo, é controlado por uma AI engine que decide o menor caminho desde sua origem até a comida. Para isso, utiliza-se algoritmos de busca como, por exemplo, a busca em largura (BFS).

# Author(s)

- Nome: Ronald Nathan Silva de Oliveira
- Email: nathanoliveira399@gmail.com
- Turma: DIM0176 - T01 - Programação I

# Problems found or limitations

- Encontrar o caminho a cada nova posição;
- Atualização de tela sem flickering.

# Compiling and Runnig

- Com Cmake:
```
cmake -S . -B build
cmake --build build
```

- Com G++:
```
mkdir -p build
g++ -std=c++17 -Wall -pedantic src/*.cpp -I includes -I lib -o build/snaze
```
