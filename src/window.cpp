#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

// 用户改变窗口大小时候，view应该被调整,每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window) {
  // `glfwGetKey` 检测对应按键是否被按下
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  // ------ glfw ------
  // 初始化 glfw
  glfwInit();
  // 主版号
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // 次版号
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfw 使用主核心模式 core-profile 只能使用OpenGL功能的一个子集
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 创建窗口对象
  // width, height, title, (monitor, share)
  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // ------ glad ------
  // 初始化 GLAD
  // 给 GLAD 传入了用来加载系统相关的 OpenGL 函数指针地址的函数
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // ------ view ------
  // 窗口左下角(x,y)， 窗口width, 窗口height
  glViewport(0, 0, 800, 600);

  // 告诉 GLFW 我们希望每当窗口调整大小的时候调用这个函数
  // 当窗口被第一次显示的时候 framebuffer_size_callback 也会被调用
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // ------ Render Loop ------
  // 每次循环开始前检测 GLFW 是否被要求退出
  while (!glfwWindowShouldClose(window)) {
    // 按下 ecs glfw 退出
    processInput(window);

    // 渲染指令
    // glClear 清空屏幕颜色, 并填充 glClearColor 里面的颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
    glClear(GL_COLOR_BUFFER_BIT);

    // (Double Buffer)
    // 前缓冲保存着最终输出的图像，它会在屏幕上显示,
    // 而所有的的渲染指令都会在后缓冲上绘制
    //
    // 交换颜色缓冲, 作为输出显示到屏幕上
    glfwSwapBuffers(window);
    // 检查触发事件
    // 键盘输入， 鼠标移动， 更新窗口状态并调用对应回调函数
    glfwPollEvents();
  }

  // ------ end ------
  // 释放资源
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
