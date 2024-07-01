/**
 * @file player.h
 *
 * @description
 * This class represents the AI engine.
 * It's responsible for defining the snake's minimum 
 * path to the food in the maze.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <deque>
#include "common.h"
#include "level.h"

namespace snaze {

class Player {
public:
    //== Aliases
    using direction = std::pair<Position, dir_e>; //!< Alias for the position e direction pair.

    /// Default constructor.
    Player() = default;
    /// Default constructor.
    Player(const Level& level) : m_level(level) { /* empty */ }
    /// Destructor.
    ~Player() = default;

    /// Returns the shortest path from the snake's origin to the food.
    bool find_solution(const Position &, const Position &);
    /// Return the next step to the food.
    direction next_move();
    /// Returns the target position of the snake.
    Position last_move() const { return m_paths.back(); }
    /// Returns the number of steps to the destination.
    size_t amount_of_steps() const { return m_paths.size(); }

private:
    Level m_level;                  //!< The maze grid.
    std::deque<Position> m_paths;   //!< Stores the found positions.
    std::deque<dir_e> m_directions; //!< Stores the found directions.
};

} // NAMESPACE SNAZE

#endif
