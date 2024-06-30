#include <cstdio>
#include <unordered_map>
#include "snake_game.h"
#include "cell.h"
#include "common.h"
#include "level.h"
#include "player.h"
#include "snake.h"

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
    m_curr_lives = m_lives;
    m_score = 0;
}

void SnakeGame::initialize(const vector<vector<char>> &maze)
{
    m_level = Level(maze);
    m_player = Player(m_level);
    m_game_state = state_e::STARTING;

    m_curr_foods = 0;
    m_end_game = false; 

    m_system_msg = "Press <ENTER> to start the game!";
}

void SnakeGame::process_events()
{
    if (m_game_state == state_e::WELLCOME or m_match_state == match_e::RESET) {
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

            m_match_state = has_solution ? match_e::LOOKING_FOR_FOOD
                                         : match_e::WALK_TO_DEATH;
        }
        else if (m_match_state == match_e::LOOKING_FOR_FOOD) {
            auto [step, direction] = m_player.next_move();
            bool found_food = step == m_level.food();

            m_level.update(step, direction, found_food);

            if (found_food) {
                bool death = true;

                for (const dir_e dir : { UP, LEFT, DOWN, RIGHT })
                    if (not m_level.is_blocked(step, dir))
                        death = false;

                m_level.spawn(step);
                m_curr_foods++;
                m_score += 20;

                if (death) {
                    m_curr_lives -= 1;
                    if (m_curr_lives == 0) {
                        m_end_game = true;
                        m_match_state = match_e::GAME_OVER;
                    }
                    else {
                        m_match_state = match_e::RESET;
                    }
                }
                else {
                    m_match_state = match_e::STARTING;
                }

                if (m_curr_foods == m_total_foods) {
                    m_match_state = match_e::WIN;
                    m_end_game = true;
                }
            }
        }
        else if (m_match_state == match_e::WALK_TO_DEATH) {
            auto [step, direction] = m_player.next_move();

            if (not m_level.is_blocked(step, direction))
                m_level.update(step, direction, false);

            m_level.spawn(m_player.last_move());

            if (m_player.amount_of_steps() == 0) {
                m_score - 20 < 0 ? m_score = 0 
                                 : m_score -= 20;
                m_curr_lives -= 1;

                if (m_curr_lives == 0) {
                    m_end_game = true;
                    m_match_state = match_e::GAME_OVER;
                    return;
                }
                else {
                    m_system_msg = "Press <ENTER> to try again.";
                }

                m_match_state = match_e::RESET;
            }
            
        }
        else if (m_match_state == match_e::RESET) {
            m_level.reset();
            m_match_state = match_e::STARTING;
        }
    }
    else if (m_match_state == match_e::WIN) {
        m_game_state = state_e::ENDING;
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
        else if (m_match_state == match_e::STARTING) {
            cout << m_level.to_string();
        }
        else if (m_match_state == match_e::WALK_TO_DEATH) {
            cout << m_level.to_string();
        }
        else if (m_match_state == match_e::RESET) {
            display_death_snake();
            display_system_messages();
        }
        else if (m_match_state == match_e::WIN) {
            display_won_message();
        }
        else if (m_match_state == match_e::GAME_OVER) {
            display_lost_message();
        }
    }
    else if (m_game_state == state_e::ENDING) {
        //
    }
}

bool SnakeGame::game_over() const
{
    return m_end_game;
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
    display_life(m_curr_lives);

    cout <<  " | "
         << "Score: " << m_score << " | "
         << "Food eaten: " << m_curr_foods << " of " << m_total_foods 
         << endl;

    draw_horizontal_line();
}

void SnakeGame::display_life(count_t curr_lives) {
    cout << "Lives: ";
    if (curr_lives == m_lives) {
        for (count_t i = 0; i < m_lives; i++)
            cout << "󰋑";
    }
    else {
        for (count_t i = 0; i < curr_lives; i++)
            cout << "󰋑";

        for (count_t i = 0; i < m_lives - curr_lives; i++)
            cout << "󰋕";
    }
}

void SnakeGame::display_won_message() const
{
    auto maze = m_level.maze();

    for (size_t i = 0; i < (m_level.rows() / 2) - 2; i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];
            cout << Level::render[cell.type()];
        }

        cout << endl;
    }
    cout << "+--------------------------------------------+\n";
    cout << "|       CONGRATILATIONS anaconda WON!        |\n";
    cout << "|            Thanks for playing!             |\n";
    cout << "+--------------------------------------------+\n";

    for (size_t i = (m_level.rows() / 2) + 2; i < m_level.rows(); i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];
            cout << Level::render[cell.type()];
        }

        cout << endl;
    }
}

void SnakeGame::display_lost_message() const
{
    auto maze = m_level.maze();

    for (size_t i = 0; i < (m_level.rows() / 2) - 2; i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];

            if (cell.type() == Cell::cell_e::SNAKE_HEAD) {
                cout << Level::render[Cell::cell_e::DEATH_SNAKE_HEAD];
            }
            else if (cell.type() == Cell::cell_e::SNAKE_BODY) {
                cout << Level::render[Cell::cell_e::DEATH_SNAKE_BODY];
            }
            else {
                cout << Level::render[cell.type()];
            }
        }

        cout << endl;
    }
    cout << "+--------------------------------------------+\n";
    cout << "|       Sorry, anaconda LOST the game!       |\n";
    cout << "|            Try again next time!            |\n";
    cout << "+--------------------------------------------+\n";

    for (size_t i = (m_level.rows() / 2) + 2; i < m_level.rows(); i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];

            if (cell.type() == Cell::cell_e::SNAKE_HEAD) {
                cout << Level::render[Cell::cell_e::DEATH_SNAKE_HEAD];
            }
            else if (cell.type() == Cell::cell_e::SNAKE_BODY) {
                cout << Level::render[Cell::cell_e::DEATH_SNAKE_BODY];
            }
            else {
                cout << Level::render[cell.type()];
            }
        }

        cout << endl;
    }
}

void SnakeGame::display_death_snake() const 
{
    auto maze = m_level.maze();
    
    for (size_t i = 0; i < m_level.rows(); i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];

            if (cell.type() == Cell::cell_e::SNAKE_HEAD) {
                cout << Level::render[Cell::cell_e::DEATH_SNAKE_HEAD];
            }
            else if (cell.type() == Cell::cell_e::SNAKE_BODY) {
                cout << Level::render[Cell::cell_e::DEATH_SNAKE_BODY];
            }
            else {
                cout << Level::render[cell.type()];
            }
        }

        cout << endl;
    }
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
