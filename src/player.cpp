#include <cstring>
#include <queue>
#include <utility>

#include "player.h"
#include "common.h"


namespace snaze {

/**
 * @brief Finds a solution path from the start to the end position in the maze.
 * 
 * This function uses a breadth-first search algorithm to find a path from the
 * start position to the end position in the maze. It stores the found path and
 * directions taken in member variables.
 * 
 * @param start The starting position in the maze.
 * @param end The target position to reach in the maze.
 * @return true if a path is found from start to end, false otherwise.
 */
bool Player::find_solution(const Position &start, const Position &end) 
{
    std::queue<Position> queue;
    std::queue<std::deque<Position>> q_pos;
    std::queue<std::deque<dir_e>> q_dir;

    std::deque<Position> path_to_death;
    std::deque<dir_e> dirs_to_death;

    // Initialize visited array to track visited positions.
    bool visited[m_level.rows()][m_level.cols()];
    memset(visited, false, sizeof(visited));

    visited[start.row][start.col] = true;

    queue.push(start);
    q_pos.push({ start });
    q_dir.push({});

    while (!queue.empty()) {
        Position curr_pos = queue.front();
        auto directions = q_dir.front();
        auto positions = q_pos.front();

        queue.pop();
        q_dir.pop();
        q_pos.pop();

        if (curr_pos == end) {
            // Found a path to the end position.
            m_paths = positions;
            m_directions = directions;
            m_directions.push_back(m_directions.back()); // Ensure directions include the last move.

            return true;
        }

        // Explore all four possible directions (UP, LEFT, DOWN, RIGHT).
        for (const dir_e dir : { UP, LEFT, DOWN, RIGHT }) {
            if (!m_level.is_blocked(curr_pos, dir)) {
                Position next = m_level.move_to(curr_pos, dir);

                if (!visited[next.row][next.col]) {
                    visited[next.row][next.col] = true;

                    // Create new path and directions including the current move.
                    std::deque<Position> new_path = positions;
                    new_path.push_back(next);

                    std::deque<dir_e> new_directions = directions;
                    new_directions.push_back(dir);

                    // Enqueue the next position, updated path, and directions.
                    queue.push(next);
                    q_pos.push(new_path);
                    q_dir.push(new_directions);

                    // Store the path to death in case no valid path is found.
                    path_to_death = new_path;
                    dirs_to_death = new_directions;
                }
            }
        }
    }

    // If no path to the end is found, use the path to death as fallback.
    m_paths = path_to_death;
    m_directions = dirs_to_death;
    m_directions.push_back(m_directions.back()); // Ensure directions include the last move.

    return false;
}

/**
 * @brief Retrieves the next move to be made by the player.
 * 
 * This function retrieves the next position and direction from the front
 * of the dequeues storing the player's movement path and directions.
 * 
 * @return A pair containing the next position and direction to move.
 */
std::pair<Position, dir_e> Player::next_move()
{
    // Get the next position and direction from the front of the deques.
    Position pos = m_paths.front();
    dir_e dir = m_directions.front();

    // Remove the front elements from the deques.
    m_paths.pop_front();
    m_directions.pop_front();

    // Return a pair with the next position and direction.
    return std::make_pair(pos, dir);
}

} // NAMESPACE SNAZE
