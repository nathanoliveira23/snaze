/**
 * @file cell.h
 *
 * @description
 * This class represents each cell in the maze.
 */

#ifndef CELL_H
#define CELL_H

#include "snake.h"

namespace snaze {

class Cell {
public:
    //== Enums

    /// Identification of each cell in the maze.
    enum class cell_e {
        WALL = 0,           //!< The maze wall.
        INV_WALL,           //!< A invisible wall.
        FREE,               //!< A free passage through the maze.
        FOOD,               //!< The food of a snake.
        SPAWN,              //!< The start position of a snake.
        SNAKE_HEAD,         //!< The snake head. 
        SNAKE_BODY,         //!< The snake body.
        DEATH_SNAKE_HEAD,   //!< The death snake head.
        DEATH_SNAKE_BODY,   //!< The death snake body.
    };

    //== Public interface.

    /// Default constructor.
    Cell() = default;
    /// Default constructor.
    Cell(const cell_e type) : m_cell_type(type) { /* empty */ }
    /// Destructor.
    ~Cell() = default;

    /// Returns the current state of the cell.
    cell_e type() const { return m_cell_type; }
    /// Sets a state for the cell.
    void type(const cell_e type) { m_cell_type = type; }

private:
    cell_e m_cell_type; //!< The state of cell.
};

} // NAMESPACE SNAZE

#endif
