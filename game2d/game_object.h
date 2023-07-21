#ifndef _GAME_OBJECT_H__
#define _GAME_OBJECT_H__

#include "g_sprite_render.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <memory>

#include "g_texture.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject {
public:
  typedef std::shared_ptr<GameObject> ptr;

  // Object state
  glm::vec2 Position, Size, Velocity;
  glm::vec3 Color = glm::vec3(1.0f);

  float Rotation = 0;
  bool IsSolid = false;
  bool Destroyed = false;

  // Render state
  Texture2D::ptr Sprite;

  // Constructor(s)
  GameObject();
  GameObject(glm::vec2 pos, glm::vec2 size, Texture2D::ptr sprite,
             glm::vec3 color = glm::vec3(1.0f),
             glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

  // Draw sprite
  virtual void Draw(SpriteRenderer::ptr renderer);
};

#endif