/*!
 * @file cmd_parse.h
 *
 * @brief Header file defining functions for parsing
 * command line arguments into configuration options.
 */

#ifndef  CMD_PARSE_H
#define CMD_PARSE_H

#include <iostream>
#include "common.h"

using std::cout, std::endl;

/// Prints usage information for the snaze game simulation.
void usage();
///Parses command line arguments to configure the snaze game simulation.
RunningOpt parse_cmd(int argc, char* argv[]);

#endif
