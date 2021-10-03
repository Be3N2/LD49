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
    GAME_WIN,
    GAME_OVER
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

const glm::vec2 PLAYER_SIZE(32.0f, 32.0f);
const float PLAYER_VELOCITY(50.0f);

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
    Collision CheckCollision(PlayerObject& one, EnemyObject& two);
    Collision CheckIfPlayerAttackHit(PlayerObject& one, EnemyObject& two);
    Collision CheckIfEnemyAttackHit(PlayerObject& one, EnemyObject& two);
    Direction VectorDirection(glm::vec2 target);
};
#endif