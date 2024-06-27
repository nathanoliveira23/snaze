#include "snake_game.h"
#include "cell.h"
#include "common.h"
#include <cstdio>

namespace snaze {

static size_t current_pos = 0;

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
    m_level.add_food();
    m_game_state = state_e::STARTING;

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
    }
    else if (m_game_state == state_e::RUNNING) {
        if (m_player.find_solution(m_level.spawn(), m_level.food())) {
            vector<Position> path = m_player.path_to_food();
            Position pos = path[current_pos];

            if (current_pos < path.size()) {
                m_level.set_cell(pos, Cell::cell_e::SPAWN);
                current_pos++;
            }
            else {
                cout << "cabou\n";
                current_pos = 0;
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
        cout << m_level.to_string();
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
