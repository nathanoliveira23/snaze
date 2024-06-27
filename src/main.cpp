#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unistd.h>
#include "cell.h"
#include "common.h"
#include "snake_game.h"
#include "level.h"
#include "player.h"

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
        game.render();
        sleep(1);
    }

    //snaze::Level level(m);

    //level.add_food();

    //cout << level.food().to_str() << endl;
    //cout << level.spawn().to_str() << endl;

    //cout << level.to_string() << endl;

    //snaze::Player player(level);

    //player.find_solution();

    //auto path = player.path_to_food();
    //auto grid = level.maze();

    //for (auto p : path) {
    //    grid[p.row][p.col] = snaze::Cell::cell_e::SPAWN;

    //    for (size_t r = 0; r < level.rows(); ++r) {
    //        for (size_t c = 0; c < level.cols(); ++c) {
    //            snaze::Cell cell = grid[r][c];

    //            if (cell.type() == snaze::Cell::cell_e::WALL) {
    //                cout << "█";
    //            } else if (cell.type() == snaze::Cell::cell_e::INV_WALL) {
    //                cout << ".";
    //            } else if (cell.type() == snaze::Cell::cell_e::FREE) {
    //                cout << " ";
    //            } else if (cell.type() == snaze::Cell::cell_e::FOOD) {
    //                cout << "F";
    //            } else if (cell.type() == snaze::Cell::cell_e::SPAWN) {
    //                cout << "&";
    //            } else if (cell.type() == snaze::Cell::cell_e::SNAKE_HEAD) {
    //                cout << "H";
    //            } else if (cell.type() == snaze::Cell::cell_e::SNAKE_BODY) {
    //                cout << "B";
    //            } 
    //        }
    //        cout << "\n";
    //    }
    //    sleep(1);
    //}

    //for (auto p : path) cout << p.to_str() << " - ";
    //cout << endl;

//    snaze::SnakeGame snaze_game(runOpt);
//    snaze_game.initialize(m);
//
//    if (read == READ_OK) {
//        snaze_game.render();
//    }
//    else {
//        std::cerr << "Falha ao ler arquivo\n";
//    }

    return EXIT_SUCCESS;
}
