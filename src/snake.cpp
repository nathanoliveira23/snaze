#include "snake.h"
#include "common.h"

namespace snaze {

Snake::Snake(const Position& pos) {
    m_snake.push_back(pos);
}

void Snake::grow(const Position &pos) {
    m_snake.push_front(pos);
}

void Snake::move(const Position &new_pos)
{
    m_snake.push_back(new_pos);
    m_snake.pop_front();
}

}
