#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <vector>

#include "ball_object.h"
#include "g_paticle.h"
#include "g_shader.h"
#include "g_sprite_render.h"
#include "game.h"
#include "game_level.h"
#include "game_object.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "post_processor.h"
#include "power_up.h"
#include "resource_manager.h"
#include "text_renderer.h"

SpriteRenderer::ptr Renderer;
TextRenderer::ptr Text;
PostProcessor::ptr Effects;

GameObject::ptr Player;
const glm::vec2 PLAYER_SIZE(100, 20);
const float PLAYER_VELOCITY(500.0f);

BallObject::ptr Ball;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

ParticleGenerator::ptr Particles;

// AABB - circle collision
Collision CheckCollision(BallObject::ptr one, GameObject::ptr two);
// AABB - AABB collision
bool CheckCollision(GameObject::ptr one, GameObject::ptr two);
Direction VectorDirection(glm::vec2 target);
void ActivatePowerUp(PowerUp::ptr powerUp);
bool IsOtherPowerUpActive(std::vector<PowerUp::ptr> &powerUps,
                          std::string type);

float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GameState::GAME_ACTIVE), Width(width), Height(height) {}

Game::~Game() {
  Renderer.reset();
  Player.reset();
  Ball.reset();
  Particles.reset();
  Effects.reset();
}

void Game::Init() {
  // Text loading
  Text = std::make_shared<TextRenderer>(TextRenderer(Width, Height));
  Text->Load("game2d/assets/OCRAEXT.TTF", 24);

  // Shader loading
  ResourceManager::LoadShader("game2d/shaders/sprite.vs",
                              "game2d/shaders/sprite.fs", nullptr, "sprite");
  glm::mat4 projection =
      glm::ortho(0.0f, static_cast<GLfloat>(Width),
                 static_cast<GLfloat>(Height), 0.0f, -1.0f, 1.0f);
  Shader::ptr sprite_shader = ResourceManager::GetShader("sprite")->Use();
  sprite_shader->SetInteger("image", 0);
  sprite_shader->SetMatrix4("projection", projection);

  ResourceManager::LoadShader("game2d/shaders/particle.vs",
                              "game2d/shaders/particle.fs", nullptr,
                              "particle");
  Shader::ptr particle_shader = ResourceManager::GetShader("particle")->Use();
  particle_shader->SetMatrix4("projection", projection);
  particle_shader->SetInteger("sprite", 0);

  ResourceManager::LoadShader("game2d/shaders/post_processing.vs",
                              "game2d/shaders/post_processing.fs", nullptr,
                              "postprocessing");

  // Texture loading
  ResourceManager::LoadTexture("game2d/textures/awesome.png", true, "face");
  ResourceManager::LoadTexture("game2d/textures/background.jpg", false,
                               "background");
  ResourceManager::LoadTexture("game2d/textures/avatar.jpg", false, "avatar");
  ResourceManager::LoadTexture("game2d/textures/paddle.png", true, "paddle");
  ResourceManager::LoadTexture("game2d/textures/block.png", false, "block");
  ResourceManager::LoadTexture("game2d/textures/block_solid.png", false,
                               "block_solid");
  ResourceManager::LoadTexture("game2d/textures/particle.png", true,
                               "particle");
  ResourceManager::LoadTexture("game2d/textures/powerup_speed.png", true,
                               "powerup_speed");
  ResourceManager::LoadTexture("game2d/textures/powerup_sticky.png", true,
                               "powerup_sticky");
  ResourceManager::LoadTexture("game2d/textures/powerup_increase.png", true,
                               "powerup_increase");
  ResourceManager::LoadTexture("game2d/textures/powerup_confuse.png", true,
                               "powerup_confuse");
  ResourceManager::LoadTexture("game2d/textures/powerup_chaos.png", true,
                               "powerup_chaos");
  ResourceManager::LoadTexture("game2d/textures/powerup_passthrough.png", true,
                               "powerup_passthrough");

  // Sprite Renderer
  Renderer = std::make_shared<SpriteRenderer>(
      SpriteRenderer(ResourceManager::GetShader("sprite")));

  // Game level loading
  GameLevel::ptr one(new GameLevel);
  one->Load("game2d/levels/1.lvl", Width, Height * 0.5);
  GameLevel::ptr two(new GameLevel);
  two->Load("game2d/levels/2.lvl", Width, Height * 0.5);
  GameLevel::ptr three(new GameLevel);
  three->Load("game2d/levels/3.lvl", Width, Height * 0.5);
  GameLevel::ptr four(new GameLevel);
  four->Load("game2d/levels/4.lvl", Width, Height * 0.5);
  Levels.push_back(one);
  Levels.push_back(two);
  Levels.push_back(three);
  Levels.push_back(four);

  Level = 0;

  // Player logic
  glm::vec2 playerPos =
      glm::vec2(static_cast<float>(Width) / 2 - PLAYER_SIZE.x / 2,
                static_cast<float>(Height) - PLAYER_SIZE.y);

  Player = std::make_shared<GameObject>(GameObject(
      playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle")));

  Lives = 3;

  // Ball logic
  glm::vec2 ballPos =
      playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
  Ball = std::make_shared<BallObject>(
      BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
                 ResourceManager::GetTexture("face")));

  // Particle logic
  Particles = std::make_shared<ParticleGenerator>(
      ParticleGenerator(ResourceManager::GetShader("particle"),
                        ResourceManager::GetTexture("particle"), 500));

  // Post processing effect
  Effects = std::make_shared<PostProcessor>(PostProcessor(
      ResourceManager::GetShader("postprocessing"), Width, Height));
}

void Game::Update(float dt) {
  // update object
  Ball->Move(dt, Width);
  // update particles
  Particles->Update(dt, Ball, 2, glm::vec2(Ball->Radius / 2));
  // check collision
  DoCollisions();
  // update PowerUps
  UpdatePowerUp(dt);
  // reduce shake time
  if (ShakeTime > 0.0f) {
    ShakeTime -= dt;
    if (ShakeTime <= 0.0f) {
      Effects->Shake = false;
    }
  }

  if (Ball->Position.y >= Height) {
    --Lives;

    if (Lives == 0) {
      ResetLevel();
      State = GameState::GAME_MENU;
    }
    ResetPlayer();
  }

  if (State == GameState::GAME_ACTIVE && Levels[Level]->IsCompleted()) {
    ResetLevel();
    ResetPlayer();
    Effects->Chaos = GL_TRUE;
    State = GameState::GAME_WIN;
  }
}

void Game::ProcessInput(float dt) {
  if (State == GameState::GAME_ACTIVE) {
    float velocity = PLAYER_VELOCITY * dt;
    // move paddle
    if (Keys[GLFW_KEY_A]) {
      if (Player->Position.x >= 0) {
        Player->Position.x -= velocity;
        if (Ball->Stuck) {
          Ball->Position.x =
              Player->Position.x + Player->Size.x / 2 - Ball->Size.x / 2;
        }
      }
    }
    if (Keys[GLFW_KEY_D]) {
      if (Player->Position.x <= Width - Player->Size.x) {
        Player->Position.x += velocity;
        if (Ball->Stuck) {
          Ball->Position.x =
              Player->Position.x + Player->Size.x / 2 - Ball->Size.x / 2;
        }
      }
    }

    if (Keys[GLFW_KEY_SPACE]) {
      Ball->Stuck = false;
    }
  }

  if (State == GameState::GAME_MENU) {
    if (Keys[GLFW_KEY_ENTER] && !KeysProcessed[GLFW_KEY_ENTER]) {
      State = GameState::GAME_ACTIVE;
      KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
    }
    if (Keys[GLFW_KEY_W] && !KeysProcessed[GLFW_KEY_W]) {
      Level = (Level + 1) % 4;
      KeysProcessed[GLFW_KEY_W] = GL_TRUE;
    }
    if (Keys[GLFW_KEY_S] && !KeysProcessed[GLFW_KEY_S]) {
      if (Level > 0)
        --Level;
      else
        Level = 3;
      KeysProcessed[GLFW_KEY_S] = GL_TRUE;
    }
  }

  if (State == GameState::GAME_WIN) {
    if (Keys[GLFW_KEY_ENTER]) {
      KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
      Effects->Chaos = GL_FALSE;
      State = GameState::GAME_MENU;
    }
  }
}

void Game::Render() {
  if (State == GameState::GAME_ACTIVE || State == GameState::GAME_MENU) {

    std::stringstream ss;
    ss << Lives;
    Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);

    Effects->BeginRender();

    // Draw background
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
                         glm::vec2(0, 0), glm::vec2(Width, Height), 0.0f);
    // Draw level
    Levels[Level]->Draw(Renderer);
    // Draw player
    Player->Draw(Renderer);
    // Draw particles
    Particles->Draw();
    // Draw ball
    Ball->Draw(Renderer);

    for (PowerUp::ptr &powerUp : PowerUps) {
      if (!powerUp->Destroyed) {
        powerUp->Draw(Renderer);
      }
    }

    Effects->EndRender();
    Effects->Render(glfwGetTime());
  }
  if (State == GameState::GAME_MENU) {
    Text->RenderText("Press ENTER to start", 250.0f,
                     static_cast<float>(Height) / 2, 1.0f);
    Text->RenderText("Press W or S to select level", 245.0f,
                     static_cast<float>(Height) / 2 + 20.0f, 0.75f);
  }
  if (State == GameState::GAME_WIN) {
    Text->RenderText("You WON!!!", 320.0, static_cast<float>(Height) / 2 - 20.0,
                     1.0, glm::vec3(0.0, 1.0, 0.0));
    Text->RenderText("Press ENTER to retry or ESC to quit", 130.0,
                     static_cast<float>(Height) / 2, 1.0,
                     glm::vec3(1.0, 1.0, 0.0));
  }
}

void Game::DoCollisions() {
  for (auto &box : Levels[Level]->Bricks) {
    if (!box->Destroyed) {
      Collision collision = CheckCollision(Ball, box);
      if (std::get<0>(collision)) {
        if (!box->IsSolid) {
          box->Destroyed = true;
          SpawnPowerUps(box);
        }
        ShakeTime = 0.05f;
        Effects->Shake = true;

        Direction dir = std::get<1>(collision);
        glm::vec2 diff_vector = std::get<2>(collision);
        if (!(Ball->PassThrough && !box->IsSolid)) {
          if (dir == Direction::LEFT || dir == Direction::RIGHT) {
            // Horizontal collision
            Ball->Velocity.x = -Ball->Velocity.x;
            float penetration = Ball->Radius - std::abs(diff_vector.x);
            if (dir == Direction::LEFT) {
              Ball->Position.x += penetration;
            } else {
              Ball->Position.x -= penetration;
            }
          } else {
            // Vertical collision
            Ball->Velocity.y = -Ball->Velocity.y;
            float penetration = Ball->Radius - std::abs(diff_vector.y);
            if (dir == Direction::UP) {
              Ball->Position.y -= penetration;
            } else {
              Ball->Position.y += penetration;
            }
          }
        }
      }
    }
  }

  Collision result = CheckCollision(Ball, Player);
  if (!Ball->Stuck && std::get<0>(result)) {
    float centerBoard = Player->Position.x + Player->Size.x / 2;
    float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
    float percentage = distance / (Player->Size.x / 2);
    float strength = 2.0f;
    glm::vec2 oldVelocity = Ball->Velocity;
    Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
    Ball->Velocity.y = -1 * std::abs(Ball->Velocity.y);
    Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    Ball->Stuck = Ball->Sticky;
  }

  for (PowerUp::ptr &powerUp : PowerUps) {
    if (!powerUp->Destroyed) {
      if (powerUp->Position.y >= Height) {
        powerUp->Destroyed = true;
      }
      if (CheckCollision(Player, powerUp)) {
        ActivatePowerUp(powerUp);
        powerUp->Destroyed = true;
        powerUp->Activated = true;
      }
    }
  }
}

void Game::ResetLevel() {
  switch (Level) {
  case 0:
    Levels[0]->Load("game2d/levels/1.lvl", Width, Height * 0.5f);
    break;
  case 1:
    Levels[1]->Load("game2d/levels/2.lvl", Width, Height * 0.5f);
    break;
  case 2:
    Levels[2]->Load("game2d/levels/3.lvl", Width, Height * 0.5f);
    break;
  case 3:
    Levels[3]->Load("game2d/levels/4.lvl", Width, Height * 0.5f);
    break;
  }

  Lives = 3;
}

void Game::ResetPlayer() {
  // Reset player/ball stats
  Player->Size = PLAYER_SIZE;
  Player->Position =
      glm::vec2(static_cast<float>(Width) / 2 - PLAYER_SIZE.x / 2,
                static_cast<float>(Height) - PLAYER_SIZE.y);
  Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS,
                                           -(BALL_RADIUS * 2)),
              INITIAL_BALL_VELOCITY);
  // disable all active powerUps
  Effects->Chaos = Effects->Confuse = false;
  Ball->PassThrough = false;
  Player->Color = glm::vec3(1.0f);
  Ball->Color = glm::vec3(1.0f);

  PowerUps.clear();
}

bool ShouldSpawn(unsigned int chance) {
  unsigned int random = rand() % chance;
  return random == 0;
}

void Game::SpawnPowerUps(GameObject::ptr block) {
  // 1 in 75 chance
  if (ShouldSpawn(75)) {
    PowerUps.push_back(std::make_shared<PowerUp>(
        PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block->Position,
                ResourceManager::GetTexture("powerup_speed"))));
  }
  if (ShouldSpawn(75)) {
    PowerUps.push_back(std::make_shared<PowerUp>(
        PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block->Position,
                ResourceManager::GetTexture("powerup_sticky"))));
  }
  if (ShouldSpawn(75)) {
    PowerUps.push_back(std::make_shared<PowerUp>(PowerUp(
        "pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block->Position,
        ResourceManager::GetTexture("powerup_passthrough"))));
  }
  if (ShouldSpawn(75)) {
    PowerUps.push_back(std::make_shared<PowerUp>(PowerUp(
        "pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block->Position,
        ResourceManager::GetTexture("powerup_increase"))));
  }
  // Negative powerups should spawn more often
  if (ShouldSpawn(15)) {
    PowerUps.push_back(std::make_shared<PowerUp>(
        PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block->Position,
                ResourceManager::GetTexture("powerup_confuse"))));
  }
  if (ShouldSpawn(15)) {
    PowerUps.push_back(std::make_shared<PowerUp>(
        PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block->Position,
                ResourceManager::GetTexture("powerup_chaos"))));
  }
}

void Game::UpdatePowerUp(float dt) {
  for (PowerUp::ptr &powerUp : PowerUps) {
    powerUp->Position += powerUp->Velocity * dt;
    if (powerUp->Activated) {
      powerUp->Duration -= dt;

      if (powerUp->Duration <= 0.0f) {
        powerUp->Activated = false;
        if (powerUp->Type == "sticky") {
          if (!IsOtherPowerUpActive(PowerUps, "sticky")) {
            Ball->Sticky = false;
            Player->Color = glm::vec3(1.0f);
          }
        } else if (powerUp->Type == "pass-through") {
          if (!IsOtherPowerUpActive(PowerUps, "pass-through")) {
            Ball->PassThrough = false;
            Ball->Color = glm::vec3(1.0f);
          }
        } else if (powerUp->Type == "confuse") {
          if (!IsOtherPowerUpActive(PowerUps, "confuse")) {
            Effects->Confuse = false;
          }
        } else if (powerUp->Type == "chaos") {
          Effects->Chaos = false;
        }
      }
    }
  }
  // Remove all PowerUps from vector that are destroyed AND !activated (thus
  // either off the map or finished) Note we use a lambda expression to remove
  // each PowerUp which is destroyed and not activated
  auto check = [](const PowerUp::ptr &powerUp) {
    return powerUp->Destroyed && !powerUp->Activated;
  };
  auto newEnd = std::remove_if(PowerUps.begin(), PowerUps.end(), check);
  PowerUps.erase(newEnd, PowerUps.end());
}

// AABB - AABB collision
bool CheckCollision(GameObject::ptr one, GameObject::ptr two) {
  bool collisionX = one->Position.x + one->Size.x >= two->Position.x &&
                    two->Position.x + two->Size.x >= one->Position.x;
  bool collisionY = one->Position.y + one->Size.y >= two->Position.y &&
                    two->Position.y + two->Size.y >= one->Position.y;
  return collisionX && collisionY;
}
// AABB - Circle collision
Collision CheckCollision(BallObject::ptr one, GameObject::ptr two) {
  // center of circle
  glm::vec2 center(one->Position + one->Radius);
  // AABB information(center, half extents)
  glm::vec2 aabb_half_extents(two->Size.x / 2, two->Size.y / 2);
  glm::vec2 aabb_center(two->Position.x + aabb_half_extents.x,
                        two->Position.y + aabb_half_extents.y);
  // Get the difference vector of the two centers
  glm::vec2 difference = center - aabb_center;
  glm::vec2 clamped =
      glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

  glm::vec2 closest = aabb_center + clamped;
  difference = closest - center;
  if (glm::length(difference) < one->Radius) {
    return std::make_tuple(true, VectorDirection(difference), difference);
  } else {
    return std::make_tuple(false, Direction::UP, glm::vec2(0, 0));
  }
}

Direction VectorDirection(glm::vec2 target) {
  const glm::vec2 compass[] = {
      glm::vec2(0.0f, 1.0f),  // UP
      glm::vec2(1.0f, 0.0f),  // RIGHT
      glm::vec2(0.0f, -1.0f), // DOWN
      glm::vec2(-1.0f, 0.0f)  // LEFT
  };
  float max = 0.0f;
  unsigned int best_match = -1;
  for (unsigned int i = 0; i < 4; ++i) {
    float dot_product = glm::dot(glm::normalize(target), compass[i]);
    if (dot_product > max) {
      max = dot_product;
      best_match = i;
    }
  }
  return (Direction)best_match;
}

void ActivatePowerUp(PowerUp::ptr powerUp) {
  if (powerUp->Type == "speed") {
    Ball->Velocity *= 1.2;
  } else if (powerUp->Type == "sticky") {
    Ball->Sticky = true;
    Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
  } else if (powerUp->Type == "pass-through") {
    Ball->PassThrough = true;
    Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
  } else if (powerUp->Type == "pad-size-increase") {
    Player->Size.x += 50;
  } else if (powerUp->Type == "confuse") {
    if (!Effects->Chaos) {
      Effects->Confuse = true;
    }
  } else if (powerUp->Type == "chaos") {
    if (!Effects->Confuse) {
      Effects->Chaos = true;
    }
  }
}

bool IsOtherPowerUpActive(std::vector<PowerUp::ptr> &powerUps,
                          std::string type) {
  for (const PowerUp::ptr &powerUp : powerUps) {
    if (powerUp->Activated && powerUp->Type == type) {
      return true;
    }
  }
  return false;
}