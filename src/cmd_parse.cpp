#include <cstdlib>
#include <filesystem>
#include <cstring>
#include <set>
#include <string>
#include "cmd_parse.h"
#include "common.h"

namespace fs = std::filesystem;

void usage() 
{
    cout << "Usage: snaze [<options>] <input_level_file>\n";
    cout << "   Game simulation options:\n";
    cout << "     --help                Print this help text.\n";
    cout << "     --fps <num>           Number of frames (board) presented per second.\n";
    cout << "     --lives <num>         Number of lives the snake shall have. Default = 5.\n";
    cout << "     --food <num>          Number of food pellets for the entire simulation. Default = 10.\n";
    cout << "     --playertype <type>   Type of snake intelligence: random, backtracking. Default = backtracking.\n";
}

RunningOpt parse_cmd(int argc, char* argv[])
{
    RunningOpt runOpt; 

    std::set<std::string> flags { "--fps", "--lives", "--food", "--playertype" };

    // Checks if there is any invalid option in the input.
    for (int arg = 0; arg < argc; ++arg) {
        std::string flag = argv[arg];

        if (flag.find("--") != std::string::npos) {
            if (flags.find(flag) == flags.end()) {
                std::cerr << "snaze: unreconized command line option \'" << flag << "\'.\n\n";
                usage();
                exit(EXIT_FAILURE);
            }
        }
        else if (flag.find("-") != std::string::npos) {
            std::cerr << "snaze: unreconized command line option \'" << flag << "\'.\n\n";
            usage();
            exit(EXIT_FAILURE);
        }
    }

    for (int arg = 0; arg < argc; ++arg) {
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
                runOpt.player_type = player_e::BACKTRACKING; // Keeps the default value.
        }
    }

    return runOpt;
}
