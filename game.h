#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>

#include "game_object.h"
#include "enemy.h"
#include "player.h"
#include <ft2build.h>

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

// Initial size of player paddle
const glm::vec2 PLAYER_SIZE(32.0f, 32.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(100.0f);

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    bool                    MouseLeft, MouseRight;
    unsigned int            Scale;
    unsigned int            Width, Height;
    unsigned int            ScreenWidth, ScreenHeight;

    // constructor/destructor
    Game(unsigned int screenWidth, unsigned int screenHeight, unsigned int scale);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void DoCollisions();
    void ResetLevel();
    void ResetPlayer();
    Collision CheckCollision(PlayerObject& one, EnemyObject& two);
    Collision CheckIfPlayerAttackHit(PlayerObject& one, EnemyObject& two);
    //Collision CheckIfEnemyAttackHit(PlayerObject& one, EnemyObject& two);
    Direction VectorDirection(glm::vec2 target);
};
#endif