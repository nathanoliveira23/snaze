#include <cstdlib>
#include <filesystem>
#include <cstring>
#include <stdexcept>

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
 * @brief Displays an error message prefixed with "snaze: " on the standard error stream.
 * 
 * This function outputs the provided error message prefixed with "snaze: " to the standard error stream (cerr).
 * It is typically used for displaying error messages in the context of the "snaze" program.
 * 
 * @param err The error message to display.
 */
void show_error(const std::string &err)
{
    cerr << "snaze: " << err << "\n\n";
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

    // Check for unrecognized command line options.
    for (int arg = 1; arg < argc; ++arg) {
        std::string flag = argv[arg];

        if (flag.find("--") != std::string::npos) {
            if (flags.find(flag) == flags.end()) {
                show_error("unrecognized command line option.");
                usage();
                exit(EXIT_FAILURE);
            }
        }
    }

    // Parse command line arguments to set simulation options.
    for (int arg = 1; arg < argc; ++arg) {
        if (fs::is_regular_file(argv[arg])) {
            runOpt.level_path = argv[arg];
        }
        else if (!strcmp(argv[arg], "--fps")) {
            if (arg + 1 < argc) {
                auto fps = try_parse_int(argv[arg + 1], show_error);

                if (fps.has_value())
                    runOpt.fps = fps.value();
                else
                    exit(EXIT_FAILURE);
            }
            else {
                show_error("Missing arguments for --fps.");
                usage();
                exit(EXIT_FAILURE);
            }
        }
        else if (!strcmp(argv[arg], "--lives")) {
            if (arg + 1 < argc) {
                auto lives = try_parse_int(argv[arg + 1], show_error);

                if (lives.has_value())
                    runOpt.lives = lives.value();
                else
                    exit(EXIT_FAILURE);
            }
            else {
                show_error("Missing arguments for --lives.");
                usage();
                exit(EXIT_FAILURE);
            }
        }
        else if (!strcmp(argv[arg], "--food")) {
            if (arg + 1 < argc) {
                auto foods = try_parse_int(argv[arg + 1], show_error);

                if (foods.has_value())
                    runOpt.fps = foods.value();
                else
                    exit(EXIT_FAILURE);
            }
            else {
                show_error("Missing arguments for --food.");
                usage();
                exit(EXIT_FAILURE);
            }
        }
        else if (!strcmp(argv[arg], "--playertype")) {
            if (arg + 1 < argc) {
                if (!strcmp(argv[arg + 1], "backtracking")) {
                    runOpt.player_type = player_e::BACKTRACKING;
                }
                else if (!strcmp(argv[arg + 1], "random")) {
                    runOpt.player_type = player_e::RANDOM;
                }
                else {
                    show_error("\'" + std::string(argv[arg+1]) + "\' is not a valid argument.");
                    usage();
                    exit(EXIT_FAILURE);
                }
            }
            else {
                show_error("Missing arguments for --foods.");
                exit(EXIT_FAILURE);
            }
        }
    }

    return runOpt;
}

/**
 * @brief Tries to parse a string into an optional non-negative integer value (`unsigned`).
 * 
 * This function attempts to convert the provided string into an `unsigned` integer value.
 * If the conversion succeeds and the value is non-negative, it returns the converted value wrapped in `std::optional`.
 * If the conversion fails (e.g., due to invalid characters or out-of-range values), it calls the provided callback function
 * with an error message and returns `std::nullopt`.
 * 
 * @param str The string containing the value to convert.
 * @param callback The callback function to call in case of an error. Its signature should be `void(const std::string&)`.
 * @return std::optional<unsigned> The converted value wrapped in `std::optional`, or `std::nullopt` if conversion fails.
 * 
 * @throws std::invalid_argument If the string cannot be converted to an `unsigned` integer.
 * @throws std::out_of_range If the converted value is out of the range of `unsigned`.
 */
optional<unsigned> try_parse_int(std::string str, void (*callback)(const std::string &))
{
    unsigned result; // Variable to hold the converted unsigned integer.
    
    try {
        size_t pos; // Position indicator for std::stoi.
        int temp = std::stoi(str, &pos); // Convert string to integer.

        // Check if there are any remaining characters after the conversion.
        if (pos != str.size()) {
            callback("\'" + str + "\' is not a valid integer argument type."); // Notify callback of conversion failure.
            return nullopt; // Return empty optional to signal failure.
        }

        // Check if the converted integer is negative (invalid for unsigned).
        if (temp < 0) {
            callback("\'" + str + "\' is not a positive number."); // Notify callback of negative value.
            return nullopt; // Return empty optional to signal failure.
        }

        result = static_cast<unsigned>(temp); // Cast integer to unsigned.
    }
    catch (const std::invalid_argument &err) {
        callback("\'" + str + "\' is not a valid integer argument type."); // Notify callback of conversion failure.
        return nullopt; // Return empty optional to signal failure.
    }
    catch (const std::out_of_range &err) {
        callback("\'" + str + "\' is out of range for an integer."); // Notify callback of out-of-range error.
        return nullopt; // Return empty optional to signal failure.
    }

    return result; // Return the successfully converted unsigned integer.
}
