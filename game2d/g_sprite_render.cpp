#include "g_sprite_render.h"
#include "g_shader.h"

SpriteRenderer::SpriteRenderer(Shader::ptr shader) : m_shader(shader) {
  initRenderData();
}

SpriteRenderer::~SpriteRenderer() { glDeleteVertexArrays(1, &m_quadVAO); }

void SpriteRenderer::DrawSprite(Texture2D::ptr texture, glm::vec2 position,
                                glm::vec2 size, float rotate, glm::vec3 color) {
  // 装备变换
  m_shader->Use();
  glm::mat4 model = glm::mat4(1.0f);
  // 变换的矩阵顺序是相反的：移动，旋转，缩放
  model = glm::translate(model, glm::vec3(position, 0.0f));

  // 移动
  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
  // 旋转
  model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
  model =
      glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  // 缩放
  model = glm::scale(model, glm::vec3(size, 1.0f));

  m_shader->SetMatrix4("model", model);

  // render textured quad
  m_shader->SetVector3f("spriteColor", color);

  glActiveTexture(GL_TEXTURE0);
  texture->Bind();

  glBindVertexArray(m_quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void SpriteRenderer::initRenderData() {
  // 元素的位置定义为元素左上角的位置
  // 配置 VAO/VBO
  unsigned int VBO;
  float vertices[] = {
      // 位置                // 纹理
      0.0f, 1.0f, 0.0f, 1.0f, //
      1.0f, 0.0f, 1.0f, 0.0f, //
      0.0f, 0.0f, 0.0f, 0.0f, //

      0.0f, 1.0f, 0.0f, 1.0f, //
      1.0f, 1.0f, 1.0f, 1.0f, //
      1.0f, 0.0f, 1.0f, 0.0f  //
  };

  glGenVertexArrays(1, &m_quadVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(m_quadVAO);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}