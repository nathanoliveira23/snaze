#include <cstring>
#include <iostream>
#include <queue>
#include <utility>
#include <unordered_map>
#include <string>
#include "player.h"
#include "common.h"


namespace snaze {

std::unordered_map<snaze::dir_e, std::string> direct2 {
    { UP, "CIMA" },
    { DOWN, "BAIXO" },
    { LEFT, "ESQUERDA" },
    { RIGHT, "DIREITA" },
};

bool Player::find_solution(const Position &start, const Position &end) 
{
    std::queue<Position> queue;
    std::queue<std::deque<Position>> q_pos;
    std::queue<std::deque<dir_e>> q_dir;

    std::deque<Position> path_to_death;
    std::deque<dir_e> dirs_to_death;

    bool visited[m_level.rows()][m_level.cols()];
    memset(visited, false, sizeof(visited));

    visited[start.row][start.col] = true;

    queue.push(start);
    q_pos.push({ start });
    q_dir.push({});

    while (not queue.empty()) {
        Position curr_pos = queue.front();
        auto directions = q_dir.front();
        auto positions = q_pos.front();

        queue.pop();

        if (not q_dir.empty())
            q_dir.pop();

        if (not q_pos.empty())
            q_pos.pop();

        if (curr_pos == end) {
            m_paths = positions;

            m_directions = directions;
            m_directions.push_back(m_directions.back());

            return true;
        }

        for (const dir_e dir : { UP, LEFT, DOWN, RIGHT }) {
            if (not m_level.is_blocked(curr_pos, dir)) {
                Position next = m_level.move_to(curr_pos, dir);

                if (visited[next.row][next.col] == false) {
                    visited[next.row][next.col] = true;
                    std::deque<Position> new_path = positions;
                    new_path.push_back(next);

                    std::deque<dir_e> new_directions = directions;
                    new_directions.push_back(dir);

                    queue.push(next);
                    q_pos.push(new_path);
                    q_dir.push(new_directions);

                    path_to_death = new_path;
                    dirs_to_death = new_directions;
                }
            }
        }
    }

    m_paths = path_to_death;
    m_directions = dirs_to_death;
    m_directions.push_back(m_directions.back());

    return false;
}

std::pair<Position, dir_e> Player::next_move()
{
    Position pos = m_paths.front();
    dir_e dir = m_directions.front();

    m_paths.pop_front();
    m_directions.pop_front();

    return std::make_pair(pos, dir);
}

} // NAMESPACE SNAZE
