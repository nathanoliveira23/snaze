#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "common.h"
#include "level.h"

namespace snaze {

class Player {
public:
    Player() = default;
    Player(const Level& level) : m_level(level) { /* empty */ }
    ~Player() = default;

    bool find_solution(const Position &, const Position &);
    std::vector<Position> path_to_food() const { return m_paths; }
    std::vector<dir_e> directions() const { return m_directions; }

private:
    Level m_level;
    std::vector<Position> m_paths;
    std::vector<dir_e> m_directions;
};

} // NAMESPACE SNAZE

#endif
