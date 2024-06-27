#include "snake_game.h"
#include "common.h"
#include "level.h"

namespace snaze {

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
    m_system_msg = "Press <ENTER> to start the game!";
}

void SnakeGame::render()
{
    display_welcome();
    display_game_info();
    display_system_messages();
    display_match_info();
    cout << m_level.to_string();
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

void SnakeGame::draw_horizontal_line() const 
{
    cout << string(80, '-') << endl;
}

} // NAMESPACE SNAZE
