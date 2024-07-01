# Introduction

Este projeto consiste em um jogo da forca versão CLI na qual, a partir de um arquivo .csv contendo várias palavras e suas categorias, essas palavras são sorteadas de acordo com o nível de dificuldade escolhida para a partida. Esse jogo foi desenvolvido utilizando Game Loop como padrão de projeto.

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
