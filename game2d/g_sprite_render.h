#ifndef _G_SPRITE_RENDER_H__
#define _G_SPRITE_RENDER_H__

#include <iostream>
#include <memory>

#include "g_shader.h"
#include "g_texture.h"

class SpriteRenderer {
public:
  typedef std::shared_ptr<SpriteRenderer> ptr;

  // Constructor (inits shaders/shapes)
  SpriteRenderer(Shader::ptr shader);
  // Destructor
  ~SpriteRenderer();
  // Renders a defined quad textured with given sprite
  void DrawSprite(Texture2D::ptr texture, glm::vec2 position,
                  glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
                  glm::vec3 color = glm::vec3(1.0f));

  void Debug() { std::cout << "SpriteRenderer debug" << std::endl; }

private:
  // Render state
  Shader::ptr m_shader;
  unsigned int m_quadVAO;
  // Initializes and configures the quad's buffer and vertex attributes
  void initRenderData();
};

#endif