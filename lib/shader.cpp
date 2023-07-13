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
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  // 删除着色器
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use() { glUseProgram(ID); }

unsigned int Shader::getUniform(const std::string &name) const {
  return glGetUniformLocation(ID, name.c_str());
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
void Shader::setVec3(const std::string &name, float value1, float value2,
                     float value3) const {
  glUniform3f(getUniform(name), value1, value2, value3);
}
void Shader::setVec4(const std::string &name, float value1, float value2,
                     float value3, float value4) const {
  glUniform4f(getUniform(name), value1, value2, value3, value4);
}

void Shader::setMatrix4fv(const std::string &name, glm::mat4 value) const {
  glUniformMatrix4fv(getUniform(name), 1 /* 多少可矩阵 */,
                     GL_FALSE /* 如何矩阵进行转置,交换矩阵的行和列 */,
                     glm::value_ptr(value));
}