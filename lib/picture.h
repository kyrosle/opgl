#ifndef _PICTURE_H__
#define _PICTURE_H__

#include <cassert>
#include <iostream>
#include <memory>

#include <glad/glad.h>

#include "stb_image.h"

class Picture {
public:
  typedef std::shared_ptr<Picture> ptr;

  Picture(int width, int height, int nrChannels)
      : m_width(width), m_hight(height), m_nrChannels(nrChannels) {}
  ~Picture() {}

private:
  int m_width;
  int m_hight;
  int m_nrChannels;
};

class PictureLoader {
public:
  typedef std::shared_ptr<PictureLoader> ptr;

  // 加载图片文件
  static Picture::ptr loadAsset(const std::string &assetPath) {
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(assetPath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D /* 纹理目标 */,
                   0 /* 纹理指定多级渐远纹理的级别 */,
                   GL_RGBA,       // 告诉 opengl 纹理储存为何种格式
                   width, height, // 纹理宽度和高度
                   0, GL_RGBA /* 源图的格式 */, GL_UNSIGNED_BYTE /* 数据类型
                                                                  */
                   ,
                   data /* 真正图像数据 */);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture" << std::endl;
    }

    // 生成纹理和相应的多级渐远纹理， 释放图像内存
    stbi_image_free(data);

    return Picture::ptr(new Picture(width, height, nrChannels));
  }

  static void setFlipVerticalOnLoad(bool flip) {
    // 图像加载时帮助我们翻转y轴
    stbi_set_flip_vertically_on_load(true);
  }
};

#endif