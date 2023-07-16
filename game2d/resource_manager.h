#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <map>
#include <sstream>
#include <string>

#include <glad/glad.h>

#include "g_shader.h"
#include "g_texture.h"

class ResourceManager {
public:
  static std::map<std::string, Shader::ptr> Shaders;
  static std::map<std::string, Texture2D::ptr> Textures;
  static Shader::ptr LoadShader(const char *vShaderFile,
                                const char *fShaderFile,
                                const char *gShaderFile, std::string name);
  static Shader::ptr GetShader(std::string name);
  static Texture2D::ptr LoadTexture(const char *file, bool alpha,
                                    std::string name);
  static Texture2D::ptr GetTexture(std::string name);
  static void Clear();

  static std::string Debug() {
    std::stringstream ss;
    ss << "Shaders:" << std::endl;
    for (auto iter : Shaders) {
      ss << iter.first << std::endl;
    }
    ss << "Texture:" << std::endl;
    for (auto iter : Textures) {
      ss << iter.first << std::endl;
    }
    return ss.str();
  }

private:
  ResourceManager() {}
  static Shader::ptr loadShaderFromFile(const char *vShaderFile,
                                        const char *fShaderFile,
                                        const char *gShaderFile = nullptr);
  static Texture2D::ptr loadTextureFromFile(const char *file, bool alpha);
};

#endif