#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "shader.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 希望渲染的三角形
float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 右上
    0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window = glfwCreateWindow(800, 600, "CSS", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Shader *shader =
      new Shader("asserts/texture/texture.vs", "asserts/texture/texture.fs");

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // 1. 绑定 VAO
  glBindVertexArray(VAO);
  // 2. 把顶点数组复制到缓冲区供 OpenGL 使用
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  // 3. 设置顶点属性指针
  // 位置属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // 颜色属性
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 额外顶点属性
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // ------ Texture -------
  // 纹理目标, 纹理轴 指定S,T轴, 环绕方式(Wrapping)
  // glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  // glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  // GL_CLAMP_TO_BORDER 要 指定边缘颜色
  // float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
  // glTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  // 纹理过滤
  // GL_NEAREST:纹理像素的中心距离纹理坐标最近，所以它会被选择为样本颜色(颗粒感)
  // GL_LINEAR:邻近像素的混合色(朦胧感)
  // Minify 缩小
  // glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // Magnify 放大
  // glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // 多级渐远纹理(Mipmap)
  // glGenerateTextureMipmap();
  // 过滤方式
  // a: NEAREST, b: LINEAR
  // GL_x_MIPMAP_x * 4

  // 生成纹理1
  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Minify 缩小
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // Magnify 放大
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // 加载与创建纹理
  int width, height, nrChannels;
  // 图像加载时帮助我们翻转y轴
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load("asserts/box.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D /* 纹理目标 */,
                 0 /* 纹理指定多级渐远纹理的级别 */,
                 GL_RGB,        // 告诉 opengl 纹理储存为何种格式
                 width, height, // 纹理宽度和高度
                 0, GL_RGB /* 源图的格式 */, GL_UNSIGNED_BYTE /* 数据类型 */,
                 data /* 真正图像数据 */);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  // 生成纹理和相应的多级渐远纹理， 释放图像内存
  stbi_image_free(data);

  // 生成纹理2
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Minify 缩小
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // Magnify 放大
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("asserts/awesome.png", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D /* 纹理目标 */,
                 0 /* 纹理指定多级渐远纹理的级别 */,
                 GL_RGBA,       // 告诉 opengl 纹理储存为何种格式
                 width, height, // 纹理宽度和高度
                 0, GL_RGBA /* 源图的格式 */, GL_UNSIGNED_BYTE /* 数据类型 */,
                 data /* 真正图像数据 */);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  // 生成纹理和相应的多级渐远纹理， 释放图像内存
  stbi_image_free(data);

  // 设置uniform变量之前激活着色器程序
  shader->useProgram();
  // 设置每个采样器的方式告诉OpenGL每个着色器采样器属于哪个纹理单元
  shader->setInt("texture1", 0);
  shader->setInt("texture2", 1);

  // ------ Render Loop ------
  // 每次循环开始前检测 GLFW 是否被要求退出
  while (!glfwWindowShouldClose(window)) {
    // 按下 ecs glfw 退出
    processInput(window);

    // glClear 清空屏幕颜色, 并填充 glClearColor 里面的颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 渲染指令

    glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元， 0 默认激活
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader->useProgram();

    float timeValue = glfwGetTime(); // 获得运行的秒数
    // g
    float greenValue =
        (std::sin(timeValue) / 2.0f) + 0.5f; // 颜色在 0.0 到 1.0 之间变化

    // b
    float blueValue =
        (std::cos(timeValue) / 2.0) + 0.5f; // 颜色在 0.0 到 1.0 之间变化

    // x offset
    float x_offset = (std::sin(timeValue) / 2.0f); // 颜色在 0.0 到 1.0 之间变化

    // y offset
    float y_offset = (std::cos(timeValue) / 2.0); // 颜色在 0.0 到 1.0 之间变化

    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, timeValue, glm::vec3(0.0f, 0.0f, 1.0f));

    unsigned int transformLoc = shader->getUniform("transform");
    glUniformMatrix4fv(transformLoc, 1 /* 多少可矩阵 */,
                       GL_FALSE /* 如何矩阵进行转置,交换矩阵的行和列 */,
                       glm::value_ptr(trans));

    shader->setVec4("uColor", 0.0f, greenValue, blueValue, 0.1f);
    shader->setVec3("transPos", x_offset, y_offset, 0);

    // 绘制三角形
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES /* 绘制三角形 */, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
