
#include "player.h"
#include <iostream>

PlayerObject::PlayerObject() :
    GameObject()
{ }

PlayerObject::PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D spriteSheet) :
    GameObject(pos, size, spriteSheet, glm::vec3(1.0f)), moving(false), 
    attacking(false), animLocked(false), onTheGround(false), missed(false),
    triggerGetUp(false), gettingUp(false), hit(false), health(5),
    knockbackDuration(0.0f), knockbackTimer(0.0f), knockback(0.0f), distanceTraveled(0.0f)
{
    initAnimations();
}

void PlayerObject::initAnimations() {
    standingAnimRight = SpriteAnimation(10, 8, 1000, 10.0f, 20.0f, true);
    standingAnimLeft = SpriteAnimation(20, 8, 1000, 10.0f, 20.0f, true);
    standingAnimDown = SpriteAnimation(30, 8, 1000, 10.0f, 20.0f, true);
    standingAnimUp = SpriteAnimation(40, 4, 500, 10.0f, 20.0f, true);
    movingRightAnim = SpriteAnimation(50, 6, 1000, 10.0f, 20.0f, true);
    movingLeftAnim = SpriteAnimation(60, 6, 1000, 10.0f, 20.0f, true);
    movingDownAnim = SpriteAnimation(70, 6, 1000, 10.0f, 20.0f, true);
    movingUpAnim = SpriteAnimation(80, 6, 1000, 10.0f, 20.0f, true);
    attackingAnim = SpriteAnimation(100, 6, 500, 10.0f, 20.0f, false);
    attackMissAnim = SpriteAnimation(110, 10, 750, 10.0f, 20.0f, false);
    onTheGroundAnim = SpriteAnimation(4, 1, 1000, 10.0f, 20.0f, true);
    getUpAnim = SpriteAnimation(120, 5, 500, 10.0f, 20.0f, false);
}

void PlayerObject::Update(double dt, double glfwTime) {
    if (attacking && !animLocked) {
        if (missed) 
            startAnimation(attackMissAnim, glfwTime);
        else
            startAnimation(attackingAnim, glfwTime);
    }

    if (onTheGround && triggerGetUp) {
        triggerGetUp = false;
        gettingUp = true;
        startAnimation(getUpAnim, glfwTime);
    }

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
}

void PlayerObject::Draw(SpriteRenderer& renderer, double time) {
    if (hit)
        Color = glm::vec3(1.0f, 0.4f, 0.4f);
    else
        Color = glm::vec3(1.0f);

    if (animLocked && animStartTime + activeAnimation.playbackSpeed < int(time * 1000)) {
        endAnimation();
    }

    if (animLocked) {
        renderer.DrawSprite(Sprite, Position, activeAnimation, time, animStartTime, Size, Rotation, Color);
        return;
    }

    SpriteAnimation currentAnim;
    if (onTheGround) { // highest priority
        currentAnim = onTheGroundAnim;
    } 
    else if (moving) {
        switch (direction) {
        case 0:
            currentAnim = movingUpAnim;
            break;
        case 1:
            currentAnim = movingRightAnim;
            break;
        case 2:
            currentAnim = movingDownAnim;
            break;
        case 3:
        default:
            currentAnim = movingLeftAnim;
            break;
        }
    }
    else {
        switch (direction) {
        case 0:
            currentAnim = standingAnimUp;
            break;
        case 1:
            currentAnim = standingAnimRight;
            break;
        case 2:
            currentAnim = standingAnimDown;
            break;
        case 3:
        default:
            currentAnim = standingAnimLeft;
            break;
        }
    }
    renderer.DrawSprite(Sprite, Position, currentAnim, time, animStartTime, Size, Rotation, Color);
}

void PlayerObject::startAnimation(SpriteAnimation animation, double startTime) {
    animLocked = true;
    activeAnimation = animation;
    animStartTime = int(startTime * 1000);
}

void PlayerObject::endAnimation() {
    animLocked = false;
    if (attacking) {
        attacking = false;
        if (missed) {
            onTheGround = true;
            missed = false;
        }
    }
    if (gettingUp) {
        gettingUp = false;
        onTheGround = false;
    }
}