#ifndef __GAME__H__
#define __GAME__H__

#include <tuple>
#include <vector>

#include "game_level.h"
#include "game_object.h"
#include "power_up.h"
#include <glad/glad.h>

// Current game state
enum class GameState {
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN,
};

enum class Direction { UP, RIGHT, DOWN, LEFT };

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Game {
public:
  // game state
  GameState State;
  bool Keys[1024];
  bool KeysProcessed[1024];
  unsigned int Width, Height;

  unsigned int Lives;

  std::vector<GameLevel::ptr> Levels;
  unsigned int Level;

  std::vector<PowerUp::ptr> PowerUps;

  Game(unsigned int width, unsigned int height);
  ~Game();

  void ResetLevel();
  void ResetPlayer();

  void SpawnPowerUps(GameObject::ptr block);
  void UpdatePowerUp(float dt);

  // initialize game state  (loading shader/texture/level)
  void Init();
  // do collision detection
  void DoCollisions();
  // game loop
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();
};

#endif