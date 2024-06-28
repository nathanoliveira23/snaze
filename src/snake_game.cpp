#include <cstdio>
#include <unordered_map>
#include "snake_game.h"
#include "common.h"
#include "player.h"

namespace snaze {

std::unordered_map<dir_e, string> direct {
    { UP, "CIMA" },
    { DOWN, "BAIXO" },
    { LEFT, "ESQUERDA" },
    { RIGHT, "DIREITA" },
};


SnakeGame::SnakeGame(RunningOpt& opt)
{
    m_total_foods = opt.foods;
    m_fps = opt.fps;
    m_lives = opt.lives;
    m_player_type = opt.player_type;

    m_curr_foods = 0;
    m_score = 0;
}

void SnakeGame::initialize(const vector<vector<char>> &maze)
{
    m_level = Level(maze);
    m_player = Player(m_level);
    m_game_state = state_e::STARTING;

    m_curr_foods = 0;

    m_system_msg = "Press <ENTER> to start the game!";
}

void SnakeGame::process_events()
{
    if (m_game_state == state_e::WELLCOME) {
        read_enter();
    }
}

void SnakeGame::update() 
{
    if (m_game_state == state_e::STARTING) {
        m_game_state = state_e::WELLCOME;
    }
    else if (m_game_state == state_e::WELLCOME) {
        m_game_state = state_e::RUNNING;
        m_match_state = match_e::STARTING;
    }
    else if (m_game_state == state_e::RUNNING) {
        if (m_match_state == match_e::STARTING) {
            m_level.add_food();
            m_level.insert_snake(m_level.spawn());

            m_player = Player(m_level);

            bool has_solution = m_player.find_solution(m_level.spawn(), m_level.food());

            if (has_solution) {
                auto path = m_player.path_to_food();
                auto dirs = m_player.directions();

                for (const auto &p : path) m_paths.push(p);

                for (const auto &d : dirs) m_dirs.push(d);

                m_match_state = match_e::LOOKING_FOR_FOOD;
            }
        }
        else if (m_match_state == match_e::LOOKING_FOR_FOOD) {
            if (not m_paths.empty()) {
                auto pos = m_paths.front();
                m_paths.pop();

                bool ate_food = false;

                if (pos == m_level.food()) {
                    m_level.spawn(pos);

                    while (not m_paths.empty())
                        m_paths.pop();

                    while (not m_dirs.empty())
                        m_dirs.pop();

                    ate_food = true;

                    m_level.update(pos, m_dirs.back(), ate_food);

                    m_curr_foods++;
                    m_match_state = match_e::STARTING;

                    return;
                }

                if (not m_dirs.empty()) {
                    auto dir = m_dirs.front();
                    m_dirs.pop();
                    
                    m_level.update(pos, dir, ate_food);
                }
            }
        }
    }
}

void SnakeGame::render()
{
    if (m_game_state == state_e::WELLCOME) {
        display_welcome();
        display_game_info();
        display_system_messages();
        display_match_info();
        cout << m_level.to_string();
    }
    else if (m_game_state == state_e::RUNNING) {
        display_match_info();
        if (m_match_state == match_e::LOOKING_FOR_FOOD) {
            cout << m_level.to_string();
        }
    }
}

void SnakeGame::display_welcome() const 
{
    cout << " --> Welcome to the classic Snake Game <-- \n"; 
    cout << "      copyright DIMAp/UFRN 2013-2024\n";

    draw_horizontal_line();
}

void SnakeGame::display_game_info() const
{
    cout << " Levels loaded: " << m_n_levels << " | "
         << "Snake lives: " << m_lives << " | "
         << "Apples to eat: " << m_total_foods << endl;

    cout << " Clear all levels to win the game. Good luck!!!\n";

    draw_horizontal_line();
}

void SnakeGame::display_system_messages() const
{
    cout << ">>> " << m_system_msg << endl << endl;
}

void SnakeGame::display_match_info()
{
    cout << "Lives: " << string(m_lives, 'O') << " | "
         << "Score: " << m_score << " | "
         << "Food eaten: " << m_curr_foods << " of " << m_total_foods 
         << endl;

    draw_horizontal_line();
}

void SnakeGame::read_enter() const
{
    char enter;

    while (enter != '\n')
        enter = getchar();
}

void SnakeGame::draw_horizontal_line() const 
{
    cout << string(80, '-') << endl;
}

} // NAMESPACE SNAZE
