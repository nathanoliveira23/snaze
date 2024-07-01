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

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "common.h"
#include "snake_game.h"
#include "cmd_parse.h"

#define READ_OK 0
#define READ_FAILED 1

/**
 * @brief Reads maze configuration from a file.
 * 
 * This function reads a maze configuration from the specified file path.
 * The maze is expected to be formatted with the first line containing the number of rows
 * and columns, followed by the maze grid itself. Each character in the grid represents
 * a type of cell in the maze.
 * 
 * @param path The file path to read the maze configuration from.
 * @param maze Output parameter where the maze configuration will be stored.
 *             Each vector represents a row in the maze grid.
 * 
 * @return int Returns READ_OK (0) if the maze is read successfully, or READ_FAILED (1) if there
 *             was an error reading the file.
 */
int read_file(const std::string &path, std::vector<std::vector<char>> &maze)
{
    std::ifstream fin(path);

    if (!fin.is_open()) return READ_FAILED; // Unable to open file.

    size_t rows, cols;
    fin >> rows >> cols; // Read number of rows and columns.

    fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore rest of the first line.

    std::string line;
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

    std::vector<vector<char>> level;

    int read = read_file(runOpt.level_path, level);

    snaze::SnakeGame snaze(runOpt);
    snaze.initialize(level);

    while (not snaze.game_over()) {
        snaze.process_events();
        snaze.update();
        refresh();
        snaze.render();
        wait(snaze.fps());
    }

    return EXIT_SUCCESS;
}
