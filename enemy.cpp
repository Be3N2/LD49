
#include "enemy.h"

EnemyObject::EnemyObject() :
    GameObject()
{ }

EnemyObject::EnemyObject(glm::vec2 pos, glm::vec2 size, Texture2D spriteSheet) :
    GameObject(pos, size, spriteSheet, glm::vec3(1.0f))
{
    initAnimations();
}

void EnemyObject::initAnimations() {
    standingAnimRight = SpriteAnimation(140, 1, 1000, 10.0f, 20.0f, true);
    standingAnimLeft = SpriteAnimation(143, 1, 1000, 10.0f, 20.0f, true);
    walkingRightAnim = SpriteAnimation(140, 2, 1000, 10.0f, 20.0f, true);
    standingAnimLeft = SpriteAnimation(143, 2, 1000, 10.0f, 20.0f, true);
    attackRightAnim = SpriteAnimation(150, 8, 1000, 10.0f, 20.0f, false);
    attackLeftAnim = SpriteAnimation(160, 8, 1000, 10.0f, 20.0f, false);
    recoverLeftAnim = SpriteAnimation(170, 8, 1000, 10.0f, 20.0f, false);
    recoverRightAnim = SpriteAnimation(180, 8, 1000, 10.0f, 20.0f, false);
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
    renderer.DrawSprite(Sprite, Position, currentAnim, time, animStartTime, Size, Rotation, Color);
}