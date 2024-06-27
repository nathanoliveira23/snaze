#include <cstddef>
#include <random>
#include <sstream>
#include "level.h"
#include "cell.h"
#include "common.h"

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

bool Level::is_blocked(const Position &pos, dir_e dir) const
{
    switch (dir) {
        case UP:
            return pos.row > 0 and
                (m_maze[pos.row - 1][pos.col].type() == Cell::cell_e::WALL);
        case DOWN:
            return pos.row < m_rows and
                (m_maze[pos.row + 1][pos.col].type() == Cell::cell_e::WALL);
        case LEFT:
            return pos.col > 0 and
                (m_maze[pos.row][pos.col - 1].type() == Cell::cell_e::WALL);
        case RIGHT:
            return pos.col < m_cols and
                (m_maze[pos.row][pos.col + 1].type() == Cell::cell_e::WALL);
    }

    return false;
}

bool Level::is_blocked(const Position &pos) const
{
    if (pos.row > 0 and pos.row < rows() and pos.col > 0 and pos.col < cols()) {
        return m_maze[pos.row][pos.col].type() != Cell::cell_e::FREE;
    }

    return true;
}

Position Level::move_to(const Position &pos, dir_e dir) const
{
    size_t r = pos.row, 
           c = pos.col;

    switch (dir) {
        case UP:    return Position(r - 1, c);
        case DOWN:  return Position(r + 1, c);
        case LEFT:  return Position(r, c - 1);
        case RIGHT: return Position(r, c + 1);
    }
}

void Level::add_food()
{
    Position spawn_food = choose_position();

    if (is_blocked(spawn_food)) {
        add_food();
    }
    else {
        m_maze[spawn_food.row][spawn_food.col] = Cell::cell_e::FOOD;
        m_food_pos = spawn_food;
    }

}

Position Level::choose_position() const
{
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<size_t> random_row(0, rows());
    std::uniform_int_distribution<size_t> random_col(0, cols());

    return Position(random_row(rng), random_col(rng));
}

void Level::set_cell(Position &pos, Cell::cell_e cell_type) {

    //Cell &cell = m_maze[pos.row][pos.col];
    //cell.type(cell_type);
    m_maze[pos.row][pos.col].type(cell_type);
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
