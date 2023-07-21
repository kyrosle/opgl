#ifndef _POWER_UP_H__
#define _POWER_UP_H__

#include "g_texture.h"
#include "game_object.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject, std::enable_shared_from_this<PowerUp> {
public:
  typedef std::shared_ptr<PowerUp> ptr;

  std::string Type;
  float Duration;
  bool Activated;

  PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position,
          Texture2D::ptr texture)
      : GameObject(position, SIZE, texture, color, VELOCITY), Type(type),
        Duration(duration), Activated() {}
};

#endif