#ifndef ENEMY_H
#define ENEMY_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "texture.h"

class EnemyObject : public GameObject {
public:
    SpriteAnimation standingAnimLeft,
        standingAnimRight,
        walkingRightAnim,
        walkingLeftAnim,
        attackLeftAnim,
        recoverLeftAnim,
        attackRightAnim,
        recoverRightAnim;

    unsigned int health;
    unsigned int boardWidth, boardHeight;
    unsigned int direction; // 0 1 left or right
    float maxSpeed;
    glm::vec2 attackRange;
    bool moving, attacking, recovering, hit, animLocked;
    float knockbackDuration;
    float knockbackTimer;
    glm::vec2 knockback;
    glm::vec2 distanceTraveled;
    
    SpriteAnimation activeAnimation;
    int animStartTime;

    EnemyObject();
    EnemyObject(glm::vec2 pos, glm::vec2 size, Texture2D spriteSheet, unsigned int healthTotal, unsigned int gameWidth, unsigned int gameHeight);
    void Update(double dt, glm::vec2 playerPos, double glfwTime);
    void MoveTowardsPlayer(double dt, glm::vec2 playerPos);
    bool checkInRange(glm::vec2 playerPos);
    void Draw(SpriteRenderer& renderer, double time);
private:
    void initAnimations();
    void startAnimation(SpriteAnimation animation, double startTime);
    void endAnimation(double time);
};

#endif
