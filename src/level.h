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
    template<typename Key, typename Value>
    using unmap = std::unordered_map<Key, Value>;
    using maze_t = std::vector<std::vector<Cell>>;
    using coord_t = size_t;

    Level() = default;
    Level(const std::vector<std::vector<char>> &);
    ~Level() = default;

    coord_t rows() const { return m_rows; }
    coord_t cols() const { return m_cols; }
    maze_t maze() const { return m_maze; }

    void fill(const Position &, Cell::cell_e);
    void insert_snake(const Position &);
    void update(const Position &, dir_e, bool);
    void reset();
    
    Position spawn() const { return m_snake_spawn; }
    void spawn(const Position &);
    Position food() const { return m_food_pos; }
    void add_food();
    bool is_blocked(const Position &, dir_e) const;
    bool is_blocked(const Position &) const;
    Position move_to(const Position &, dir_e) const;

    std::string to_string() const;

private:
    Position choose_position() const;

    coord_t m_rows;
    coord_t m_cols;
    maze_t m_maze;
    Snake m_snake;
    Position m_snake_spawn;
    Position m_food_pos;
};

} // NAMESPACE SNAZE

#endif
