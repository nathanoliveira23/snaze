#include "snake.h"
#include "common.h"

namespace snaze {

/**
 * @brief Constructor for initializing the snake with a starting position.
 * 
 * Initializes the snake with a single position at the given starting position.
 * 
 * @param pos The initial position of the snake.
 */
Snake::Snake(const Position& pos) 
{
    m_snake.push_back(pos);
}

/**
 * @brief Makes the snake grow by adding a new position at the front.
 * 
 * Adds a new position to the front of the snake's body, representing growth.
 * This position is the new position of the snake's head. 
 *
 * @param pos The new position to add at the front of the snake.
 */
void Snake::grow(const Position &pos) 
{
    m_snake.push_front(pos);
}

/**
 * @brief Moves the snake to a new position and returns its last tail position.
 * 
 * Moves the snake to a new position specified by `new_pos`, pushing this position
 * to the back of the snake's body and removing the front position, which represents
 * the snake's movement.
 * 
 * @param new_pos The new position to move the snake to.
 * @return The last tail position of the snake before moving.
 */
Position Snake::move(const Position &new_pos)
{
    // Get the current position of the last tail (front of the deque).
    Position last_tail = m_snake.front();

    // Add the new snake's head position to the back of the snake's body.
    m_snake.push_back(new_pos);

    // Remove the front position of the snake's body (last tail position).
    m_snake.pop_front();

    // Return the last tail position before moving.
    return last_tail;
}

}
