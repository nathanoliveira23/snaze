/**
 * @file snake.h
 *
 * @description
 * This class represents the snake in a maze in the simulation.
 */

#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <queue>
#include <vector>
#include "common.h"

namespace snaze {

class Snake {
public:
    /// Default constructor.
    Snake() = default;
    /// Default constructor.
    Snake(const Position &pos);
    /// Destructor.
    ~Snake() = default;

    /// Increases the length of the snake.
    void grow(const Position &pos);
    /// Move the snake position.
    Position move(const Position &pos);
    /// Returns the length of the snake.
    size_t size() const { return m_snake.size(); }
    /// Returns the current direction of the snake.
    dir_e direction() const { return m_snake_direction; }
    /// sets the direction of the snake.
    void direction(dir_e dir) { m_snake_direction = dir; }
    /// Returns the positions where the snake is inserted in the maze.
    std::deque<Position> body() const { return m_snake; }

private:
    std::deque<Position> m_snake; //!< Stores the positions where the snake parts are in the maze.
    dir_e m_snake_direction;      //!< The snake direction.
};

}

#endif
