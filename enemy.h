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

    unsigned int direction; // 0 1 2 3 clockwise for direction
    bool moving, attacking;
    int animStartTime;
    EnemyObject();
    EnemyObject(glm::vec2 pos, glm::vec2 size, Texture2D spriteSheet);
    void Draw(SpriteRenderer& renderer, double time);
private:
    void initAnimations();
};

#endif
