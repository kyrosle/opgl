// 顶点数组对象: VAO
// 顶点缓冲对象: VBO
// 元素缓冲对象/索引缓冲对象: EBO/IBO
// Graphics Pipeline: Vertex Shader -> Primitive
// Assembly -> Geometry Shader
// -> Rasterization Stage -> Fragment Shader
// -> Alpha testing and Blending

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

// 希望渲染的三角形
float vertices[] = {
    -0.5f, -0.5f, 0.0f, // a
    0.5f,  -0.5f, 0.0f, // b
    0.0f,  0.5f,  0.0f  // c
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
  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
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

  // ------------------------
  // ------ 顶点着色器 ------
  // ------------------------
  // OpenGL 3.3^ core-mode
  // #version 330 core
  // 在顶点着色器中声明所有的输入顶点属性
  // 设定了输入变量的位置值
  // layout (location = 0) in vec3 aPos;
  //
  // void main()
  // {
  //      gl_Position设置的值会成为该顶点着色器的输出
  //     gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
  // }

  // ------ 编译着色器 ------
  // 创建着色器对象, 用ID来引用
  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER /* 表示顶点着色器 */);
  // 着色器源码附加到着色器对象上，然后编译
  glShaderSource(vertexShader /* 着色器对象 */, 1 /* 源码字符串数量 */,
                 &vertexShaderSource /* 顶点着色器真正源码 */, NULL);
  glCompileShader(vertexShader);
  // 检查编译是否成功:
  // int success;
  // char infoLog[512];
  // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  // if (!success) {
  //   glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
  //   std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
  //             << infoLog << std::endl;
  // }

  // ------ 片段着色器 ------
  const char *fragmentShaderSource =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
      "}\0";
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER /* 片段着色器 */);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // ------ 着色器程序 ------
  // 着色器程序对象(Shader Program
  // Object)是多个着色器合并之后并最终链接完成的版本
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // 检查错误:
  // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  // if (!success) {
  //   glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
  //   ...
  // }
  // 在把着色器对象链接到程序对象以后，记得删除着色器对象
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // -> 当我们渲染一个物体时要使用着色器程序
  // glUseProgram(shaderProgram);

  // ------------------------
  // ------ 顶点输入 ------
  // ------------------------
  // 0. 复制顶点数组到缓冲区中供OpenGL使用
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型
  // 顶点缓冲对象: GL_ARRAY_BUFFER
  // 把新创建的缓冲绑定到 GL_ARRAY_BUFFER 目标上
  // 任何 (GL_ARRAY_BUFFER)缓冲调用 都会用来配置当前绑定的 缓冲VBO
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 把之前定义的顶点数据复制到缓冲内存中
  // 显卡如何管理给定的数据
  // 1.GL_STATIC_DRAW: 数据不会几乎不改变
  // 2.GL_DYNAMIC_DRAW：数据改变很多
  // 3.GL_STREAM_DRAW: 数据每次绘制都会改变
  // (2/3)这样就能确保显卡把数据放在能够高速写入的内存部分
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // ------ 链接顶点属性 ------
  // 1. 设置顶点属性指针
  // 如何解析顶点数据
  // glVertexAttribPointer(0 /* 第一个顶点位置 */, 3 /* 顶点属大小 */,
  //                       GL_FLOAT /* 指定数据的类型 */,
  //                       GL_FALSE /* 是否希望数据被标准化(Normalize) (0~1) */,
  //                       3 * sizeof(float) /* 步长 */, (void *)0);
  // 以顶点属性位置值作为参数，启用顶点属性
  // glEnableVertexAttribArray(0);
  // -> 2. 渲染一个物体时要使用着色器程序
  // glUseProgram(shaderProgram);
  // ...

  // ------------------------
  // ------ 顶点数组对象 ------
  // ------------------------
  // 随后的顶点属性调用都会储存在这个VAO中
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // 1. 绑定 VAO
  glBindVertexArray(VAO);
  // 2. 把顶点数组复制到缓冲区供 OpenGL 使用
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 3. 设置顶点属性指针
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
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
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
