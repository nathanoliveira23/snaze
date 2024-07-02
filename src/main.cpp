/**
 * @file main.cpp
 *
 * @description
 * This program simulates the classic snake game controlled 
 * by AI. 
 * It reads an input file that represents the maze where the 
 * snake will move.
 *
 * @author  Ronald Nathan Silva de Oliveira
 */

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <list>
#include "common.h"
#include "snake_game.h"
#include "cmd_parse.h"

#define READ_OK 0
#define READ_FAILED 1

using maze = std::vector<std::vector<char>>;

/**
 * @brief Reads mazes from a file and stores them in a list.
 *
 * This function reads a file containing multiple mazes, each defined by
 * its number of rows and columns followed by the maze layout. Each maze is
 * stored as a vector of vectors of characters, representing the grid layout.
 * The function stores these mazes in a list.
 *
 * @param path The path to the input file containing maze definitions.
 * @param mazes A reference to a list where the read mazes will be stored.
 * @return READ_OK if the file is read successfully, READ_FAILED otherwise.
 */
int read_file(const std::string &path, std::list<maze> &mazes)
{
    std::ifstream fin(path);

    if (not fin.is_open()) return READ_FAILED; // Unable to open file.

    while (true) {
        size_t rows, cols;
        if (not (fin >> rows >> cols)) {
            if (fin.eof()) break;   // End of file reached.

            return READ_FAILED;     // Error reading dimensions.
        }

        fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore rest of the first line.

        std::string line;
        maze maze;
        for (size_t r = 0; r < rows; ++r) {
            std::vector<char> buff;

            if (std::getline(fin, line)) { // Read each line of the maze.
                for (const char &c : line) {
                    buff.push_back(c); // Store each character in the row vector.
                }
                maze.push_back(buff); // Store the row vector in the maze.
            } 
            else {
                return READ_FAILED; // Error reading line.
            }
        }

        mazes.push_back(maze);
    }

    return READ_OK; // Successfully read maze.
}

/**
 * @brief Clears the console screen and moves the cursor to the top-left corner.
 * 
 * This function uses ANSI escape sequences to clear the console screen and
 * move the cursor to the top-left corner.
 */
inline void refresh() 
{
    cout << "\033[2J\033[H"; // ANSI escape sequence to clear screen and move cursor
}

/**
 * @brief Pauses the execution for a specified duration.
 * 
 * This function pauses the execution for the specified amount of time in milliseconds.
 * It uses std::this_thread::sleep_for to suspend the execution of the current thread.
 * 
 * @param time The duration in milliseconds to pause execution.
 */
inline void wait(size_t time) 
{
    constexpr size_t SECOND = 1000; // Number of milliseconds in a second. (1s)
    size_t t = SECOND / time;       // Calculate sleep time.

    std::this_thread::sleep_for(std::chrono::milliseconds(t)); // Pause execution.
}

int main(int argc, char* argv[])
{
    if (argc == 1 or !strcmp(argv[1], "--help")) {
        usage();
        return EXIT_SUCCESS;
    }

    RunningOpt runOpt = parse_cmd(argc, argv);

    std::list<maze> levels;

    int read = read_file(runOpt.level_path, levels);

    if (read == READ_FAILED) {
        std::cerr << "Unable to read file: " << strerror(errno) << ".\n";
        return EXIT_FAILURE;
    }

    snaze::SnakeGame snaze(runOpt);
    snaze.initialize(levels);

    while (not snaze.game_over()) {
        snaze.process_events();
        snaze.update();
        refresh();
        snaze.render();
        wait(snaze.fps());
    }

    return EXIT_SUCCESS;
}
