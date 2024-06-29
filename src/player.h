#ifndef PLAYER_H
#define PLAYER_H

#include <deque>
#include "common.h"
#include "level.h"

namespace snaze {

class Player {
public:
    using direction = std::pair<Position, dir_e>;

    Player() = default;
    Player(const Level& level) : m_level(level) { /* empty */ }
    ~Player() = default;

    bool find_solution(const Position &, const Position &);
    direction next_move();
    Position last_move() const { return m_paths.back(); }
    size_t amount_of_steps() const { return m_paths.size(); }

private:
    Level m_level;
    std::deque<Position> m_paths;
    std::deque<dir_e> m_directions;
};

} // NAMESPACE SNAZE

#endif
