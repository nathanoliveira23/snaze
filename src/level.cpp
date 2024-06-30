#include <cstddef>
#include <random>
#include <sstream>
#include "level.h"
#include <iostream>
#include "cell.h"
#include "common.h"
#include "snake.h"

using std::cout, std::endl;

namespace snaze {

Level::unmap<Cell::cell_e, std::string> 
Level::render = {
    { Cell::cell_e::WALL, "█" },
    { Cell::cell_e::INV_WALL, " " },
    { Cell::cell_e::FREE, " " },
    { Cell::cell_e::FOOD, "" },
    { Cell::cell_e::SPAWN, "󱔎" },
    { Cell::cell_e::SNAKE_HEAD, "" },
    { Cell::cell_e::SNAKE_BODY, "" },
    { Cell::cell_e::DEATH_SNAKE_HEAD, "󰯈" },
    { Cell::cell_e::DEATH_SNAKE_BODY, "." },
};

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

            if (map[cell] == Cell::cell_e::SPAWN) {
                m_snake_spawn = Position(r, c);
                m_snake = Snake(Position(r, c));

                buff[c] = Cell::cell_e::SPAWN;
            }
        }

        m_maze.push_back(buff);
    }
}

void Level::insert_snake(const Position &pos)
{
    for (const auto &segments : m_snake.body())
        fill(segments, Cell::cell_e::SNAKE_BODY);

    fill(pos, Cell::cell_e::SNAKE_HEAD);
}

void Level::update(const Position &pos, dir_e direction, bool ate_food)
{
    if (ate_food) {
        Position tail;
        if (m_snake.size() == 1) {
            if (direction == UP) {
                tail = move_to(pos, DOWN);
            }
            else if (direction == DOWN) {
                tail = move_to(pos, UP);
            }
            else if (direction == RIGHT) {
                tail = move_to(pos, LEFT);
            }
            else if (direction == LEFT) {
                tail = move_to(pos, RIGHT);
            }
            m_snake.grow(tail);
        }
        else {
            m_snake.grow(pos);
        }
    }
    else {
        Position next = move_to(pos, direction);

        Position old_tail = m_snake.move(next);
        fill(old_tail, Cell::cell_e::FREE);

        if (m_snake.size() > 1) {
            for (const Position &p : m_snake.body()) {
                fill(p, Cell::cell_e::SNAKE_BODY);
            }

            Position head = m_snake.body().back();
            fill(head, Cell::cell_e::SNAKE_HEAD);
        }
        else {
            fill(next, Cell::cell_e::SNAKE_HEAD);
        }
    }
}

void Level::reset()
{
    m_snake = Snake(Position(m_snake_spawn));
    fill(m_food_pos, Cell::cell_e::FREE);

    for (coord_t r = 0; r < rows(); ++r) {
        for (coord_t c = 0; c < cols(); ++c) {
            Cell &cell = m_maze[r][c];

            if (cell.type() == Cell::cell_e::SNAKE_HEAD or cell.type() == Cell::cell_e::SNAKE_BODY) {
                Position pos = Position(r, c);
                fill(pos, Cell::cell_e::FREE);
            }
        }
    }
}

bool Level::is_blocked(const Position &pos, dir_e dir) const
{
    size_t r = pos.row, c = pos.col;
    Cell cell;

    switch (dir) {
        case UP:
            r = r - 1;
            cell = m_maze[r][c];
            return pos.row > 0 and cell.type() != Cell::cell_e::FREE and cell.type() != Cell::cell_e::FOOD;
        case DOWN:
            r = r + 1;
            cell = m_maze[r][c];
            return pos.row < m_rows and cell.type() != Cell::cell_e::FREE and cell.type() != Cell::cell_e::FOOD;
        case LEFT:
            c = c - 1;
            cell = m_maze[r][c];
            return pos.col > 0 and cell.type() != Cell::cell_e::FREE and cell.type() != Cell::cell_e::FOOD;
        case RIGHT:
            c = c + 1;
            cell = m_maze[r][c];
            return pos.col < m_cols and cell.type() != Cell::cell_e::FREE and cell.type() != Cell::cell_e::FOOD;
    }

    return false;
}

bool Level::is_blocked(const Position &pos) const
{
    const Cell &cell = m_maze[pos.row][pos.col];

    if (pos.row > 0 and pos.row < rows() and pos.col > 0 and pos.col < cols()) {
        return cell.type() != Cell::cell_e::FREE;
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
        fill(spawn_food, Cell::cell_e::FOOD);
        m_food_pos = spawn_food;
    }
}

void Level::spawn(const Position &new_spawn) 
{
    m_snake_spawn = new_spawn;
}

Position Level::choose_position() const
{
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<size_t> random_row(0, rows());
    std::uniform_int_distribution<size_t> random_col(0, cols());

    return Position(random_row(rng), random_col(rng));
}

void Level::fill(const Position &pos, Cell::cell_e cell_type) {

    Cell &cell = m_maze[pos.row][pos.col];
    cell.type(cell_type);
}

std::string Level::to_string() const
{
    std::ostringstream oss;
    
    for (coord_t r = 0; r < rows(); ++r) {
        for (coord_t c = 0; c < cols(); ++c) {
            Cell cell = m_maze[r][c];
            oss << Level::render[cell.type()];
        }
        oss << "\n";
    }

    return oss.str();
}

} //NAMESPACE SNAZE
