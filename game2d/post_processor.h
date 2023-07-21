#ifndef _POST_PROCESSOR_H__
#define _POST_PROCESSOR_H__

#include <memory>

#include "g_shader.h"
#include "g_texture.h"

// PostProcessor hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textured quad after which one can
// enable specific effects by enabling either the Confuse, Chaos or
// Shake boolean.
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.
class PostProcessor {
public:
  typedef std::shared_ptr<PostProcessor> ptr;

  // State
  unsigned int Width, Height;
  // Options
  bool Confuse, Chaos, Shake;
  // Constructor
  PostProcessor(Shader::ptr shader, unsigned int width, unsigned int height);
  // Prepares the post processor's framebuffer operations before rendering the
  // game
  void BeginRender();
  // Should be called after rendering the game, so it stores all the rendered
  // data into a texture object
  void EndRender();
  // Renders the PostProcessor texture quad (as a screen-encompassing large
  // sprite)
  void Render(float time);

private:
  Shader::ptr m_postProcessingShader;
  Texture2D::ptr m_texture2D;
  // MSFBO = Multisampled FBO. FBO is regular, used for blitting MS color-buffer
  // to texture
  unsigned int m_MSFBO, m_FBO;
  // RBO is used for multisampled color buffer
  unsigned int m_RBO;
  unsigned int m_VAO;

  // Initialize quad for rendering postprocessing texture
  void initRenderData();
};

#endif