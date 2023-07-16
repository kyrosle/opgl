#ifndef _BASE_H__
#define _BASE_H__

#include <glad/glad.h>
#include <memory>

class Color {
public:
  typedef std::shared_ptr<Color> ptr;

  Color(float r, float g, float b, float alpha) {
    m_value[0] = r;
    m_value[1] = g;
    m_value[2] = b;
    m_value[3] = alpha;
  }
  float *getValue() { return &m_value[0]; }

private:
  float m_value[4];
};

#endif