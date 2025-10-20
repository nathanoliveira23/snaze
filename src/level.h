/**
 * @file level.h
 *
 * @description
 * This class represents the maze.
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "cell.h"
#include "common.h"
#include "snake.h"

namespace snaze {

class Level {
public:
    //== Aliases
    template<typename Key, typename Value>
    using unmap = std::unordered_map<Key, Value>;
    using maze_t = std::vector<std::vector<Cell>>;
    using coord_t = size_t;

    /// Default constructor.
    Level() = default;
    /// Default constructor.
    Level(const std::vector<std::vector<char>> &);
    /// Destructor.
    ~Level() = default;

    /// Hash table that maps the cell type to its ASCII representation.
    static unmap<Cell::cell_e, std::string> render;

    /// Returns the number of rows in the matrix.
    coord_t rows() const { return m_rows; }
    /// Returns the number of cols in the matrix.
    coord_t cols() const { return m_cols; }
    /// Returns the current state of the maze.
    maze_t maze() const { return m_maze; }

    /// Given a coordinate in the matrix, fill the cell with the cell type.
    void fill(const Position &, Cell::cell_e);
    /// Position the snake in the maze.
    void place_snake(const Position &);
    /// Updates the state of the maze.
    void update(const Position &, dir_e, bool);
    /// Reset the maze.
    void reset();
    
    /// Returns the initial position of the snake.
    Position spawn() const { return m_snake_spawn; }
    /// Sets the initial position of the snake.
    void spawn(const Position &);
    /// Returns the position of the food in the maze.
    Position food() const { return m_food_pos; }
    /// Sets the position of the food in the maze.
    void add_food();
    /// Checks whether a position is blocked in the maze.
    bool is_blocked(const Position &, dir_e) const;
    bool is_blocked(const Position &) const;
    /// Returns the position of the neighbor cell based on the provided direction.
    Position move_to(const Position &, dir_e) const;

    /// Returns the ASCII representation of the maze.
    std::string to_string() const;

private:
    /// Returns a random position in the maze.
    Position choose_position() const;

    coord_t m_rows;             //!< The number of rows in the matrix.
    coord_t m_cols;             //!< The number of cols in the matrix.
    maze_t m_maze;              //!< The matrix.
    Snake m_snake;              //!< The snake to be inserted into the maze.
    Position m_snake_spawn;     //!< The initial position of the snake.
    Position m_food_pos;        //!< The position of the food in the maze.
};

} // NAMESPACE SNAZE

#endif
