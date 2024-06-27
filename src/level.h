#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include "cell.h"
#include "common.h"

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
    
    Position spawn() const { return m_snake_spawn; }

    std::string to_string() const;

private:
    coord_t m_rows;
    coord_t m_cols;
    maze_t m_maze;
    Position m_snake_spawn;
};

} // NAMESPACE Snake

#endif
