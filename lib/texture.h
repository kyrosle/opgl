#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glad/glad.h>
#include <memory>
#include <string>
#include <vector>

#include "base.h"
#include "sigleton.h"

// 纹理轴
enum class TextureWrap {
  // S轴
  TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S,
  // T轴
  TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T
};

// 纹理环绕方式
enum class TextureWrapping {
  // 对纹理的默认行为,重复纹理图像
  REPEAT = GL_REPEAT,
  // 和GL_REPEAT一样,但每次重复图片是镜像反置的
  MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
  // 纹理坐标会被约束在0到1之间,超出的部分会重复纹理坐标的边缘,产生一种边缘被拉伸的效果
  CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
  // 超出的坐标为用户指定的边缘颜色
  CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
};

// 纹理过滤
// 一个常见的错误是,将放大过滤的选项设置为多级渐远纹理过滤选项之一.这样没有任何效果,
// 因为多级渐远纹理主要是使用在纹理被缩小的情况下的:纹理放大不会使用多级渐远纹理,
// 为放大过滤设置多级渐远纹理的选项会产生一个GL_INVALID_ENUM错误代码
enum class TextureFilter {
  // 选择中心点最接近纹理坐标的那个像素
  NEAREST = GL_NEAREST,
  // 基于纹理坐标附近的纹理像素,计算出一个插值,近似出这些纹理像素之间的颜色
  LINEAR = GL_LINEAR,
  // 使用最邻近的多级渐远纹理来匹配像素大小,并使用邻近插值进行纹理采样
  NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
  // 使用最邻近的多级渐远纹理级别,并使用线性插值进行采样
  LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
  // 在两个最匹配像素大小的多级渐远纹理之间进行线性插值,使用邻近插值进行采样
  NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
  // 在两个邻近的多级渐远纹理之间使用线性插值,并使用线性插值进行采样
  LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};

// 用来指示使用的纹理单元
struct TextureUnit {
  unsigned int pos = 0;
  static unsigned int getTextureUnit(int pos) { return GL_TEXTURE0 + pos; }
};

class Texture2D {
public:
  typedef std::shared_ptr<Texture2D> ptr;

  // texture 图片路径， 使用的 shader 着色器程序
  Texture2D(const std::string &textureAssetPath,
            const std::string &textureName);

  // 获取 opengl 分配的 id
  unsigned int getTextureID() { return m_id; }

  const std::string &getTextureName() const { return m_textureName; }

  // 设置 texture 环绕方式
  void setTextureWrap(TextureWrap type, TextureWrapping value);
  // 环绕方式为 GL_CLAMP_TO_BORDER 时候，要指定一个边缘的颜色
  void setTextureBorderColor(Color color);

  // texture 被缩小的时候使用的过滤方式
  void setTextureMinifyFiltering(TextureFilter filter);
  // texture 被放大的时候使用的过滤方式
  void setTextureMagnifyFiltering(TextureFilter filter);

  // 从图片 加载 texture
  bool Generate();

  void Clear() { glDeleteTextures(1, &m_id); }

private:
  unsigned int m_id;
  std::string m_textureName;
  std::string m_assetPath;
  unsigned int Width, Height;
};

class TextureAssets {
public:
  typedef std::shared_ptr<TextureAssets> ptr;

private:
  std::vector<Texture2D> m_textures;
};

#endif