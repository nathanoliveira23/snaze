#include <algorithm>
#include <cstring>
#include <queue>
#include <utility>
#include <iostream>
#include "player.h"
#include "common.h"

namespace snaze {

bool Player::find_solution(const Position &start, const Position &end) 
{
    std::queue<std::pair<Position, std::vector<Position>>> queue;

    bool visited[m_level.rows()][m_level.cols()];
    memset(visited, false, sizeof(visited));

    visited[start.row][start.col] = true;
    queue.push({ start, { start } });

    while (not queue.empty()) {
        auto [curr_pos, path] = queue.front();
        queue.pop();

        if (curr_pos == end) {
            m_paths = path;
            return true;
        }

        for (const Level::dir_e dir : { Level::UP, Level::LEFT, Level::DOWN, Level::RIGHT }) {
            if (not m_level.is_blocked(curr_pos, dir)) {
                Position next = m_level.move_to(curr_pos, dir);

                if (visited[next.row][next.col] == false) {
                    visited[next.row][next.col] = true;
                    std::vector<Position> new_path = path;
                    new_path.push_back(next);

                    queue.push({ next, new_path });
                }
            }
        }
    }

    return false;
}

} // NAMESPACE SNAZE
