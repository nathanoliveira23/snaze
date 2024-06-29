#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unistd.h>
#include "common.h"
#include "snake_game.h"

#define READ_OK 0
#define READ_FAILED 1

int read_file(const std::string &path, std::vector<vector<char>> &maze)
{
    std::ifstream fin(path);

    if (!fin.is_open()) return READ_FAILED;

    size_t rows, cols;
    fin >> rows >> cols;

    fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line;
    for (size_t r = 0; r < rows; ++r) {
        std::vector<char> buff;

        if (std::getline(fin, line)) {
            for (const char &c : line) {
                buff.push_back(c);
            }
            maze.push_back(buff);
        } 
        else {
            return READ_FAILED;
        }
    }

    return READ_OK;
}

int main(int argc, char* argv[])
{
    RunningOpt runOpt;
    std::vector<vector<char>> level;

    int read = read_file(argv[1], level);

    snaze::SnakeGame game(runOpt);
    game.initialize(level);

    while (true) {
        game.process_events();
        game.update();
        //system("clear");
        game.render();
        usleep(70000);
        //sleep(1);
    }

    return EXIT_SUCCESS;
}
