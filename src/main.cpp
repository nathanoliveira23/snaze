#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unistd.h>
#include "common.h"
#include "snake_game.h"
#include "cmd_parse.h"

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

inline void refresh() 
{
    cout << "\033[2J\033[H";
}

int main(int argc, char* argv[])
{
    if (argc == 1 or !strcmp(argv[1], "--help")) {
        usage();
        return EXIT_SUCCESS;
    }

    RunningOpt runOpt = parse_cmd(argc, argv);
    std::vector<vector<char>> level;

    int read = read_file(runOpt.level_path, level);

    snaze::SnakeGame snaze(runOpt);
    snaze.initialize(level);

    while (not snaze.game_over()) {
        snaze.process_events();
        snaze.update();
        refresh();
        snaze.render();
        usleep(50000);
    }

    return EXIT_SUCCESS;
}
