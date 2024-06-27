#ifndef COMMON_H
#define COMMON_H

#include <cstddef>

namespace snaze {

struct Position {
    size_t row, col;

    Position(size_t r = 0, size_t c = 0) : row(r), col(c) { /* empty */}
};

} //NAMESPACE SNAZE

enum player_e {
    RANDOM = 0,
    BACKTRACKING,
};

struct RunningOpt {
    unsigned fps = 2;       //!< Default fps value.
    unsigned lives = 5;     //!< Default # of lives the snake shall have.
    unsigned foods = 10;    //!< Default # of food pellets for the entire simulation.
    player_e player_type = player_e::BACKTRACKING; //!< Default player type.
};

#endif
