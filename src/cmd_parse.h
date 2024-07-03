/*!
 * @file cmd_parse.h
 *
 * @brief Header file defining functions for parsing
 * command line arguments into configuration options.
 */

#ifndef  CMD_PARSE_H
#define CMD_PARSE_H

#include <iostream>
#include <string>
#include <optional>
#include <set>

#include "common.h"

using std::cout, std::endl, std::cerr;
using std::optional, std::nullopt;
using std::string;
using std::set;

// Set of recognized command line flags.
static const set<string> flags { "--fps", "--lives", "--food", "--playertype" };

/// Prints usage information for the snaze game simulation.
void usage();
/// Attempts to convert strings provided by the command line to a positive integer.
optional<unsigned> try_parse_int(std::string, void(*callback)(const std::string &));
/// Displays an error message prefixed with "snaze: " on the standard error stream.
void show_error(const std::string &);
///Parses command line arguments to configure the snaze game simulation.
optional<RunningOpt> parse_cmd(int argc, char* argv[]);

#endif
