
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
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, //

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, //

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //
    -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f, //
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, //
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, //
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, //

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //
    0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, //
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, //
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, //

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //
    0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, //
    -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, //
    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, //

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, //
    0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, //
    -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, //
    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f  //
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

const glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),    //
    glm::vec3(2.3f, -3.3f, -4.0f),  //
    glm::vec3(-4.0f, 2.0f, -12.0f), //
    glm::vec3(0.0f, 0.0f, -3.0f)    //
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

  Shader::ptr lightingShader(
      new Shader("asserts/multiplelight/lighting_maps.vs",
                 "asserts/multiplelight/lighting_maps.fs"));
  Shader::ptr lightCubeShader(
      new Shader("asserts/multiplelight/light_cube.vs",
                 "asserts/multiplelight/light_cube.fs"));

  unsigned int VBO, cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  Texture2D::ptr diffuseMap(
      new Texture2D("asserts/container2.png", "material.diffuse"));
  diffuseMap->Generate();

  Texture2D::ptr specular(
      new Texture2D("asserts/container2_specular.png", "material.specular"));
  specular->Generate();

  Texture2D::ptr avatar(new Texture2D("asserts/avatar.jpg", "material.avatar"));
  avatar->Generate();

  // lightingShader->useProgram();
  // lightingShader->setInt("material.diffuse", 0);
  // lightingShader->setInt("material.specular", 1);
  lightingShader->addTexture(diffuseMap);
  lightingShader->addTexture(specular);
  lightingShader->addTexture(avatar);

  lightingShader->setUpTextures();

  // 灯光源 cube
  unsigned int lightCubeVAO;
  glGenVertexArrays(1, &lightCubeVAO);
  glBindVertexArray(lightCubeVAO);

  // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
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
    // directional light
    lightingShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    lightingShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    lightingShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    lightingShader->setVec3("pointLights[0].position", pointLightPositions[0]);
    lightingShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    lightingShader->setFloat("pointLights[0].constant", 1.0f);
    lightingShader->setFloat("pointLights[0].linear", 0.09f);
    lightingShader->setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
    lightingShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    lightingShader->setFloat("pointLights[1].constant", 1.0f);
    lightingShader->setFloat("pointLights[1].linear", 0.09f);
    lightingShader->setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    lightingShader->setVec3("pointLights[2].position", pointLightPositions[2]);
    lightingShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    lightingShader->setFloat("pointLights[2].constant", 1.0f);
    lightingShader->setFloat("pointLights[2].linear", 0.09f);
    lightingShader->setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    lightingShader->setVec3("pointLights[3].position", pointLightPositions[3]);
    lightingShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    lightingShader->setFloat("pointLights[3].constant", 1.0f);
    lightingShader->setFloat("pointLights[3].linear", 0.09f);
    lightingShader->setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    lightingShader->setVec3("spotLight.position", camera.Position);
    lightingShader->setVec3("spotLight.direction", camera.Front);
    lightingShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    lightingShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    lightingShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    lightingShader->setFloat("spotLight.constant", 1.0f);
    lightingShader->setFloat("spotLight.linear", 0.09f);
    lightingShader->setFloat("spotLight.quadratic", 0.032f);
    lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader->setFloat("spotLight.outerCutOff",
                             glm::cos(glm::radians(15.0f)));

    lightingShader->setFloat("material.shininess", 32.0f);

    // glm::vec3 lightColor = glm::vec3(1.0f);
    // lightColor.x = std::sin(glfwGetTime() * 2.0f);
    // lightColor.y = std::sin(glfwGetTime() * 0.7f);
    // lightColor.z = std::sin(glfwGetTime() * 1.3f);

    // glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    // glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);

    // lightingShader->setVec3("light.ambient", ambientColor);
    // lightingShader->setVec3("light.diffuse", diffuseColor);

    // view/projection transformations
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader->setMat4("projection", projection);
    lightingShader->setMat4("view", view);

    lightingShader->activeTexture();

    // world transformation
    // glm::mat4 model = glm::mat4(1.0f);
    // lightingShader->setMat4("model", model);

    glBindVertexArray(cubeVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    for (unsigned int i = 0; i < 10; ++i) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

      lightingShader->setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object
    lightCubeShader->useProgram();
    lightCubeShader->setMat4("projection", projection);
    lightCubeShader->setMat4("view", view);

    // lightCubeShader->setVec3("lightColor", lightColor);

    // 把灯位移到这里，然后将它缩小一点，让它不那么明显

    glBindVertexArray(lightCubeVAO);
    for (unsigned int i = 0; i < 4; ++i) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
      lightCubeShader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

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