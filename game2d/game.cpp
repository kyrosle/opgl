#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include <iostream>
#include <memory>

#include "g_shader.h"
#include "g_sprite_render.h"
#include "game.h"

#include "glm/ext/vector_float2.hpp"
#include "resource_manager.h"

SpriteRenderer::ptr Renderer;

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_ACTIVE), Width(width), Height(height) {}

Game::~Game() {}

void Game::Init() {
  ResourceManager::LoadShader("game2d/shaders/sprite.vs",
                              "game2d/shaders/sprite.fs", nullptr, "sprite");

  glm::mat4 projection =
      glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height),
                 0.0f, -1.0f, 1.0f);

  auto o = ResourceManager::GetShader("sprite");
  auto sprite_shader = o->Use();
  sprite_shader->SetInteger("image", 0);
  sprite_shader->SetMatrix4("projection", projection);

  Renderer.reset(new SpriteRenderer(ResourceManager::GetShader("sprite")));

  // ResourceManager::LoadTexture("game2d/textures/avatar.jpg", GL_TRUE,
  // "avatar");
  ResourceManager::LoadTexture("asserts/awesome.png", true, "face");
}

void Game::Update(float dt) {}

void Game::ProcessInput(float dt) {}

void Game::Render() {
  Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200),
                       glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  // Renderer->DrawSprite(ResourceManager::GetTexture("avatar"),
  //                      glm::vec2(200, 200), glm::vec2(300, 400), 45.0f,
  //                      glm::vec3(0.0f, 1.0f, 0.0f));
}