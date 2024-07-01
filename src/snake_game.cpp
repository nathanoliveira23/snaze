#include <cstdio>
#include <cstdlib>

#include "snake_game.h"
#include "cell.h"
#include "common.h"
#include "level.h"
#include "player.h"

namespace snaze {

/**
 * @brief Constructs a Snake game instance with given options.
 * 
 * This constructor initializes a Snake game instance with specified options,
 * including the number of total foods, frames per second (fps), number of lives,
 * and type of player intelligence.
 * 
 * @param opt The running options for configuring the Snake game.
 */
SnakeGame::SnakeGame(const RunningOpt& opt)
{
    m_total_foods = opt.foods;       // Initialize total foods for the game.
    m_fps = opt.fps;                 // Initialize frames per second.
    m_lives = opt.lives;             // Initialize number of lives.
    m_player_type = opt.player_type; // Initialize type of player intelligence.
}

/**
 * @brief Initializes the Snake game with the given maze configuration.
 * 
 * This function sets up the initial state of the Snake game using the provided maze configuration.
 * It creates instances of the Level and Player classes, initializes game state variables,
 * and sets up initial messages for the player.
 * 
 * @param maze The maze configuration represented as a vector of vectors of characters.
 */
void SnakeGame::initialize(const vector<vector<char>> &maze)
{
    m_level = Level(maze);
    m_player = Player(m_level);
    m_game_state = state_e::STARTING;

    m_curr_foods = 0;                // Initialize current number of foods eaten.
    m_curr_lives = m_lives;          // Initialize current number of lives remaining.
    m_score = 0;                     // Initialize game score.
    m_n_levels = 1;                  // Initialize the number of levels.
    m_curr_foods = 0;                // Initialize the current number of foods.
    m_end_game = false;              // Initialize the end game flag.

    m_system_msg = "Press <ENTER> to start the game!";
}

/**
 * @brief Processes events based on the current game and match states.
 * 
 * This function handles specific events depending on the current states of the game.
 * 
 */
void SnakeGame::process_events()
{
    if (m_game_state == state_e::WELLCOME) {
        read_enter();
    }
    else if (m_game_state == state_e::RUNNING) {
        if (m_match_state == match_e::RESET or m_match_state == match_e::WIN or m_match_state == match_e::LOST) {
            read_enter();
        }
    }
}

/**
 * @brief Updates the game state.
 * 
 * This function is responsible for updating the game state or 
 * game logic based on specific state.
 */
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
            // Add food to the maze.
            m_level.add_food();

            // Place the snake at its spawn position.
            m_level.place_snake(m_level.spawn());

            // Initialize the player with the current level configuration.
            m_player = Player(m_level);

            // Determine if there's a solution path from the snake's spawn to the food.
            bool has_solution = m_player.find_solution(m_level.spawn(), m_level.food());

            // Update the match state based on whether a solution was found.
            m_match_state = has_solution ? match_e::LOOKING_FOR_FOOD
                                         : match_e::WALK_TO_DEATH;
        }
        else if (m_match_state == match_e::LOOKING_FOR_FOOD) {
            // Get the next move from the player.
            auto [step, direction] = m_player.next_move();

            // Check if the step taken by the snake is the position of the food.
            bool found_food = step == m_level.food();

            // Update the level based on the snake's movement and whether food was found.
            m_level.update(step, direction, found_food);

            if (found_food) {
                bool death = true; //!< Flag that indicates whether the snake was surrounded after eating the food.
                for (const dir_e dir : { UP, LEFT, DOWN, RIGHT }) {
                    // If there is a clear path, the snake is not surrounded.
                    if (not m_level.is_blocked(step, dir)) {
                        death = false;
                        break;
                    }
                }

                // Spawn new food, update game state and scores.
                m_level.spawn(step);
                m_curr_foods++;
                m_score += 20;

                // Check if the snake dies. (surrounded by walls or itself)
                if (death) {
                    m_curr_lives -= 1;
                    if (m_curr_lives == 0)
                        m_match_state = match_e::LOST;
                    else 
                        // Reset the game state if lives remain.
                        m_match_state = match_e::RESET;
                }
                else {
                    // Restart the game to continue playing.
                    m_match_state = match_e::STARTING;
                }

                // Check if all food has been eaten to win the game.
                if (m_curr_foods == m_total_foods) {
                    m_match_state = match_e::WIN;
                    //m_end_game = true;

                    m_system_msg = "Press <ENTER> to continue...";
                }
            }
        }
        else if (m_match_state == match_e::WALK_TO_DEATH) {
            // Get the next move from the player.
            auto [step, direction] = m_player.next_move();

            // Update the level if the snake's next move is not blocked.
            if (not m_level.is_blocked(step, direction)) {
                m_level.update(step, direction, false);
            }

            // Spawn a new food at the snake's last position.
            m_level.spawn(m_player.last_move());

            // Check if the snake has no more steps left.
            if (m_player.amount_of_steps() == 0) {
                // Decrease the score by 20 points, ensure it does not go below zero.
                m_score = (m_score - 20 < 0) ? 0 : m_score - 20;

                // Decrease the current number of lives by 1.
                m_curr_lives -= 1;

                // Check if there are no lives left.
                if (m_curr_lives == 0) {
                    // End the game if no lives left.
                    //m_end_game = true;
                    m_match_state = match_e::LOST;
                    m_system_msg = "Press <ENTER> to continue...";
                    return;
                }
                else {
                    // Display system message to prompt player to try again.
                    m_system_msg = "Press <ENTER> to try again.";
                }

                // Reset the game state.
                m_match_state = match_e::RESET;
            }
        }
        else if (m_match_state == match_e::RESET) {
            m_level.reset();
            m_match_state = match_e::STARTING;
        }
        else if (m_match_state == match_e::LOST or m_match_state == match_e::WIN) {
            m_game_state = state_e::ENDING;
        }
    }
    else if (m_game_state == state_e::ENDING) {
        m_end_game = true;
    }
}

/**
 * @brief Renders the current game state.
 * 
 * This function handles the rendering of the current game state to the screen.
 */
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
        draw_horizontal_line();
        display_match_info();
        if (m_match_state == match_e::STARTING) {
            cout << m_level.to_string();
        }
        else if (m_match_state == match_e::LOOKING_FOR_FOOD) {
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
            display_system_messages();
        }
        else if (m_match_state == match_e::LOST) {
            display_lost_message();
            display_system_messages();
        }
    }
    else if (m_game_state == state_e::ENDING) {
        if (m_match_state == match_e::WIN)
            display_won_message();
        else if (m_match_state == match_e::LOST)
            display_lost_message();
    }
}

/**
 * @brief Checks if the game is over.
 * 
 * @return true if the game is over, false otherwise.
 */
bool SnakeGame::game_over() const
{
    return m_end_game;
}

/**
 * @brief Displays a welcome message for the Snake Game.
 * 
 * This function prints a welcome message to the console,
 * including the title of the game and copyright information.
 * It also draws a horizontal line after printing the message.
 */
void SnakeGame::display_welcome() const 
{
    std::cout << " --> Welcome to the classic Snake Game <-- \n"; 
    std::cout << "      copyright DIMAp/UFRN 2013-2024\n";

    draw_horizontal_line();
}

/**
 * @brief Displays general game information.
 * 
 * This function prints general information about the game, including
 * the number of levels loaded, the number of lives remaining for the snake,
 * and the total number of apples to eat across all levels. It also provides
 * a good luck message to the player.
 */
void SnakeGame::display_game_info() const
{
    std::cout << " Levels loaded: " << m_n_levels << " | "
              << "Snake lives: " << m_lives << " | "
              << "Apples to eat: " << m_total_foods << std::endl;

    std::cout << " Clear all levels to win the game. Good luck!!!\n";

    draw_horizontal_line();
}

/**
 * @brief Displays the current system message.
 * 
 * This function prints the current system message to the console,
 * followed by a blank line.
 */
void SnakeGame::display_system_messages() const
{
    std::cout << ">>> " << m_system_msg << std::endl << std::endl;
}

/**
 * @brief Displays the current match information.
 * 
 * This function prints the current status of the match, including the number
 * of lives remaining, the current score, and the amount of food eaten out of
 * the total amount.
 */
void SnakeGame::display_match_info()
{
    display_life(m_curr_lives);

    std::cout << " | "
              << "Score: " << m_score << " | "
              << "Food eaten: " << m_curr_foods << " of " << m_total_foods 
              << std::endl;

    draw_horizontal_line();
}

/**
 * @brief Displays the current number of lives remaining.
 * 
 * This function prints a visual representation of the current number of lives
 * remaining in the game, using symbols to represent lives.
 * 
 * @param curr_lives The current number of lives remaining.
 */
void SnakeGame::display_life(count_t curr_lives) {
    std::cout << "Lives: ";
    if (curr_lives == m_lives) {
        for (count_t i = 0; i < m_lives; i++)
            std::cout << "󰋑";
    }
    else {
        for (count_t i = 0; i < curr_lives; i++)
            std::cout << "󰋑";

        for (count_t i = 0; i < m_lives - curr_lives; i++)
            std::cout << "󰋕";
    }
}

/**
 * @brief Displays a message indicating the game has been won.
 * 
 * This function displays a visual representation of the game board from the `m_level`
 * object with the snake head replaced by its directional character ('v', '^', '<', '>') from
 * the ASCII representation of the maze. It then prints a congratulatory message indicating
 * that the game has been won.
 */
void SnakeGame::display_won_message() const
{
    auto maze = m_level.maze();
    std::string maze_ascii = m_level.to_string();

    const char* head = "v^><";
    size_t idx = maze_ascii.find_first_of(head);

    // Display top portion of the board with snake head directional characters.
    for (size_t i = 0; i < (m_level.rows() / 2) - 2; i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];
            if (cell.type() == Cell::cell_e::SNAKE_HEAD)
                std::cout << maze_ascii[idx];
            else
                std::cout << Level::render[cell.type()];
        }

        std::cout << std::endl;
    }

    // Display message indicating the game has been won.
    std::cout << "+--------------------------------------------+\n";
    std::cout << "|       CONGRATULATIONS anaconda WON!        |\n";
    std::cout << "|            Thanks for playing!             |\n";
    std::cout << "+--------------------------------------------+\n";

    // Display bottom portion of the board with snake head directional characters.
    for (size_t i = (m_level.rows() / 2) + 2; i < m_level.rows(); i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];
            if (cell.type() == Cell::cell_e::SNAKE_HEAD)
                std::cout << maze_ascii[idx];
            else
                std::cout << Level::render[cell.type()];
        }

        std::cout << std::endl;
    }
}

/**
 * @brief Displays a message indicating the game has been lost.
 * 
 * This function displays a visual representation of the game board from the `m_level`
 * object with death snake visuals, and then prints a message indicating that the game
 * has been lost, encouraging the player to try again next time.
 */
void SnakeGame::display_lost_message() const
{
    auto maze = m_level.maze();

    // Display top portion of the board with death snake visuals.
    for (size_t i = 0; i < (m_level.rows() / 2) - 2; i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];

            if (cell.type() == Cell::cell_e::SNAKE_HEAD) {
                std::cout << Level::render[Cell::cell_e::DEATH_SNAKE_HEAD];
            }
            else if (cell.type() == Cell::cell_e::SNAKE_BODY) {
                std::cout << Level::render[Cell::cell_e::DEATH_SNAKE_BODY];
            }
            else {
                std::cout << Level::render[cell.type()];
            }
        }

        std::cout << std::endl;
    }

    // Display message indicating the game has been lost.
    std::cout << "+--------------------------------------------+\n";
    std::cout << "|       Sorry, anaconda LOST the game!       |\n";
    std::cout << "|            Try again next time!            |\n";
    std::cout << "+--------------------------------------------+\n";

    // Display bottom portion of the board with death snake visuals.
    for (size_t i = (m_level.rows() / 2) + 2; i < m_level.rows(); i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];

            if (cell.type() == Cell::cell_e::SNAKE_HEAD) {
                std::cout << Level::render[Cell::cell_e::DEATH_SNAKE_HEAD];
            }
            else if (cell.type() == Cell::cell_e::SNAKE_BODY) {
                std::cout << Level::render[Cell::cell_e::DEATH_SNAKE_BODY];
            }
            else {
                std::cout << Level::render[cell.type()];
            }
        }

        std::cout << std::endl;
    }
}

/**
 * @brief Displays the current state of the game board with death snake visuals.
 * 
 * This function retrieves the maze state from the `m_level` object and displays it,
 * replacing snake head and body cells with their respective death visuals if present.
 */
void SnakeGame::display_death_snake() const 
{
    auto maze = m_level.maze();
    
    // Iterate through each cell in the maze.
    for (size_t i = 0; i < m_level.rows(); i++) {
        for (size_t j = 0; j < m_level.cols(); j++) {
            Cell cell = maze[i][j];

            // Replace snake head with death snake head visual.
            if (cell.type() == Cell::cell_e::SNAKE_HEAD) {
                std::cout << Level::render[Cell::cell_e::DEATH_SNAKE_HEAD];
            }
            // Replace snake body with death snake body visual.
            else if (cell.type() == Cell::cell_e::SNAKE_BODY) {
                std::cout << Level::render[Cell::cell_e::DEATH_SNAKE_BODY];
            }
            // Otherwise, display the regular cell visual.
            else {
                std::cout << Level::render[cell.type()];
            }
        }

        // Move to the next line after printing each row.
        std::cout << std::endl;
    }
}

/**
 * @brief Reads and discards input characters until a newline character is encountered.
 * 
 * This function reads characters from standard input until it encounters a newline character ('\n').
 * It discards all characters read during this process.
 */
void SnakeGame::read_enter() const
{
    char enter;

    // Continue reading characters until a newline ('\n') is encountered.
    while (enter != '\n')
        enter = getchar();
}

/**
 * @brief Draws a horizontal line of '-' characters on the console.
 * 
 * Draws a horizontal line of '-' characters, with a length of 80 characters,
 * on the console output followed by a newline.
 */
void SnakeGame::draw_horizontal_line() const 
{
    std::cout << std::string(80, '-') << std::endl;
}

} // NAMESPACE SNAZE
