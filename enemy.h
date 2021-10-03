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
    unsigned int direction; // 0 1 2 3 clockwise for direction
    bool moving, attacking, hit;
    float knockbackDuration;
    float knockbackTimer;
    glm::vec2 knockback;
    glm::vec2 distanceTraveled;
    int animStartTime;
    EnemyObject();
    EnemyObject(glm::vec2 pos, glm::vec2 size, Texture2D spriteSheet, unsigned int healthTotal, unsigned int gameWidth, unsigned int gameHeight);
    void Update(double dt);
    void Draw(SpriteRenderer& renderer, double time);
private:
    void initAnimations();
};

#endif
