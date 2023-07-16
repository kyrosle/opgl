#ifndef __SHADER_H__
#define __SHADER_H__
#include "glm/ext/matrix_float4x4.hpp"
#include "texture.h"
#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Shader {
public:
  typedef std::shared_ptr<Shader> ptr;
  // 构造器读取并构造着色器
  Shader(const char *vertexPath, const char *fragmentPath);

  // 使用/激活着色器程序
  void useProgram();

  unsigned int getID() const { return m_id; }

  // uniform 工具函数
  unsigned int getUniform(const std::string &name) const;
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, float x, float y) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z,
               float w) const;

  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;

  // 添加 texture
  void addTexture(Texture2D::ptr texture) { m_textures.push_back(texture); }

  // 把 texture 添加到 opengl 中
  void setUpTextures() {
    // 设置uniform变量之前激活着色器程序
    useProgram();
    for (int i = 0; i < m_textures.size(); ++i) {
      std::cout << i << "-th Texture name=" << m_textures[i]->getTextureName()
                << std::endl;
      // 设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元
      setInt(m_textures[i]->getTextureName().c_str(), i);
    }
  }
  void activeTexture() {
    for (int i = 0; i < m_textures.size(); ++i) {
      // 在绑定纹理之前先激活纹理单元， 0 默认激活
      glActiveTexture(TextureUnit::getTextureUnit(i));
      glBindTexture(GL_TEXTURE_2D, m_textures[i]->getTextureID());
    }
  }

  void Clear() { glDeleteProgram(m_id); }

private:
  // 程序ID
  unsigned int m_id;
  // texture 容器
  std::vector<Texture2D::ptr> m_textures;
};

#endif