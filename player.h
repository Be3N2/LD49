#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "texture.h"

class PlayerObject : public GameObject {
public:
    SpriteAnimation
        standingAnimLeft, standingAnimRight, standingAnimUp, standingAnimDown,
        movingRightAnim, movingLeftAnim, movingDownAnim, movingUpAnim,
        attackingAnim, attackMissAnim, onTheGroundAnim, getUpAnim;
                    
    unsigned int direction; // 0 1 2 3 clockwise for direction
    bool moving, attacking, animLocked, onTheGround, missed, triggerGetUp, gettingUp, hit;
    unsigned int health;
    SpriteAnimation activeAnimation;
    int animStartTime;

    // knockback variables
    float knockbackDuration, knockbackTimer;
    glm::vec2 knockback;
    glm::vec2 distanceTraveled;

    PlayerObject();
    PlayerObject(glm::vec2 pos, glm::vec2 size, Texture2D spriteSheet);
    void Update(double dt, double glfwTime);
    void Draw(SpriteRenderer& renderer, double time);
private:
    void initAnimations();
    void startAnimation(SpriteAnimation animation, double startTime);
    void endAnimation();
};

#endif
