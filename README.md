# Introduction
In this project a simulation of the classic Snake Game was developed in which the snake is controlled only by AI using algorithms techniques such as backtracking.

# Requeriments
This application uses glyphs to render game icons. Therefore, it is necessary to use <code><b>Nerd Fonts</b></code> for a better viewing experience.

# Usage
### Compiling with CMake:
In the project root folder, use the following commands:
```
cmake -S . -B build
cmake --build build
```

### Compiling whith g++:
In the project root folder, use the following commands:
```
mkdir -p build
g++ -std=c++17 -Wall -pedantic src/*.cpp -I includes -I lib -o build/snaze
```
