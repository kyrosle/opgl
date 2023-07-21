#ifndef _GAME_LEVEL_H__
#define _GAME_LEVEL_H__

#include "g_sprite_render.h"
#include "game_object.h"
#include <memory>
#include <vector>

class GameLevel {
public:
  typedef std::shared_ptr<GameLevel> ptr;

  std::vector<GameObject::ptr> Bricks;

  GameLevel() {}

  void Load(const char *file, unsigned int levelWidth,
            unsigned int levelHeight);

  void Draw(SpriteRenderer::ptr renderer);

  bool IsCompleted();

private:
  void init(std::vector<std::vector<unsigned int>> tileData,
            unsigned int levelWidth, unsigned int levelHeight);
};

#endif