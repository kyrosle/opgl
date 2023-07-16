// vecn 向量
// in 输入， out 输出
// Uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式, global
#include "shader.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <vector>

// 希望渲染的三角形
float vertices[] = {
    // 位置                             // 颜色
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
    0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // 顶部
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
      new Shader("asserts/shader/shader.vs", "asserts/shader/shader.fs");

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // 1. 绑定 VAO
  glBindVertexArray(VAO);
  // 2. 把顶点数组复制到缓冲区供 OpenGL 使用
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 3. 设置顶点属性指针
  // 位置属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // 颜色属性
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  // -> 4. 渲染一个物体时要使用着色器程序
  // glUseProgram(shaderProgram);
  // glBindVertexArray(VAO);
  // ...

  // ------ Render Loop ------
  // 每次循环开始前检测 GLFW 是否被要求退出
  while (!glfwWindowShouldClose(window)) {
    // 按下 ecs glfw 退出
    processInput(window);

    // glClear 清空屏幕颜色, 并填充 glClearColor 里面的颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 渲染指令
    // 随着时间改变颜色

    // 更新一个uniform之前你必须先使用程序（调用glUseProgram)，
    //  因为它是在当前激活的着色器程序中设置uniform的
    // glUseProgram(shaderProgram);
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

    // 查询 uniform outColor 位置
    // int vertexColorLocation = glGetUniformLocation(shaderProgram,
    // "outColor"); glUniform4f(vertexColorLocation, 0.0f, greenValue,
    // 0.0f, 1.0f);
    shader->setVec4("outColor", 0.0f, greenValue, blueValue, 1.0f);
    shader->setVec3("transPos", x_offset, y_offset, 0);

    // 绘制三角形
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES /* 绘制三角形 */, 0 /* 顶点数组的起始索引 */,
                 3 /* 打算绘制多少个顶点 */);

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
