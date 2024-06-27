#ifndef CELL_H
#define CELL_H

namespace snaze {

class Cell {
public:
    enum class cell_e {
        WALL = 0,
        INV_WALL,
        FREE,
        FOOD,
        SPAWN,
        SNAKE_HEAD,
        SNAKE_BODY,
    };

    Cell() = default;
    Cell(const cell_e type) : m_cell_type(type) { /* empty */ }

    cell_e type() const { return m_cell_type; }
    void type(const cell_e type) { m_cell_type = type; }

private:
    cell_e m_cell_type;
};

} // NAMESPACE SNAZE

#endif
