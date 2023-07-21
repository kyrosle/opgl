#include "game_object.h"
#include "base.h"
#include "g_sprite_render.h"

GameObject::GameObject()
    : Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f),
      Sprite(), IsSolid(false), Destroyed(false) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D::ptr sprite,
                       glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Sprite(sprite), Color(color),
      Velocity(velocity), IsSolid(false), Destroyed(false) {}

void GameObject::Draw(SpriteRenderer::ptr renderer) {
  renderer->DrawSprite(Sprite, Position, Size, Rotation, Color);
}