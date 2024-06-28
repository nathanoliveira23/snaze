#include <cstring>
#include <queue>
#include <utility>
#include "player.h"
#include "common.h"

namespace snaze {

bool Player::find_solution(const Position &start, const Position &end) 
{
    std::queue<std::pair<Position, std::vector<Position>>> queue;
    std::queue<std::vector<dir_e>> q_dir;

    bool visited[m_level.rows()][m_level.cols()];
    memset(visited, false, sizeof(visited));

    visited[start.row][start.col] = true;
    queue.push({ start, { start } });
    q_dir.push({});

    while (not queue.empty()) {
        auto [curr_pos, path] = queue.front();
        auto directions = q_dir.front();
        queue.pop();
        if (not q_dir.empty())
            q_dir.pop();

        if (curr_pos == end) {
            m_paths = path;
            m_directions = directions;
            return true;
        }

        for (const dir_e dir : { UP, LEFT, DOWN, RIGHT }) {
            if (not m_level.is_blocked(curr_pos, dir)) {
                Position next = m_level.move_to(curr_pos, dir);

                if (visited[next.row][next.col] == false) {
                    visited[next.row][next.col] = true;
                    std::vector<Position> new_path = path;
                    new_path.push_back(next);

                    std::vector<dir_e> new_directions = directions;
                    new_directions.push_back(dir);

                    queue.push({ next, new_path });
                    q_dir.push(new_directions);
                }
            }
        }
    }

    return false;
}

} // NAMESPACE SNAZE
