#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <iostream>
#include <string>
#include <vector>

#include "common.h"
#include "level.h"

using std::string;
using std::vector;
using std::cout, std::endl;

namespace snaze {

class SnakeGame {
public:
    using count_t = unsigned;

    enum class state_e : short {
        STARTING = 0,
        WELLCOME,
        RUNNING,
        ENDING,
    };

    SnakeGame() = default;
    SnakeGame(RunningOpt &);
    ~SnakeGame() = default;

    void initialize(const std::vector<std::vector<char>>&);
    void process_events();
    void update();
    void render();
    bool game_over();

private:
    void display_welcome() const;
    void display_game_info() const;
    void display_match_info();
    void display_system_messages() const;
    void draw_horizontal_line() const;

    void read_enter() const;

    Level m_level;
    string m_system_msg;

    count_t m_score;
    count_t m_n_levels;

    count_t m_fps;
    count_t m_total_foods;
    count_t m_curr_foods;
    count_t m_lives;
    player_e m_player_type;
};

} // NAMESPACE SNAZE

#endif
