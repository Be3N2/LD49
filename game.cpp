#include <algorithm>

#include "game.h"

#include "sprite_renderer.h"
#include "resource_manager.h"
#include "post_processor.h"
#include "text_renderer.h"
#include "enemy.h"

#include <iostream>
#include <string>
#include <sstream>

#include <irrklang/irrKlang.h>

PlayerObject* Player;
EnemyObject* Enemy;
SpriteRenderer* Renderer;
PostProcessor* Effects;
TextRenderer* Text;
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

float ShakeTime = 0.0f;
int getUpCounter = 0; // keeps track of number of times q has been pressed to get up

Game::Game(unsigned int screenWidth, unsigned int screenHeight, unsigned int scale)
    : State(GAME_ACTIVE), Keys(), ScreenWidth(screenWidth), ScreenHeight(screenHeight), Scale(scale), Width(screenWidth / scale), Height(screenHeight / scale)
{

}

Game::~Game()
{
    delete Player;
    delete Enemy;
    delete Renderer;
    delete Effects;
    delete Text;
    SoundEngine->drop();
}

void Game::Init()
{
    // load Shaders
    ResourceManager::LoadShader("sprite.vert", "sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("particle.vert", "particle.frag", nullptr, "particle");
    ResourceManager::LoadShader("post_processing.vert", "post_processing.frag", nullptr, "post_processing");
    
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);

    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    ResourceManager::LoadTexture("res/map.png", true, "background");
    ResourceManager::LoadTexture("res/sprites.png", true, "spriteSheet");
    Effects = new PostProcessor(ResourceManager::GetShader("post_processing"), Width, Height, ScreenWidth, ScreenHeight);

    Player = new PlayerObject(glm::vec2(30.0f, 80.f), PLAYER_SIZE, ResourceManager::GetTexture("spriteSheet"));
    Enemy = new EnemyObject(glm::vec2(180.0f, 80.0f), glm::vec2(32.0f, 32.0f), ResourceManager::GetTexture("spriteSheet"), 5, Width, Height);

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("OCRAEXT.TTF", 24);

    //SoundEngine->play2D("breakout.mp3", true);
}

void Game::Update(float dt)
{
    // check for collisions
    this->DoCollisions();
    double glfwTime = glfwGetTime();

    Player->Update(glfwTime);
    Enemy->Update(dt, Player->Position, glfwTime);
}

void Game::ResetLevel() {
    
}

void Game::ResetPlayer() {
    
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        bool moving = false;
        // move playeboard
        if (this->Keys[GLFW_KEY_W])
        {
            if (!Player->onTheGround && Player->Position.y >= 0) {
                Player->Position.y -= velocity;
                Player->direction = 0;
                moving = true;
            }
        }
        if (this->Keys[GLFW_KEY_A])
        {
            if (!Player->onTheGround && Player->Position.x >= 0) {
                Player->Position.x -= velocity;
                Player->direction = 3;
                moving = true;
            }
        }
        if (this->Keys[GLFW_KEY_S])
        {
            if (!Player->onTheGround && Player->Position.y <= Height - Player->Size.y) {
                Player->Position.y += velocity;
                Player->direction = 2;
                moving = true;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (!Player->onTheGround && Player->Position.x <= Width - Player->Size.x) {
                Player->Position.x += velocity;
                Player->direction = 1;
                moving = true;
            }
        }
        Player->moving = moving;
        if (!Player->attacking && MouseLeft && !Player->onTheGround) {
            Collision result = CheckIfPlayerAttackHit(*Player, *Enemy);
            Player->attacking = true;
            Player->missed = !std::get<0>(result);
            Enemy->hit = std::get<0>(result);
            if (std::get<0>(result)) {
                // apply knockback force to enemy
                Enemy->knockbackDuration = 0.4f;
                Enemy->knockbackTimer = 0.4f;
                Enemy->distanceTraveled = glm::vec2(0.0f, 0.0f);
                switch (std::get<1>(result)) {
                case UP:
                    Enemy->knockback = glm::vec2(0.0f, -16.0f);
                    break;
                case DOWN:
                    Enemy->knockback = glm::vec2(0.0f, 16.0f);
                    break;
                case LEFT:
                    Enemy->knockback = glm::vec2(-16.0f, 0.0f);
                    break;
                case RIGHT:
                    Enemy->knockback = glm::vec2(16.0f, 0.0f);
                    break;
                }
            }
        }
        if (this->Keys[GLFW_KEY_Q] && !KeysProcessed[GLFW_KEY_Q])
        {
            KeysProcessed[GLFW_KEY_Q] = true;
            if (Player->onTheGround) {
                getUpCounter++;
                if (getUpCounter > 20) {
                    getUpCounter = 0;
                    Player->triggerGetUp = true;
                }
            }
        }
    }
}

void Game::Render()
{
    if (State == GAME_ACTIVE || State == GAME_MENU || this->State == GAME_WIN) {
        Effects->BeginRender();
        //std::cout << "Begin Render GL Error: " << glGetError() << std::endl;
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"),
            glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
        );
        //std::cout << "Draw SPrite GL Error: " << glGetError() << std::endl;

        Enemy->Draw(*Renderer, glfwGetTime());
        Player->Draw(*Renderer, glfwGetTime());
        
        Effects->EndRender();
        //std::cout << "End Render GL Error: " << glGetError() << std::endl;
        Effects->Render(glfwGetTime());
        //std::cout << "Effects Render GL Error: " << glGetError() << std::endl;

        // draw text
        if (Player->onTheGround) {
            Text->RenderText("Your On the Ground", Width / 2 - 70, 20, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
            Text->RenderText("Rapidly tap Q to get back up!", Width / 2 - 90, 40, 0.4f, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (Enemy->hit) {
            Text->RenderText("ENEMY HIT", Width / 2 - 70, 20, 0.5f, glm::vec3(1.0f, 0.0f, 1.0f));
        }
    }
}

void Game::DoCollisions()
{
    // check player against enemy
    Collision result = CheckCollision(*Player, *Enemy);
    if (std::get<0>(result)) {
        float penetration;
        switch (std::get<1>(result)) {
        case UP:
            penetration = Enemy->Size.y - (Enemy->Size.y - (Enemy->Position.y - Player->Position.y + 16));
            Player->Position.y += penetration;
            break;
        case DOWN:
            penetration = Player->Size.y - (Enemy->Position.y + 16 - Player->Position.y);
            Player->Position.y -= penetration;
            break;
        case LEFT:
            penetration = Enemy->Size.x - (Player->Position.x - Enemy->Position.x) - 16; 
            Player->Position.x += penetration;
            break;
        case RIGHT:
            penetration = (Player->Position.x + Player->Size.x - 8) - (Enemy->Position.x + 8) ;
            Player->Position.x -= penetration;
            break;
        }
    }
}

Collision Game::CheckCollision(PlayerObject& one, EnemyObject& two) // AABB - AABB collision
{
    // collision x-axis?
    // 8 pixel buffer on all the x's
    bool collisionX = one.Position.x + one.Size.x - 8 >= two.Position.x + 8 &&
        two.Position.x + two.Size.x - 8 >= one.Position.x + 8;
    // collision y-axis?
    // 16 pixel buffer on enemy top y and players top y
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y + 16 &&
        two.Position.y + two.Size.y >= one.Position.y + 16;
    // collision only if on both axes
    if (collisionX && collisionY) {
        // determine direction
        glm::vec2 oneToTwo = one.Position - two.Position;
        return Collision(true, VectorDirection(oneToTwo), oneToTwo);
    }
    else {
        return Collision(false, UP, glm::vec2(0.0f, 0.0f));
    }
}

Collision Game::CheckIfPlayerAttackHit(PlayerObject& one, EnemyObject& two) {
    // collision x-axis?
    // add 16 pixel range to player ones' size compared to checkCollision
    bool collisionX = one.Position.x + one.Size.x + 8 >= two.Position.x + 8 &&
        two.Position.x + two.Size.x - 8 >= one.Position.x - 8;
    // collision y-axis?
    // add another 16 pixel range to player ones' size compared to checkCollision
    bool collisionY = one.Position.y + one.Size.y + 16 >= two.Position.y + 16 &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    if (collisionX && collisionY) {
        // determine direction
        glm::vec2 oneToTwo = one.Position - two.Position;
        return Collision(true, VectorDirection(oneToTwo), oneToTwo);
    }
    else {
        return Collision(false, UP, glm::vec2(0.0f, 0.0f));
    }
}

//Collision Game::CheckIfEnemyAttackHit(PlayerObject& one, EnemyObject& two) { 
//        
//}

Direction Game::VectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // Up
        glm::vec2(-1.0f, 0.0f),  // Right
        glm::vec2(0.0f, -1.0f), // Down
        glm::vec2(1.0f, 0.0f)  // left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++) {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}