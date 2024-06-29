#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>

#include "common.h"
#include "level.h"
#include "player.h"

using std::string;
using std::vector;
using std::pair;
using std::queue;
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

    enum class match_e : short {
        STARTING = 0,
        FOUND_FOOD,
        WALK_TO_DEATH,
        LOOKING_FOR_FOOD,
        RESET,
        GAME_OVER,
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

    state_e m_game_state;
    match_e m_match_state;

    Level m_level;
    Player m_player;
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
