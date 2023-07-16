#include <iostream>

#include "stb_image.h"
#include "texture.h"

static int TEXTURE_INDEX = 0;

// transform the enum type into unsigned int value
#define va(x) (unsigned int)x

Texture2D::Texture2D(const std::string &textureAssetPath,
                     const std::string &textureName)
    : m_assetPath(textureAssetPath), m_textureName(textureName) {
  glGenTextures(1, &m_id);
  // glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::setTextureWrap(TextureWrap type, TextureWrapping value) {
  glTexParameteri(GL_TEXTURE_2D, va(type), va(value));
}

void Texture2D::setTextureBorderColor(Color color) {
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color.getValue());
}

void Texture2D::setTextureMinifyFiltering(TextureFilter filter) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, va(filter));
}
void Texture2D::setTextureMagnifyFiltering(TextureFilter filter) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, va(filter));
}

bool Texture2D::Generate() {
  int width, height, nrComponents;
  unsigned char *data =
      stbi_load(m_assetPath.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // -- optional--
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << m_assetPath << std::endl;
    stbi_image_free(data);
    return false;
  }
  return true;
}
