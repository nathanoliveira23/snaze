#include <cstddef>
#include <random>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "level.h"
#include "cell.h"
#include "common.h"
#include "snake.h"

namespace snaze {

Level::unmap<Cell::cell_e, std::string> 
Level::render = {
    { Cell::cell_e::WALL, "█" },
    { Cell::cell_e::INV_WALL, " " },
    { Cell::cell_e::FREE, " " },
    { Cell::cell_e::FOOD, "" },
    { Cell::cell_e::SPAWN, "󱔎" },
    { Cell::cell_e::SNAKE_BODY, "•" },
    { Cell::cell_e::DEATH_SNAKE_HEAD, "󰯈" },
    { Cell::cell_e::DEATH_SNAKE_BODY, "." },
};

/**
 * @brief Constructs a Level object from a given 2D character vector.
 * 
 * This constructor initializes a Level object using a 2D vector of characters
 * to define the maze structure. The characters are mapped to specific cell types.
 * 
 * @param input A 2D vector of characters representing the maze.
 * 
 * @throws std::invalid_argument if an invalid character is found in the input.
 */
Level::Level(const std::vector<std::vector<char>> &input)
{
    // Mapping of characters to cell types.
    unmap<char, Cell::cell_e> map = {
        { '#', Cell::cell_e::WALL },      //!< Wall cell.
        { '.', Cell::cell_e::INV_WALL },  //!< Invisible wall cell.
        { ' ', Cell::cell_e::FREE },      //!< Free space cell.
        { '&', Cell::cell_e::SPAWN },     //!< Snake spawn cell.
    };

    // Set number of rows and columns based on input size.
    m_rows = input.size();
    m_cols = input[0].size();

    // Clear the existing maze.
    m_maze.clear();

    for (size_t r = 0; r < m_rows; ++r) {
        std::vector<Cell> buff;

        for (size_t c = 0; c < m_cols; ++c) {
            char cell = input[r][c];

            // Check if the character is valid and map it to the corresponding cell type.
            if (map.find(cell) != map.end()) {
                buff.push_back(Cell(map[cell]));

                // Handle the snake spawn point.
                if (map[cell] == Cell::cell_e::SPAWN) {
                    m_snake_spawn = Position(r, c);
                    m_snake = Snake(Position(r, c));
                    buff[c] = Cell::cell_e::SPAWN;
                }
            }
            else {
                // Throw an error if an invalid character is found.
                std::stringstream error_msg;
                error_msg << "Invalid character: \'" << cell << "\'.\n";
                throw std::invalid_argument(error_msg.str());
            }
        }

        // Add the row to the maze
        m_maze.push_back(buff);
    }
}

/**
 * @brief Places the snake in the maze at the given position.
 * 
 * This function updates the maze to place the snake's body segments and head
 * at their respective positions.
 * 
 * @param pos The position to place the snake's head.
 */
void Level::place_snake(const Position &pos)
{
    // Iterate through each segment of the snake's body and fill it in the maze.
    for (const auto &segments : m_snake.body())
        fill(segments, Cell::cell_e::SNAKE_BODY);

    // Fill the position of the snake's head in the maze.
    fill(pos, Cell::cell_e::SNAKE_HEAD);
}

/**
 * @brief Updates the snake's position and state in the maze.
 * 
 * This function updates the snake's direction, grows the snake if it ate food,
 * and moves the snake to the next position, updating the maze accordingly.
 * 
 * @param pos The current position of the snake's head.
 * @param direction The direction in which the snake is moving.
 * @param ate_food A boolean indicating whether the snake has eaten food.
 */
void Level::update(const Position &pos, dir_e direction, bool ate_food)
{
    // Update the snake's direction.
    m_snake.direction(direction);

    // If the snake has eaten food.
    if (ate_food) {
        /**
         * @details
         * If the snake only consists of the head, it is necessary to know 
         * its last direction when eating the food as the tail will be 
         * inserted in the opposite position.
         */
        Position tail;
        if (m_snake.size() == 1) {
            // Determine the tail position based on the direction.
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
            // Grow the snake at the tail position.
            m_snake.grow(tail);
        }
        else {
            // Grow the snake at the current position.
            m_snake.grow(pos);
        }
    }
    else {
        // Move the snake to the next position.
        Position next = move_to(pos, direction);

        // Get the old tail position and mark it as free.
        Position old_tail = m_snake.move(next);
        fill(old_tail, Cell::cell_e::FREE);

        // Update the maze with the new snake body and head positions.
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

/**
 * @brief Resets the level to its initial state.
 * 
 * This function resets the snake to its spawn position, clears the food position,
 * and removes any snake body or head cells from the maze.
 */
void Level::reset()
{
    // Reset the snake to its spawn position.
    m_snake = Snake(Position(m_snake_spawn));

    // Clear the food position.
    fill(m_food_pos, Cell::cell_e::FREE);

    // Iterate through each cell in the maze.
    for (coord_t r = 0; r < rows(); ++r) {
        for (coord_t c = 0; c < cols(); ++c) {
            Cell &cell = m_maze[r][c];

            // Check if the cell is part of the snake and clear it.
            if (cell.type() == Cell::cell_e::SNAKE_HEAD || cell.type() == Cell::cell_e::SNAKE_BODY) {
                Position pos = Position(r, c);
                fill(pos, Cell::cell_e::FREE);
            }
        }
    }
}

/**
 * @brief Checks if the next cell in the given direction is blocked.
 * 
 * This function checks if the cell in the specified direction from the given
 * position is blocked. A cell is considered blocked if it is not free or food.
 * 
 * @param pos The current position.
 * @param dir The direction to check.
 * @return true if the cell in the given direction is blocked, false otherwise.
 */
bool Level::is_blocked(const Position &pos, dir_e dir) const
{
    size_t r = pos.row, c = pos.col;
    Cell cell;

    switch (dir) {
        case UP:
            r = r - 1;
            cell = m_maze[r][c];
            return pos.row > 0 && cell.type() != Cell::cell_e::FREE && cell.type() != Cell::cell_e::FOOD;
        case DOWN:
            r = r + 1;
            cell = m_maze[r][c];
            return pos.row < m_rows && cell.type() != Cell::cell_e::FREE && cell.type() != Cell::cell_e::FOOD;
        case LEFT:
            c = c - 1;
            cell = m_maze[r][c];
            return pos.col > 0 && cell.type() != Cell::cell_e::FREE && cell.type() != Cell::cell_e::FOOD;
        case RIGHT:
            c = c + 1;
            cell = m_maze[r][c];
            return pos.col < m_cols && cell.type() != Cell::cell_e::FREE && cell.type() != Cell::cell_e::FOOD;
    }

    return false;
}

/**
 * @brief Checks if the cell at the given position is blocked.
 * 
 * This function checks if the cell at the specified position is blocked.
 * A cell is considered blocked if it is not free or if the position is outside the maze boundaries.
 * 
 * @param pos The position to check.
 * @return true if the cell is blocked, false otherwise.
 */
bool Level::is_blocked(const Position &pos) const
{
    // Check if the position is within the maze boundaries.
    if (pos.row > 0 && pos.row < rows() && pos.col > 0 && pos.col < cols()) {
        const Cell &cell = m_maze[pos.row][pos.col];
        // Return true if the cell is not free.
        return cell.type() != Cell::cell_e::FREE;
    }

    // Return true if the position is outside the maze boundaries.
    return true;
}

/**
 * @brief Returns the position resulting from moving in the specified direction.
 * 
 * This function computes the new position by moving from the given position
 * in the specified direction.
 * 
 * @param pos The current position.
 * @param dir The direction to move.
 * @return The new position after moving in the specified direction.
 */
Position Level::move_to(const Position &pos, dir_e dir) const
{
    size_t r = pos.row, c = pos.col;

    switch (dir) {
        case UP:    return Position(r - 1, c);
        case DOWN:  return Position(r + 1, c);
        case LEFT:  return Position(r, c - 1);
        case RIGHT: return Position(r, c + 1);
        default:    return pos;
    }
}

/**
 * @brief Adds food to a random position in the maze.
 * 
 * This function chooses a random position in the maze to place food. If the chosen
 * position is blocked, it recursively tries to find an unblocked position.
 */
void Level::add_food()
{
    // Choose a random position to spawn food.
    Position spawn_food = choose_position();

    // If the position is blocked, recursively try to add food again.
    if (is_blocked(spawn_food)) {
        add_food();
    }
    else {
        // Place food at the chosen position and update the food position.
        fill(spawn_food, Cell::cell_e::FOOD);
        m_food_pos = spawn_food;
    }
}

/**
 * @brief Sets a new spawn position for the snake.
 * 
 * This function updates the snake's spawn position to the specified position.
 * 
 * @param new_spawn The new spawn position for the snake.
 */
void Level::spawn(const Position &new_spawn) 
{
    m_snake_spawn = new_spawn;
}

/**
 * @brief Chooses a random position within the maze.
 * 
 * This function generates a random position within the bounds of the maze
 * using a uniform distribution for both rows and columns.
 * 
 * @return A randomly chosen position within the maze.
 */
Position Level::choose_position() const
{
    std::random_device rd;
    std::mt19937 rng(rd());

    // Define uniform distributions for rows and columns within valid ranges.
    std::uniform_int_distribution<size_t> random_row(0, rows() - 1);
    std::uniform_int_distribution<size_t> random_col(0, cols() - 1);

    // Generate random row and column indices.
    size_t r = random_row(rng);
    size_t c = random_col(rng);

    return Position(r, c);
}

/**
 * @brief Fills a cell at the given position with the specified cell type.
 * 
 * This function updates the cell at the specified position in the maze
 * with the provided cell type.
 * 
 * @param pos The position in the maze to fill.
 * @param cell_type The type of cell to fill the position with.
 */
void Level::fill(const Position &pos, Cell::cell_e cell_type)
{
    // Get a reference to the cell at the specified position.
    Cell &cell = m_maze[pos.row][pos.col];
    
    // Set the cell type to the specified value.
    cell.type(cell_type);
}

/**
 * @brief Converts the maze into a string representation.
 * 
 * This function converts the current state of the maze into a string representation,
 * where each cell is represented by a character based on its type. Snake head direction
 * is indicated by specific characters.
 * 
 * @return A string representation of the maze.
 */
std::string Level::to_string() const
{
    std::ostringstream oss;
    
    // Iterate through each cell in the maze.
    for (coord_t r = 0; r < rows(); ++r) {
        for (coord_t c = 0; c < cols(); ++c) {
            Cell cell = m_maze[r][c];

            // Determine the character representation based on the cell type.
            if (cell.type() == Cell::cell_e::SNAKE_HEAD) {
                // Use specific characters for snake head based on direction.
                switch (m_snake.direction()) {
                    case UP:    oss << "v"; break;    // Snake is moving up.
                    case DOWN:  oss << "^"; break;    // Snake is moving down.
                    case LEFT:  oss << ">"; break;    // Snake is moving left.
                    case RIGHT: oss << "<"; break;    // Snake is moving right.
                    default:    oss << " ";           // Default case.
                }
            }
            else {
                // Use predefined characters for other cell types.
                oss << Level::render[cell.type()];
            }
        }
        oss << "\n"; // Add newline after each row.
    }

    return oss.str(); // Return the generated string representation.
}

} //NAMESPACE SNAZE
