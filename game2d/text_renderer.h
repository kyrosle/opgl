#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

#include "g_shader.h"
#include "g_texture.h"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
  unsigned int TextureID; // ID handle of the glyph texture
  glm::ivec2 Size;        // Size of glyph
  glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
  unsigned int Advance;   // Horizontal offset to advance to next glyph
};

// A renderer class for rendering text displayed by a font loaded using the
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextRenderer {
public:
  typedef std::shared_ptr<TextRenderer> ptr;

  // Holds a list of pre-compiled Characters
  std::map<char, Character> Characters;
  // Shader used for text rendering
  Shader::ptr TextShader;
  // Constructor
  TextRenderer(unsigned int width, unsigned int height);
  // Pre-compiles a list of characters from the given font
  void Load(std::string font, unsigned int fontSize);
  // Renders a string of text using the precompiled list of characters
  void RenderText(const std::string &text, float x, float y, float scale,
                  glm::vec3 color = glm::vec3(1.0f));

private:
  // Render state
  unsigned int m_VAO, m_VBO;
};

#endif