#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "picture.h"
#include "shader.h"
#include "stb_image.h"
#include "texture.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 希望渲染的三角形
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, //
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //

    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //
    -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, //

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, //
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, //
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, //
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, //
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f  //
};

const glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),     //
    glm::vec3(2.0f, 5.0f, -15.0f),   //
    glm::vec3(-1.5f, -2.2f, -2.5f),  //
    glm::vec3(-3.8f, -2.0f, -12.3f), //
    glm::vec3(2.4f, -0.4f, -3.5f),   //
    glm::vec3(-1.7f, 3.0f, -7.5f),   //
    glm::vec3(1.3f, -2.0f, -2.5f),   //
    glm::vec3(1.5f, 2.0f, -2.5f),    //
    glm::vec3(1.5f, 0.2f, -1.5f),    //
    glm::vec3(-1.3f, 1.0f, -1.5f)    //
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
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSS", NULL, NULL);
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

  // 启用深度测试
  glEnable(GL_DEPTH_TEST);

  Shader::ptr shader(new Shader("asserts/3d/3d.vs", "asserts/3d/3d.fs"));

  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // 1. 绑定 VAO
  glBindVertexArray(VAO);
  // 2. 把顶点数组复制到缓冲区供 OpenGL 使用
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 3. 设置顶点属性指针
  // 位置属性
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // 额外顶点属性
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  Texture2D::ptr texture1(new Texture2D("asserts/box.jpg", "texture1"));

  texture1->Generate();

  Texture2D::ptr texture2(new Texture2D("asserts/awesome.png", "texture2"));

  texture2->Generate();

  shader->addTexture(texture1);
  shader->addTexture(texture2);

  shader->setUpTextures();

  // ------ Render Loop ------
  // 每次循环开始前检测 GLFW 是否被要求退出
  while (!glfwWindowShouldClose(window)) {
    // 按下 ecs glfw 退出
    processInput(window);

    // glClear 清空屏幕颜色, 并填充 glClearColor 里面的颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 渲染指令

    shader->activeTexture();
    shader->useProgram();

    float timeValue = glfwGetTime(); // 获得运行的秒数

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection =
        glm::perspective(glm::radians(45.0f),
                         float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    glBindVertexArray(VAO);
    // a strange boxs
    // for (unsigned int i = 0; i < 10; ++i) {
    //   glm::mat4 model = glm::mat4(1.0f);
    //   model = glm::rotate(model, timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
    //   float angle = 20.0f * i;
    //   model =
    //       glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f,
    //       0.5f));
    //   shader->setMatrix4fv("model", model);

    //   glDrawArrays(GL_TRIANGLES, 0, 36);
    // }

    for (unsigned int i = 0; i < 10; ++i) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, timeValue, glm::vec3(0.5f, 1.0f, 0.0f));
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader->setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

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
