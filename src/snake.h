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
    std::deque<Position> body() const { return m_snake; }

private:
    std::deque<Position> m_snake;
};

}

#endif
