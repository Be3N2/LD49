
#include "enemy.h"

EnemyObject::EnemyObject() :
    GameObject()
{ }

EnemyObject::EnemyObject(glm::vec2 pos, glm::vec2 size, Texture2D spriteSheet, unsigned int healthTotal, unsigned int gameWidth, unsigned int gameHeight) :
    GameObject(pos, size, spriteSheet, glm::vec3(1.0f)), direction(0), health(healthTotal),
    boardWidth(gameWidth), boardHeight(gameHeight), hit(false), maxSpeed(32.0f),
    knockbackDuration(0.0f), knockbackTimer(0.0f), knockback(0.0f, 0.0f), attackRange(4.0f, 8.0f),
    animStartTime(0), animLocked(false), attacking(false), recovering(false), testHit(false)
{
    initAnimations();
}

void EnemyObject::initAnimations() {
    standingAnimRight = SpriteAnimation(140, 1, 1000, 10.0f, 20.0f, true);
    standingAnimLeft = SpriteAnimation(143, 1, 1000, 10.0f, 20.0f, true);
    walkingRightAnim = SpriteAnimation(140, 2, 1000, 10.0f, 20.0f, true);
    walkingLeftAnim = SpriteAnimation(143, 2, 1000, 10.0f, 20.0f, true);
    attackRightAnim = SpriteAnimation(150, 8, 300, 10.0f, 20.0f, false);
    attackLeftAnim = SpriteAnimation(160, 8, 300, 10.0f, 20.0f, false);
    recoverRightAnim = SpriteAnimation(170, 8, 3000, 10.0f, 20.0f, false);
    recoverLeftAnim = SpriteAnimation(180, 8, 3000, 10.0f, 20.0f, false);
}

void EnemyObject::Update(double dt, glm::vec2 playerPos, bool playerOnTheGround, double glfwTime) {
    
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
    
    Move(dt, playerPos);
    if (!attacking && !recovering) {
        bool inRange = checkInRange(playerPos, playerOnTheGround);

        if (inRange) {
            attacking = true;
            testHit = true; // tells game to test if attack hit the player and damage accordingly
            if (direction == 0)
                startAnimation(attackLeftAnim, glfwTime);
            else 
                startAnimation(attackRightAnim, glfwTime);
        }
    }

    if (attacking || recovering) {
        if (int(glfwTime * 1000) > activeAnimation.playbackSpeed + animStartTime) {
            endAnimation(glfwTime);
        }
    }

    if (Position.x < 0)
        Position.x = 0;
    if (Position.x + Size.x >= boardWidth - 8)
        Position.x = boardWidth - Size.x - 8;
    if (Position.y < 0)
        Position.y = 0;
    if (Position.y + Size.y >= boardHeight)
        Position.y = boardHeight - Size.y;

}

void EnemyObject::Move(double dt, glm::vec2 playerPos) {
    
    // get a normalized vector towards the player
    glm::vec2 moveDir = glm::normalize(playerPos - (Position + Size/2.0f));

    if (recovering) {
        moveDir *= -1;
    }
    Position += moveDir * float(maxSpeed * dt);

    // set facing direction
    if (moveDir.x > 0)
        direction = 1;
    else
        direction = 0;
}

bool EnemyObject::checkInRange(glm::vec2 playerPos, bool playerOnTheGround) {
    float distanceToPlayer = glm::length(playerPos - (Position + Size / 2.0f));
    if (playerOnTheGround) {
        return distanceToPlayer < 30; // get closer to guarantee hit
    }
    return distanceToPlayer < 40;
}

void EnemyObject::Draw(SpriteRenderer& renderer, double time) {
    if (hit)
        Color = glm::vec3(1.0f, 0.4f, 0.4f);
    else
        Color = glm::vec3(1.0f);

    if (animLocked) {
        renderer.DrawSprite(Sprite, Position, activeAnimation, time, animStartTime, Size, Rotation, Color);
        return;
    }

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

void EnemyObject::startAnimation(SpriteAnimation animation, double startTime) {
    animLocked = true;
    activeAnimation = animation;
    animStartTime = int(startTime * 1000);
}

void EnemyObject::endAnimation(double time) {
    animLocked = false;
    if (recovering) {
        recovering = false;
    }
    if (attacking) {
        attacking = false;
        recovering = true;
        if (direction == 0)
            startAnimation(recoverLeftAnim, time);
        else
            startAnimation(recoverRightAnim, time);
    }
}