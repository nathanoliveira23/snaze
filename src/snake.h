#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <queue>
#include <vector>
#include "common.h"

namespace snaze {

class Snake {
public:
    Snake() = default;
    Snake(const Position &pos);

    void grow(const Position &pos);
    Position move(const Position &pos);
    size_t size() const { return m_snake.size(); }
    dir_e direction() const { return m_snake_direction; }
    void direction(dir_e dir) { m_snake_direction = dir; }
    std::deque<Position> body() const { return m_snake; }

private:
    std::deque<Position> m_snake;
    dir_e m_snake_direction;
};

}

#endif
