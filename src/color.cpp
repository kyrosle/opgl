#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "picture.h"
#include "shader.h"
#include "stb_image.h"
#include "texture.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
    -0.5f, -0.5f, -0.5f, //
    0.5f,  -0.5f, -0.5f, //
    0.5f,  0.5f,  -0.5f, //
    0.5f,  0.5f,  -0.5f, //
    -0.5f, 0.5f,  -0.5f, //
    -0.5f, -0.5f, -0.5f, //

    -0.5f, -0.5f, 0.5f, //
    0.5f,  -0.5f, 0.5f, //
    0.5f,  0.5f,  0.5f, //
    0.5f,  0.5f,  0.5f, //
    -0.5f, 0.5f,  0.5f, //
    -0.5f, -0.5f, 0.5f, //

    -0.5f, 0.5f,  0.5f,  //
    -0.5f, 0.5f,  -0.5f, //
    -0.5f, -0.5f, -0.5f, //
    -0.5f, -0.5f, -0.5f, //
    -0.5f, -0.5f, 0.5f,  //
    -0.5f, 0.5f,  0.5f,  //

    0.5f,  0.5f,  0.5f,  //
    0.5f,  0.5f,  -0.5f, //
    0.5f,  -0.5f, -0.5f, //
    0.5f,  -0.5f, -0.5f, //
    0.5f,  -0.5f, 0.5f,  //
    0.5f,  0.5f,  0.5f,  //

    -0.5f, -0.5f, -0.5f, //
    0.5f,  -0.5f, -0.5f, //
    0.5f,  -0.5f, 0.5f,  //
    0.5f,  -0.5f, 0.5f,  //
    -0.5f, -0.5f, 0.5f,  //
    -0.5f, -0.5f, -0.5f, //

    -0.5f, 0.5f,  -0.5f, //
    0.5f,  0.5f,  -0.5f, //
    0.5f,  0.5f,  0.5f,  //
    0.5f,  0.5f,  0.5f,  //
    -0.5f, 0.5f,  0.5f,  //
    -0.5f, 0.5f,  -0.5f, //
};

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);

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
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // 启用深度测试
  glEnable(GL_DEPTH_TEST);

  Shader::ptr lightingShader(
      new Shader("asserts/color/color.vs", "asserts/color/color.fs"));
  Shader::ptr lightCubeShader(
      new Shader("asserts/color/light_cube.vs", "asserts/color/light_cube.fs"));

  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // 灯光源 cube
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // ------ Render Loop ------
  // 每次循环开始前检测 GLFW 是否被要求退出
  while (!glfwWindowShouldClose(window)) {
    // 每帧时间
    float currentFrame = static_cast<float>(glfwGetTime()); // 获得运行的秒数
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // 按下 ecs glfw 退出
    processInput(window);

    // glClear 清空屏幕颜色, 并填充 glClearColor 里面的颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 渲染指令

    lightingShader->useProgram();
    lightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    // view/projection transformations
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader->setMat4("projection", projection);
    lightingShader->setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader->setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // also draw the lamp object
    lightCubeShader->useProgram();
    lightCubeShader->setMat4("projection", projection);
    lightCubeShader->setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightCubeShader->setMat4("model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightCubeVAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessCameraMovement(CameraMovement::FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessCameraMovement(CameraMovement::BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessCameraMovement(CameraMovement::LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessCameraMovement(CameraMovement::RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xPosIn, double yPosIn) {
  float xPos = static_cast<float>(xPosIn);
  float yPos = static_cast<float>(yPosIn);

  if (firstMouse) {
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }

  float xOffset = xPos - lastX;
  float yOffset = yPos - lastY;

  lastX = xPos;
  lastY = yPos;

  camera.ProcessCameraLockAt(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
  camera.ProcessCameraZoom(static_cast<float>(yOffset));
}