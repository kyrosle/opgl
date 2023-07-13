#ifndef __SHADER_H__
#define __SHADER_H__
#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  // 程序ID
  unsigned int ID;

  // 构造器读取并构造着色器
  Shader(const char *vertexPath, const char *fragmentPath);

  // 使用/激活着色器程序
  void use();

  // uniform 工具函数
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec3(const std::string &name, float value1, float value2,
               float value3) const;
  void setVec4(const std::string &name, float value1, float value2,
               float value3, float value4) const;
};

#endif