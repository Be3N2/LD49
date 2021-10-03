
#include "enemy.h"
#include <iostream>

EnemyObject::EnemyObject() :
    GameObject()
{ }

EnemyObject::EnemyObject(glm::vec2 pos, glm::vec2 size, Texture2D spriteSheet, unsigned int healthTotal, unsigned int gameWidth, unsigned int gameHeight) :
    GameObject(pos, size, spriteSheet, glm::vec3(1.0f)), direction(0), health(healthTotal),
    boardWidth(gameWidth), boardHeight(gameHeight), hit(false),
    knockbackDuration(0.0f), knockbackTimer(0.0f), knockback(0.0f, 0.0f)
{
    initAnimations();
}

void EnemyObject::initAnimations() {
    standingAnimRight = SpriteAnimation(140, 1, 1000, 10.0f, 20.0f, true);
    standingAnimLeft = SpriteAnimation(143, 1, 1000, 10.0f, 20.0f, true);
    walkingRightAnim = SpriteAnimation(140, 2, 1000, 10.0f, 20.0f, true);
    walkingLeftAnim = SpriteAnimation(143, 2, 1000, 10.0f, 20.0f, true);
    attackRightAnim = SpriteAnimation(150, 8, 1000, 10.0f, 20.0f, false);
    attackLeftAnim = SpriteAnimation(160, 8, 1000, 10.0f, 20.0f, false);
    recoverLeftAnim = SpriteAnimation(170, 8, 1000, 10.0f, 20.0f, false);
    recoverRightAnim = SpriteAnimation(180, 8, 1000, 10.0f, 20.0f, false);
}

void EnemyObject::Update(double dt) {
    
    // apply knockback
    if (knockbackDuration > 0.01f) {
        knockbackDuration -= dt;
        float percentageComplete = 1 - (knockbackDuration / knockbackTimer);
        glm::vec2 distanceFromStart = knockback * (1 - ((1 - percentageComplete) * (1 - percentageComplete))); // 1 - (1-t)(1-t) deceleration interpolation
        Position += distanceFromStart - distanceTraveled;
        distanceTraveled = distanceFromStart;

        if (knockbackDuration <= 0.01f) {
            hit = false;
        }
    }

    if (Position.x < 0)
        Position.x = 0;
    if (Position.x + Size.x >= boardWidth)
        Position.x = boardWidth - Size.x;
    if (Position.y < 0)
        Position.y = 0;
    if (Position.y + Size.y >= boardHeight)
        Position.y = boardHeight - Size.y;

}

void EnemyObject::Draw(SpriteRenderer& renderer, double time) {
    SpriteAnimation currentAnim;

    switch (direction) {
    case 0:
        currentAnim = standingAnimLeft;
        break;
    case 1:
    default:
        currentAnim = standingAnimRight;
        break;
    }
    if (hit)
        Color = glm::vec3(1.0f, 0.4f, 0.4f);
    else
        Color = glm::vec3(1.0f);
    renderer.DrawSprite(Sprite, Position, currentAnim, time, animStartTime, Size, Rotation, Color);
}