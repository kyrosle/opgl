#include "post_processor.h"
#include "g_texture.h"
#include <memory>

PostProcessor::PostProcessor(Shader::ptr shader, GLuint width, GLuint height)
    : m_postProcessingShader(shader),
      m_texture2D(std::make_shared<Texture2D>(Texture2D())), Width(width),
      Height(height), Confuse(false), Chaos(false), Shake(false) {
  // Initialize renderbuffer/framebuffer object
  glGenFramebuffers(1, &m_MSFBO);
  glGenFramebuffers(1, &m_FBO);
  glGenRenderbuffers(1, &m_RBO);

  // Initialize renderbuffer storage with a multisampled color buffer (don't
  // need a depth/stencil buffer)
  glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
  glRenderbufferStorageMultisample(
      GL_RENDERBUFFER, 8, GL_RGB, width,
      height); // Allocate storage for render buffer object
  glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
      m_RBO); // Attach MS render buffer object to framebuffer
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO"
              << std::endl;

  // Also initialize the FBO/texture to blit multisampled color-buffer to; used
  // for shader operations (for postprocessing effects)
  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
  m_texture2D->Generate(width, height, NULL);
  glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture2D->ID,
      0); // Attach texture to framebuffer as its color attachment
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Initialize render data and uniforms
  initRenderData();
  m_postProcessingShader->SetInteger("scene", 0, true);
  float offset = 1.0f / 300.0f;
  float offsets[9][2] = {
      {-offset, offset},  // top-left
      {0.0f, offset},     // top-center
      {offset, offset},   // top-right
      {-offset, 0.0f},    // center-left
      {0.0f, 0.0f},       // center-center
      {offset, 0.0f},     // center - right
      {-offset, -offset}, // bottom-left
      {0.0f, -offset},    // bottom-center
      {offset, -offset}   // bottom-right
  };
  glUniform2fv(glGetUniformLocation(m_postProcessingShader->ID, "offsets"), 9,
               (float *)offsets);
  int edge_kernel[9] = {
      -1, -1, -1, //
      -1, 8,  -1, //
      -1, -1, -1  //
  };
  glUniform1iv(glGetUniformLocation(m_postProcessingShader->ID, "edge_kernel"),
               9, edge_kernel);
  float blur_kernel[9] = {
      1.0 / 16, 2.0 / 16, 1.0 / 16, //
      2.0 / 16, 4.0 / 16, 2.0 / 16, //
      1.0 / 16, 2.0 / 16, 1.0 / 16, //
  };
  glUniform1fv(glGetUniformLocation(m_postProcessingShader->ID, "blur_kernel"),
               9, blur_kernel);
}

void PostProcessor::BeginRender() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
void PostProcessor::EndRender() {
  // Now resolve multisampled color-buffer into intermediate FBO to store to
  // texture
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSFBO);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
  glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glBindFramebuffer(
      GL_FRAMEBUFFER,
      0); // Binds both READ and WRITE framebuffer to default framebuffer
}

void PostProcessor::Render(float time) {
  // Set uniforms/options
  m_postProcessingShader->Use();
  m_postProcessingShader->SetFloat("time", time);
  m_postProcessingShader->SetInteger("confuse", Confuse);
  m_postProcessingShader->SetInteger("chaos", Chaos);
  m_postProcessingShader->SetInteger("shake", Shake);
  // Render textured quad
  glActiveTexture(GL_TEXTURE0);
  m_texture2D->Bind();
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void PostProcessor::initRenderData() {
  // Configure VAO/VBO
  GLuint VBO;
  float vertices[] = {
      // Pos        // Tex
      -1.0f, -1.0f, 0.0f, 0.0f, //
      1.0f,  1.0f,  1.0f, 1.0f, //
      -1.0f, 1.0f,  0.0f, 1.0f, //

      -1.0f, -1.0f, 0.0f, 0.0f, //
      1.0f,  -1.0f, 1.0f, 0.0f, //
      1.0f,  1.0f,  1.0f, 1.0f  //
  };
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(m_VAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}