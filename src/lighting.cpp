/*
环境光照(Ambient Lighting)：
即使在黑暗的情况下，世界上通常也仍然有一些光亮（月亮、远处的光），所以物体几乎永远不会是完全黑暗的。
为了模拟这个，我们会使用一个环境光照常量，它永远会给物体一些颜色。

漫反射光照(Diffuse Lighting)：
模拟光源对物体的方向性影响(Directional Impact)。
它是冯氏光照模型中视觉上最显著的分量。物体的某一部分越是正对着光源，它就会越亮。

镜面光照(Specular Lighting)：
模拟有光泽物体上面出现的亮点。镜面光照的颜色相比于物体的颜色会更倾向于光的颜色。
*/

#include <GLFW/glfw3.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "picture.h"
#include "shader.h"
#include "stb_image.h"
#include "texture.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, //

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f, //

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //
    -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f, //
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f, //
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f, //
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f, //

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //
    0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, //
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f, //
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, //

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, //

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f  //
};
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 光源在场景的世界空间坐标中的位置
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
  // glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // 启用深度测试
  glEnable(GL_DEPTH_TEST);

  Shader::ptr lightingShader(new Shader("asserts/lighting/basic_lighting.vs",
                                        "asserts/lighting/basic_lighting.fs"));
  Shader::ptr lightCubeShader(new Shader("asserts/lighting/light_cube.vs",
                                         "asserts/lighting/light_cube.fs"));

  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // 灯光源 cube
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
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
    lightPos.x = 1.0f + std::sin(glfwGetTime()) * 2.0f;
    lightPos.y = std::sin(glfwGetTime() / 2.0f) * 1.0f;

    lightingShader->useProgram();
    lightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    lightingShader->setVec3("lightPos", lightPos);
    lightingShader->setVec3("viewPos", camera.Position);

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

    // 把灯位移到这里，然后将它缩小一点，让它不那么明显
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

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    camera.ProcessCameraLockAt(-10.0f, 0.0f);
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    camera.ProcessCameraLockAt(10.0f, 0.0f);

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    camera.ProcessCameraLockAt(0.0f, 10.0f);
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    camera.ProcessCameraLockAt(0.0f, -10.0f);

  if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
    camera.ProcessCameraZoom(2.0f);
  if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
    camera.ProcessCameraZoom(-2.0f);
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