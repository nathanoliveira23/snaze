/**
 * @file snake_game.h
 *
 * @description
 * This class defines game execution rules during the simulation. 
 * It's responsible for receiving requests and providing responses 
 * according to data input.
 * This class is also responsible for displaying the game states 
 * during the simulation.
 */

#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>

#include "common.h"
#include "level.h"
#include "player.h"

using std::string;
using std::vector;
using std::pair;
using std::queue;
using std::list;
using std::cout, std::endl;

namespace snaze {

class SnakeGame {
public:
    //== Aliases.
    using count_t = unsigned;

    //== Enums/Structs.

    //!< The game states.
    enum class state_e : short {
        STARTING = 0,   //<! Beginning the game.
        WELLCOME,       //!< Opening messages.
        RUNNING,        //!< Playing the game.
        ENDING,         //!< Closing the game.
    };

    //!< The match states.
    enum class match_e : short {
        STARTING = 0,       //!< Begin new game.
        WALK_TO_DEATH,      //!< The snake is trapped in the maze.
        LOOKING_FOR_FOOD,   //!< The snake searches for food in the maze.
        NEXT_LEVEL,         //!< The next level when the player won the previous one.
        RESET,              //!< Reset the level.
        WIN,                //!< Match ended and snake won.
        LOST,               //!< Match ended and snake lost.
    };

    /// Default constructor.
    SnakeGame() = default;
    /// Default constructor.
    SnakeGame(const RunningOpt &);
    /// Destructor.
    ~SnakeGame() = default;

    //=== Common methods for the Game Loop design pattern.
    /// Defines simulation settings.
    void initialize(const list<vector<vector<char>>>&);
    /// Process user input events, depending on the current game state.
    void process_events();
    /// Update the game based on the current game state.
    void update();
    /// Renders the game to the user.
    void render();
    /// Return true if the snake lost/won the game.
    bool game_over() const;

    /// Returns the fps game.
    count_t fps() const { return m_fps; }

private:
    /// Show the welcome mesage.
    void display_welcome() const;
    /// Show the game info.
    void display_game_info() const;
    /// Show the match info.
    void display_match_info();
    /// Show system message to player.
    void display_system_messages() const;
    /// Show the won message.
    void display_won_message() const;
    /// Show the lost message.
    void display_lost_message() const;
    /// Show the maze with the death snake.
    void display_death_snake() const;
    /// Shows the snake's number of lives.
    void display_life(count_t);

    /// Draw a horizontal line on the screen.
    void draw_horizontal_line() const;

    /// Reads a simple enter from the user.
    void read_enter() const;

    state_e m_game_state;   //!< The current game state.
    match_e m_match_state;  //!< The current match state.
    list<Level> m_levels;   //!< A list of mazes.
    Level m_level;          //!< The  current maze.
    Player m_player;        //!< The AI engine.

    string m_system_msg;    //!< Current system message displayed to user.

    bool m_end_game;        //!< Flag that indicates wether the game is over. 

    count_t m_score;        //!< The score in a match.
    count_t m_n_levels;     //!< The number of mazes.
    count_t m_fps;          //!< The fps game.
    count_t m_total_foods;  //!< The amount of food in a maze.
    count_t m_curr_foods;   //!< The current amount of food found by the snake.
    count_t m_lives;        //!< The number of lives of the snake.
    count_t m_curr_lives;   //!< The snake's current life count.
    player_e m_player_type; //!< The player type.
};

} // NAMESPACE SNAZE

#endif
