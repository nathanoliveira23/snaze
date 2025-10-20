# Introduction

Este projeto consiste em uma versão CLI do classico Snake Game. Entretanto, a cobra, personagem do jogo, é controlado por uma AI engine que decide o menor caminho desde sua origem até a comida. Para isso, utiliza-se algoritmos de busca como, por exemplo, a busca em largura (BFS).

# Author(s)

- Nome: Ronald Nathan Silva de Oliveira
- Email: nathanoliveira399@gmail.com
- Turma: DIM0176 - T01 - Programação I

# Problems found or limitations

- Em alguns casos, em determinado estado do labirinto, ao obter o caminho minimo o corpo da cobra pode estar bloqueando sua passagem. Devido a isso a cobra morre no meio do caminho sem haver colisão.
- Atualização de tela sem flickering.

# Grading

| Item                                                                                      | Valor máximo | Valor esperado |
| ----------------------------------------------------------------------------------------- | :----------: | :------------: |
| Correctly reads and validates a level input file.                                         |      20      |       20       |
| Correctly determines a set of directions that guides the snake to the food pellet;        |      50      |       50       |
  or make random suggestions in case there is no solution.                                  
| Correctly display the various game state.                                                 |      30      |       30       |


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
