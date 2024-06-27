#include <sstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>

#include "level.h"

namespace snaze {

Level::Level(const std::vector<std::vector<char>> &input)
{
    unmap<char, Cell::cell_e> map = {
        { '#', Cell::cell_e::WALL },
        { '.', Cell::cell_e::INV_WALL },
        { ' ', Cell::cell_e::FREE },
        { '&', Cell::cell_e::SPAWN },
    };

    m_rows = input.size();
    m_cols = input[0].size();

    m_maze.clear();

    for (size_t r = 0; r < m_rows; ++r) {
        std::vector<Cell> buff;

        for (size_t c = 0; c < m_cols; ++c) {
            char cell = input[r][c];
            buff.push_back(Cell(map[cell]));

            if (map[cell] == Cell::cell_e::SPAWN)
                m_snake_spawn = Position(r, c);
        }

        m_maze.push_back(buff);
    }
}

std::string Level::to_string() const
{
    std::ostringstream oss;
    
    for (coord_t r = 0; r < rows(); ++r) {
        for (coord_t c = 0; c < cols(); ++c) {
            Cell cell = m_maze[r][c];

            if (cell.type() == Cell::cell_e::WALL) {
                oss << "█";
            } else if (cell.type() == Cell::cell_e::INV_WALL) {
                oss << ".";
            } else if (cell.type() == Cell::cell_e::FREE) {
                oss << " ";
            } else if (cell.type() == Cell::cell_e::FOOD) {
                oss << "F";
            } else if (cell.type() == Cell::cell_e::SPAWN) {
                oss << "&";
            } else if (cell.type() == Cell::cell_e::SNAKE_HEAD) {
                oss << "H";
            } else if (cell.type() == Cell::cell_e::SNAKE_BODY) {
                oss << "B";
            } 
        }
        oss << "\n";
    }

    return oss.str();
}

} //NAMESPACE SNAZE
