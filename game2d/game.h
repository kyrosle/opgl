#ifndef __GAME__H__
#define __GAME__H__

#include <glad/glad.h>

// 代表了游戏的当前状态
enum class GameState {
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN,
};

class Game {
public:
  // 游戏状态
  GameState State;
  bool Keys[1024];
  bool KeysProcessed[1024];
  unsigned int Width, Height;

  Game(unsigned int width, unsigned int height);
  ~Game();

  // 初始化 游戏状态 (加载所有着色器/纹理/关卡)
  void Init();
  // 游戏循环
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();
};

#endif