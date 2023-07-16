#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // 1. 从文件路径获取 顶点/片段 着色器
  std::string vertexCode;
  std::string fragmentCode;

  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // 保证 ifstream 对象可以抛出异常
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // 打开文件
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // 读取文件的缓冲内容到数据流
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // 关闭文件处理器
    vShaderFile.close();
    fShaderFile.close();
    // 转换数据流到string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  // 2. 编译着色器
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];
  // 顶点着色器
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // 片段着色器
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // 着色器程序
  m_id = glCreateProgram();
  glAttachShader(m_id, vertex);
  glAttachShader(m_id, fragment);
  glLinkProgram(m_id);
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_id, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  // 删除着色器
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::useProgram() { glUseProgram(m_id); }

unsigned int Shader::getUniform(const std::string &name) const {
  return glGetUniformLocation(m_id, name.c_str());
}

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(getUniform(name), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(getUniform(name), value);
}
void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(getUniform(name), value);
}
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(getUniform(name), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const {
  glUniform2f(getUniform(name), x, y);
}
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(getUniform(name), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(getUniform(name), x, y, z);
}
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(getUniform(name), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z,
                     float w) const {
  glUniform4f(getUniform(name), x, y, z, w);
}
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(getUniform(name), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &mat[0][0]);
}