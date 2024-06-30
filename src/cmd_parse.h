#ifndef  CMD_PARSE_H
#define CMD_PARSE_H

#include <iostream>
#include "common.h"

using std::cout, std::endl;

void usage();
RunningOpt parse_cmd(int argc, char* argv[]);

#endif
