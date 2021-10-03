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
    Enemy = new EnemyObject(glm::vec2(180.0f, 80.0f), glm::vec2(32.0f, 32.0f), ResourceManager::GetTexture("spriteSheet"));

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("OCRAEXT.TTF", 24);

    //SoundEngine->play2D("breakout.mp3", true);
}

void Game::Update(float dt)
{
    // check for collisions
    this->DoCollisions();
    Player->Update(glfwGetTime());
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
        if (MouseLeft && !Player->onTheGround) {
            Player->attacking = true;
            Player->missed = true;
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

        Player->Draw(*Renderer, glfwGetTime());
        Enemy->Draw(*Renderer, glfwGetTime());
        
        Effects->EndRender();
        //std::cout << "End Render GL Error: " << glGetError() << std::endl;
        Effects->Render(glfwGetTime());
        //std::cout << "Effects Render GL Error: " << glGetError() << std::endl;

        // draw text
        if (Player->onTheGround) {
            Text->RenderText("Your On the Ground", Width / 2 - 70, 20, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
            Text->RenderText("Rapidly tap Q to get back up!", Width / 2 - 90, 40, 0.4f, glm::vec3(1.0f, 0.0f, 0.0f));
        }
    }
}

void Game::DoCollisions()
{
    // check player doesn't go out of bounds
    // check player against enemy
}

bool Game::CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}
