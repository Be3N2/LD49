#include <algorithm>

#include "game.h"

#include "sprite_renderer.h"
#include "resource_manager.h"
#include "post_processor.h"
#include "text_renderer.h"

#include <iostream>
#include <string>
#include <sstream>

#include <irrklang/irrKlang.h>

GameObject* Player;
SpriteRenderer* Renderer;
PostProcessor* Effects;
TextRenderer* Text;
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Player;
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
    ResourceManager::LoadTexture("res/gameArea.png", true, "background");
    //ResourceManager::LoadTexture("res/redDot.png", true, "player");
    Effects = new PostProcessor(ResourceManager::GetShader("post_processing"), this->Width, this->Height);

  /*  glm::vec2 playerPos = glm::vec2(
        this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
        this->Height - PLAYER_SIZE.y
    );

    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));*/

    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("OCRAEXT.TTF", 24);

    //SoundEngine->play2D("breakout.mp3", true);
}

void Game::Update(float dt)
{
    // check for collisions
    this->DoCollisions();

}

void Game::ResetLevel() {
    
}

void Game::ResetPlayer() {
    
}

void Game::ProcessInput(float dt)
{
    if (State == GAME_MENU) {
        if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER]) {
            State = GAME_ACTIVE;
            KeysProcessed[GLFW_KEY_ENTER] = true;
        }
        if (Keys[GLFW_KEY_W] && !KeysProcessed[GLFW_KEY_W]) {
            KeysProcessed[GLFW_KEY_W] = true;
        }
        if (Keys[GLFW_KEY_A] && !KeysProcessed[GLFW_KEY_A]) {
            KeysProcessed[GLFW_KEY_A] = true;
        }
        if (Keys[GLFW_KEY_S] && !KeysProcessed[GLFW_KEY_S]) {
            KeysProcessed[GLFW_KEY_S] = true;
        }
        if (Keys[GLFW_KEY_D] && !KeysProcessed[GLFW_KEY_D]) {
            KeysProcessed[GLFW_KEY_D] = true;
        }
    }

    //if (this->State == GAME_ACTIVE)
    //{
    //    float velocity = PLAYER_VELOCITY * dt;
    //    // move playeboard
    //    if (this->Keys[GLFW_KEY_A])
    //    {
    //        if (Player->Position.x >= 0.0f) {
    //            Player->Position.x -= velocity;
    //        }
    //    }
    //    if (this->Keys[GLFW_KEY_D])
    //    {
    //        if (Player->Position.x <= this->Width - Player->Size.x) {
    //            Player->Position.x += velocity;
    //        }
    //    }
    //}
    //if (State == GAME_WIN) {
    //    if (Keys[GLFW_KEY_ENTER]) {
    //        KeysProcessed[GLFW_KEY_ENTER] = true;
    //        State = GAME_MENU;
    //    }
    //}
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

        //Player->Draw(*Renderer);
        Effects->EndRender();
        //std::cout << "End Render GL Error: " << glGetError() << std::endl;
        Effects->Render(glfwGetTime());
        //std::cout << "Effects Render GL Error: " << glGetError() << std::endl;

        // draw text
        Text->RenderText("LD49", 300, 300, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
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
