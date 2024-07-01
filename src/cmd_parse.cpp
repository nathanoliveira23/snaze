#include <cstdlib>
#include <filesystem>
#include <cstring>
#include <set>
#include <string>
#include "cmd_parse.h"
#include "common.h"

namespace fs = std::filesystem;

/**
 * @brief Prints usage information for the snaze game simulation.
 * 
 * Prints usage instructions detailing command line options for running the snaze game simulation.
 */
void usage() 
{
    std::cout << "Usage: snaze [<options>] <input_level_file>\n";
    std::cout << "   Game simulation options:\n";
    std::cout << "     --help                Print this help text.\n";
    std::cout << "     --fps <num>           Number of frames (board) presented per second.\n";
    std::cout << "     --lives <num>         Number of lives the snake shall have. Default = 5.\n";
    std::cout << "     --food <num>          Number of food pellets for the entire simulation. Default = 10.\n";
    std::cout << "     --playertype <type>   Type of snake intelligence: random, backtracking. Default = backtracking.\n";
}

/**
 * @brief Parses command line arguments to configure the snaze game simulation.
 * 
 * Parses command line arguments to extract and configure options for the snaze game simulation,
 * including frame rate, number of lives, number of food pellets, and type of snake intelligence.
 * 
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 * @return A `RunningOpt` struct containing parsed options for running the simulation.
 */
RunningOpt parse_cmd(int argc, char* argv[])
{
    RunningOpt runOpt; 

    // Set of recognized command line flags.
    std::set<std::string> flags { "--fps", "--lives", "--food", "--playertype" };

    // Check for unrecognized command line options.
    for (int arg = 1; arg < argc; ++arg) {
        std::string flag = argv[arg];

        if (flag.find("--") != std::string::npos) {
            if (flags.find(flag) == flags.end()) {
                std::cerr << "snaze: unrecognized command line option \'" << flag << "\'.\n\n";
                usage();
                exit(EXIT_FAILURE);
            }
        }
        else if (flag.find("-") != std::string::npos) {
            std::cerr << "snaze: unrecognized command line option \'" << flag << "\'.\n\n";
            usage();
            exit(EXIT_FAILURE);
        }
    }

    // Parse command line arguments to set simulation options.
    for (int arg = 1; arg < argc; ++arg) {
        if (fs::is_regular_file(argv[arg])) {
            runOpt.level_path = argv[arg];
        }
        else if (!strcmp(argv[arg], "--fps")) {
            runOpt.fps = std::stoi(argv[arg + 1]);
        }
        else if (!strcmp(argv[arg], "--lives")) {
            runOpt.lives = std::stoi(argv[arg + 1]);
        }
        else if (!strcmp(argv[arg], "--food")) {
            runOpt.foods = std::stoi(argv[arg + 1]);
        }
        else if (!strcmp(argv[arg], "--playertype")) {
            if (!strcmp(argv[arg + 1], "backtracking"))
                runOpt.player_type = player_e::BACKTRACKING;
            else if (!strcmp(argv[arg + 1], "random"))
                runOpt.player_type = player_e::RANDOM;
            else
                runOpt.player_type = player_e::BACKTRACKING; // Default value if invalid type specified.
        }
    }

    return runOpt;
}
